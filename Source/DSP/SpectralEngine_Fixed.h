#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Core/MessageBus.h"
#include <array>
#include <atomic>

class SpectralEngine
{
public:
    static constexpr size_t FFT_SIZE = 512;
    static constexpr size_t HOP_SIZE = 128;  
    static constexpr size_t NUM_BINS = FFT_SIZE / 2 + 1;

    SpectralEngine() noexcept;
    ~SpectralEngine() noexcept;
    
    // Non-copyable for RT safety
    SpectralEngine(const SpectralEngine&) = delete;
    SpectralEngine& operator=(const SpectralEngine&) = delete;
    
    // Initialization (called from prepareToPlay - may allocate)
    void initialize(double sampleRate, int maxBlockSize);
    void reset() noexcept;
    
    // RT-safe processing (audio thread only)
    void processBlock(const float* inputBuffer, float* outputBuffer, int numSamples) noexcept;
    
    // RT-safe spectral data extraction (audio thread only)
    bool extractSpectralFrame(SpectralFrame& frame) noexcept;
    
    // Parameter updates (RT-safe)
    void setSpectralGain(float gain) noexcept { spectralGain_.store(gain, std::memory_order_relaxed); }
    void setSpectralMix(float mix) noexcept { spectralMix_.store(mix, std::memory_order_relaxed); }
    
    // State queries (RT-safe)
    bool isInitialized() const noexcept { return initialized_.load(std::memory_order_acquire); }
    double getSampleRate() const noexcept { return sampleRate_; }
    
private:
    // RT-safe processing methods
    void performSTFT() noexcept;
    void performiFFT() noexcept;
    void updateSpectralFeatures() noexcept;
    void applySpectralProcessing() noexcept;
    
    // Pre-allocated buffers (never resized after initialization)
    std::array<float, FFT_SIZE> fftBuffer_;
    std::array<float, FFT_SIZE> ifftBuffer_;
    std::array<float, FFT_SIZE> windowedBuffer_;
    std::array<float, FFT_SIZE> overlapBuffer_;
    
    // Spectral data (magnitude and phase)
    std::array<float, NUM_BINS> currentMagnitude_;
    std::array<float, NUM_BINS> currentPhase_;
    std::array<float, NUM_BINS> previousPhase_;
    
    // Simple circular buffers for input/output
    static constexpr size_t BUFFER_SIZE = FFT_SIZE * 4;
    std::array<float, BUFFER_SIZE> inputBuffer_;
    std::array<float, BUFFER_SIZE> outputBuffer_;
    std::atomic<size_t> inputWritePos_{0};
    std::atomic<size_t> inputReadPos_{0};
    std::atomic<size_t> outputWritePos_{0};
    std::atomic<size_t> outputReadPos_{0};
    
    // FFT processing
    std::unique_ptr<juce::dsp::FFT> forwardFFT_;
    std::unique_ptr<juce::dsp::FFT> inverseFFT_;
    std::array<float, FFT_SIZE> hannWindow_;
    
    // State management
    std::atomic<bool> initialized_;
    std::atomic<int> samplesUntilNextFrame_;
    std::atomic<double> sampleRate_;
    
    // RT-safe parameters
    std::atomic<float> spectralGain_;
    std::atomic<float> spectralMix_;
    
    // Frame tracking
    std::atomic<uint32_t> sequenceNumber_;
    std::atomic<double> currentTimeInSamples_;
    
    // Spectral analysis
    std::atomic<float> spectralCentroid_;
    std::atomic<float> fundamentalFreq_;
};