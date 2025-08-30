#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "../Core/MessageBus.h"
#include <atomic>

namespace dsp {

class SpectralEngineStub {
public:
    SpectralEngineStub() = default;
    ~SpectralEngineStub() = default;
    
    // Initialize with audio parameters
    void prepare(double sampleRate, int fftSize, int numBins, int channels);
    
    // Reset all internal state
    void reset();
    
    // Set oscillator stride (2-4 for CPU control)
    void setStride(int stride);
    
    // Called on audio thread per block - main synthesis
    void process(juce::AudioBuffer<float>& outBuffer, float oscGain) noexcept;
    
    // Called on audio thread before process - drain MaskColumn queue
    void popAllMaskColumnsInto(MaskColumnQueue& queue) noexcept;
    
    // Exposed for KeyFilter to operate on magnitudes before synthesis
    float* getMagnitudesWritePtr() noexcept { return magnitudes_.get(); }
    const float* getMagnitudesReadPtr() const noexcept { return magnitudes_.get(); }
    
    // Statistics for performance monitoring
    int getActiveBinCount() const noexcept { return activeBinCount_; }
    int getCurrentStride() const noexcept { return currentStride_; }
    int getNumBins() const noexcept { return numBins_; }  // Expose for diagnostics
    
    // Queue diagnostics (thread-safe accessors)
    uint64_t getPopCount() const noexcept { return popCount_.load(std::memory_order_relaxed); }
    float getMaxMagnitude() const noexcept { return maxMagnitude_.load(std::memory_order_relaxed); }

private:
    // Audio parameters
    double sampleRate_ = 48000.0;
    int fftSize_ = 512;
    int numBins_ = 257;
    int channels_ = 2;
    
    // Performance control
    static constexpr int minStride_ = 1;
    static constexpr int maxStride_ = 4;
    int baseStride_ = 2;  // More permissive for bring-up
    int currentStride_ = 2;  // More permissive for bring-up
    static constexpr float eps_ = 2e-6f;  // Lower threshold for audibility
    
    // Pre-allocated RT-safe buffers
    juce::HeapBlock<float> magnitudes_;     // Current target magnitudes [numBins]
    juce::HeapBlock<float> smoothed_;       // Smoothed magnitudes [numBins]  
    juce::HeapBlock<float> phases_;         // Phase accumulators [numBins]
    juce::HeapBlock<float> binIncr_;        // Per-sample phase increment [numBins]
    
    // Performance tracking
    int activeBinCount_ = 0;
    
    // Queue diagnostics atomics (RT-safe)
    std::atomic<uint64_t> popCount_{0};
    std::atomic<float> maxMagnitude_{0.0f};
    
    // Smoothing parameters
    static constexpr float smoothingRate_ = 0.2f;   // τ≈5 blocks
    static constexpr float decayRate_ = 0.98f;      // Age decay per block
    
    // Calculate adaptive stride based on active bin count
    int calculateStride(int activeBinCount) const noexcept;
    
    // Apply per-bin magnitude smoothing and decay
    void smoothMagnitudes() noexcept;
    
    // Count active bins above threshold
    int countActiveBins() const noexcept;
    
    // Render single bin contribution to output buffer
    void renderBinAdd(juce::AudioBuffer<float>& outBuffer, int bin, float amplitude) noexcept;
    
};

}  // namespace dsp