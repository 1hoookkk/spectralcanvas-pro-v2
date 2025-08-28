#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Core/RealtimeSafeTypes.h"
#include "../Core/MessageBus.h"

class SpectralEngine
{
public:
    static constexpr size_t FFT_SIZE = 512;
    static constexpr size_t HOP_SIZE = 128;  
    static constexpr size_t NUM_BINS = FFT_SIZE / 2 + 1;
    static constexpr size_t OVERLAP_BUFFER_SIZE = FFT_SIZE * 4;

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
    
    // Mask processing (RT-safe)
    void updateCurrentMask(const MaskColumn* maskColumn) noexcept;
    void updateCurrentMask(const float* maskPtr, int numBins) noexcept;
    
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
    AlignedStorage<std::array<float, FFT_SIZE>> fftBuffer_;
    AlignedStorage<std::array<float, FFT_SIZE>> ifftBuffer_;
    AlignedStorage<std::array<float, FFT_SIZE>> windowedBuffer_;
    AlignedStorage<std::array<float, FFT_SIZE>> overlapBuffer_;
    
    // Spectral data (magnitude and phase)
    AlignedStorage<std::array<float, NUM_BINS>> currentMagnitude_;
    AlignedStorage<std::array<float, NUM_BINS>> currentPhase_;
    AlignedStorage<std::array<float, NUM_BINS>> previousPhase_;
    
    // Input/output circular buffers for overlap-add processing
    RTCircularBuffer<float, OVERLAP_BUFFER_SIZE> inputBuffer_;
    RTCircularBuffer<float, OVERLAP_BUFFER_SIZE> outputBuffer_;
    
    // FFT processing
    std::unique_ptr<juce::dsp::FFT> forwardFFT_;
    std::unique_ptr<juce::dsp::FFT> inverseFFT_;
    RTWindow<FFT_SIZE> hannWindow_;
    
    // State management
    std::atomic<bool> initialized_;
    std::atomic<int> samplesUntilNextFrame_;
    std::atomic<double> sampleRate_;
    
    // RT-safe parameters
    std::atomic<float> spectralGain_;
    std::atomic<float> spectralMix_;
    
    // Current mask for spectral painting (RT-safe atomic pointer)
    std::atomic<const float*> currentMask_; // Points to MaskColumn::values
    
    // Frame tracking
    RTSequenceGenerator sequenceGenerator_;
    std::atomic<double> currentTimeInSamples_;
    
    // Spectral analysis
    std::atomic<float> spectralCentroid_;
    std::atomic<float> fundamentalFreq_;
};