#pragma once
#include <juce_dsp/juce_dsp.h>
#include "../Core/MaskColumn.h"

// Real-time-safe spectral processor
class SpectralProcessor
{
public:
    static constexpr int kFftOrder      = 10;
    static constexpr int kFftSize       = 1 << kFftOrder; // 1024
    static constexpr int kHopSize       = kFftSize / 4;   // 256 (75% overlap)
    static constexpr int kOverlapFactor = 4;

    SpectralProcessor() = default;

    void prepare(double sampleRate, int maxBlockSize);
    void reset();

    // process in effect mode: applies persistent attenuation mask
    void process(juce::AudioBuffer<float>& buffer,
                 float maskFloorDb,
                 float outputTrimDb);

    // bounded drain per block (call from audio thread)
    template <typename QueueT>
    int drainEvents(QueueT& q, int maxPerBlock) noexcept
    {
        int drained = 0;
        MaskColumn ev;
        while (drained < maxPerBlock && q.tryPop(ev))
        {
            applyPaintEvent(ev);
            ++drained;
        }
        return drained;
    }

    // latency in samples to report to host
    int getLatencySamples() const noexcept { return kFftSize - kHopSize; }

    // (optional) expose peak attenuation in dB
    float getPeakGrDb() const noexcept { return peakGrDb_; }

private:
    // packed real-FFT helpers
    static inline void getPackedBin(const float* X, int k, int N, float& re, float& im) noexcept
    {
        if      (k == 0)      { re = X[0]; im = 0.0f; }
        else if (k == N / 2)  { re = X[1]; im = 0.0f; }
        else                  { re = X[2 * k]; im = X[2 * k + 1]; }
    }
    static inline void setPackedBin(float* X, int k, int N, float re, float im) noexcept
    {
        if      (k == 0)      { X[0] = re; }
        else if (k == N / 2)  { X[1] = re; }
        else                  { X[2 * k] = re; X[2 * k + 1] = im; }
    }

    // internal DSP methods
    void processFrame(float maskFloorLin);
    void decayMaskTowardUnity() noexcept;
    void smoothMaskAttackRelease() noexcept;
    void applyMaskToSpectrum(float maskFloorLin) noexcept;
    void applyPaintEvent(const MaskColumn& col) noexcept;

    // state
    double sampleRate_ = 44100.0;
    
    std::unique_ptr<juce::dsp::FFT> fft_;
    std::vector<float> analysisWin_, synthWin_;
    std::vector<float> inputFifo_;
    int fifoWritePos_ = 0;

    std::vector<float> fftBuffer_;
    std::vector<float> olaBuffer_;
    int olaWritePos_ = 0;
    
    // persistent mask (K+1 bins)
    std::vector<float> maskTarget_;
    std::vector<float> maskSmoothed_;
    float decayCoeff_ = 0.0f;
    float atkCoeff_ = 0.0f;
    float relCoeff_ = 0.0f;
    float peakGrDb_   = 0.0f;
};
