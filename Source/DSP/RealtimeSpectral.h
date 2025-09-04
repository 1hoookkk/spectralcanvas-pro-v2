#pragma once
#include <juce_dsp/juce_dsp.h>
#include <vector>
#include <cmath>
#include <atomic>
#include "../Core/AtlasIds.h"

/// Forward STFT (real FFT) + magnitude-scaling + inverse + OLA.
/// Preserves phase by scaling complex bins by (maskedMag / mag).
/// RT-SAFE: Uses precomputed window tables to avoid placement-new race conditions.
class RealtimeSpectral
{
public:
    void prepare(double sampleRate, int fftSize, int hop)
    {
        sr_ = sampleRate; fftSize_ = fftSize; hop_ = hop; bins_ = fftSize / 2;
        fft_ = juce::dsp::FFT((int)std::log2((double)fftSize_));
        
        // RT-SAFE: Precompute window tables instead of placement-new (fixes editor crash)
        // This avoids race condition where GUI thread destructs WindowingFunction 
        // while audio thread is using it
        anaWinTable_.assign((size_t)fftSize_, 0.0f);
        synWinTable_.assign((size_t)fftSize_, 0.0f);
        juce::dsp::WindowingFunction<float>::fillWindowingTables(
            anaWinTable_.data(), (size_t)fftSize_, 
            juce::dsp::WindowingFunction<float>::hann, false);
        juce::dsp::WindowingFunction<float>::fillWindowingTables(
            synWinTable_.data(), (size_t)fftSize_, 
            juce::dsp::WindowingFunction<float>::hann, false);
        
        anaBuf_.assign((size_t)fftSize_, 0.0f);
        timeWin_.assign((size_t)fftSize_, 0.0f);  // Pre-allocated temp buffer for windowing
        // JUCE real-only FFT uses interleaved re/imag storage; allocate 2*fft
        spec_.assign((size_t)fftSize_ * 2, 0.0f);
        ola_.assign((size_t)fftSize_ + hop_, 0.0f);
        writePos_ = 0;
        inputFifo_.assign((size_t)hop_, 0.0f);
        fifoPos_ = 0;
        
        // Atomically mark ready for RT use
        currentFftSize_.store(fftSize_, std::memory_order_release);
    }

    /// Push a block of input and render masked output (mono).
    /// maskCol may be null (treated as all-ones).
    void processBlock(const float* in, float* out, int numSamples, const float* maskCol)
    {
        int n = 0;
        while (n < numSamples)
        {
            const int toCopy = juce::jmin(hop_ - fifoPos_, numSamples - n);
            std::memcpy(&inputFifo_[(size_t)fifoPos_], &in[n], sizeof(float) * (size_t)toCopy);
            fifoPos_ += toCopy; n += toCopy;

            if (fifoPos_ == hop_)
            {
                // Assemble frame: last fftSize samples (hop advance)
                // Shift previous fftSize - hop samples
                const int tail = fftSize_ - hop_;
                if (tail > 0)
                    std::memmove(anaBuf_.data(), anaBuf_.data() + hop_, sizeof(float) * (size_t)tail);
                std::memcpy(anaBuf_.data() + tail, inputFifo_.data(), sizeof(float) * (size_t)hop_);

                // Forward FFT → complex spectrum
                std::memcpy(timeWin_.data(), anaBuf_.data(), sizeof(float) * (size_t)fftSize_);
                
                // RT-SAFE: Apply analysis window from precomputed table
                const int currentSize = currentFftSize_.load(std::memory_order_acquire);
                if (currentSize == fftSize_) {
                    for (int i = 0; i < fftSize_; ++i)
                        timeWin_[(size_t)i] *= anaWinTable_[(size_t)i];
                }
                
                std::fill(spec_.begin(), spec_.end(), 0.0f);
                std::memcpy(spec_.data(), timeWin_.data(), sizeof(float) * (size_t)fftSize_);
                fft_.performRealOnlyForwardTransform(spec_.data());

                // Magnitude + scale bins (preserve phase)
                for (int k = 0; k < bins_; ++k)
                {
                    float& re = spec_[(size_t)k * 2];
                    float& im = spec_[(size_t)k * 2 + 1];
                    const float m = maskCol ? maskCol[k] : 1.0f;        // 0..1 attenuation
                    re *= m;
                    im *= m;
                }

                // Inverse + synthesis window
                fft_.performRealOnlyInverseTransform(spec_.data());
                
                // RT-SAFE: Apply synthesis window from precomputed table
                if (currentSize == fftSize_) {
                    for (int i = 0; i < fftSize_; ++i)
                        spec_[(size_t)i] *= synWinTable_[(size_t)i];
                }

                // Overlap-add into circular buffer and output hop samples
                for (int i = 0; i < hop_; ++i)
                {
                    const int wi = (writePos_ + i) % (int)ola_.size();
                    out[i + n - hop_] = ola_[wi] + spec_[(size_t)i];
                    ola_[wi] = 0.0f;
                }
                for (int i = hop_; i < fftSize_; ++i)
                {
                    const int wi = (writePos_ + i) % (int)ola_.size();
                    ola_[wi] += spec_[(size_t)i];
                }
                writePos_ = (writePos_ + hop_) % (int)ola_.size();

                // Reset hop FIFO
                fifoPos_ = 0;
            }
        }
    }

private:
    double sr_{48000.0};
    int fftSize_{2048}, hop_{256}, bins_{1024};
    juce::dsp::FFT fft_{11};
    
    // RT-SAFE: Precomputed window tables (eliminates placement-new race condition)
    std::vector<float> anaWinTable_, synWinTable_;
    std::atomic<int> currentFftSize_{0};  // Atomic size validation for RT safety
    
    std::vector<float> anaBuf_, timeWin_, spec_, ola_, inputFifo_;
    int writePos_{0};
    int fifoPos_{0};
};