#pragma once

#include <juce_dsp/juce_dsp.h>
#include <memory>
#include <vector>
#include <cmath>
#include "../rt/MaskColumnLite.h"
#include "../rt/SpscQueueLite.h"

/**
 * @file SpectralPaintNode.h
 * @brief Modern JUCE DSP-based spectral painting processor
 * 
 * Fast-paint mode using 256-point FFT with 64-sample hop (75% overlap).
 * Optimized for low latency while maintaining audio quality through
 * proper windowing and overlap-add reconstruction.
 * 
 * Features:
 * - JUCE's optimized FFT implementation
 * - Pink noise bed for audible strokes on silence
 * - Radial frequency boosting around painted bins
 * - RT-safe operation with zero allocations
 * - Log-frequency mapping (80Hz - 8kHz)
 */

namespace nodes {

class SpectralPaintNode : public juce::dsp::ProcessorBase
{
public:
    /// Initialize with default settings
    SpectralPaintNode();
    
    /// Virtual destructor for inheritance
    ~SpectralPaintNode() override = default;
    
    // ProcessorBase interface
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) override;
    void reset() override;
    
    /**
     * @brief Push paint event from UI thread
     * @param mask Lightweight paint event (12 bytes)
     * @return true if queued, false if dropped due to full queue
     * 
     * Thread Safety: UI thread only, never blocks
     */
    bool pushMask(const rt::MaskColumnLite& mask) noexcept;
    
    /**
     * @brief Enable/disable pink noise bed
     * @param enabled If true, adds -36dBFS pink noise for audible strokes
     * 
     * Pink bed makes paint strokes audible even on silence.
     */
    void setPinkBed(bool enabled) noexcept { enablePinkBed_.store(enabled, std::memory_order_relaxed); }
    
    /**
     * @brief Set brush radius for radial boosting
     * @param radius Radius in FFT bins (default: 16)
     */
    void setBrushRadius(int radius) noexcept { brushRadius_.store(radius, std::memory_order_relaxed); }
    
    /// Get current configuration
    int getFFTOrder() const noexcept { return FFT_ORDER; }
    size_t getFFTSize() const noexcept { return FFT_SIZE; }
    int getHopSize() const noexcept { return HOP_SIZE; }
    int getNumBins() const noexcept { return NUM_BINS; }
    
    /// Queue statistics for HUD display
    size_t getMaskQueueDepth() const noexcept { return maskQueue_.size(); }
    size_t getMaskDropCount() const noexcept { return maskQueue_.get_drop_count(); }

private:
    // Fast-paint configuration
    static constexpr int FFT_ORDER = 8;              // 256-point FFT
    static constexpr size_t FFT_SIZE = 1u << FFT_ORDER; // 256
    static constexpr int HOP_SIZE = FFT_SIZE / 4;    // 64 (75% overlap)
    static constexpr int NUM_BINS = FFT_SIZE / 2;    // 128 complex bins
    static constexpr size_t QUEUE_CAPACITY = 128;   // Mask event queue size
    
    // Audio parameters
    double sampleRate_ = 48000.0;
    int writeIndex_ = 0;
    
    // JUCE DSP components
    std::unique_ptr<juce::dsp::FFT> fft_;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> window_;
    
    // Processing buffers (pre-allocated, never resized)
    std::vector<float> timeBuffer_;     // Input windowing
    std::vector<float> freqBuffer_;     // FFT processing  
    std::vector<float> overlapBuffer_;  // OLA reconstruction
    std::vector<float> windowedFrame_;  // Temporary windowed frame buffer
    
    // RT-safe communication
    rt::SpscQueueLite<rt::MaskColumnLite, QUEUE_CAPACITY> maskQueue_;
    
    // Configuration atomics
    std::atomic<bool> enablePinkBed_{true};
    std::atomic<int> brushRadius_{16};
    
    // Pink noise generator state
    uint32_t pinkSeed_ = 0xdeadbeefu;
    std::vector<float> pinkPhases_;  // Persistent phases for smooth pink bed
    
    // Helper methods
    
    /**
     * @brief Map UI Y coordinate to FFT bin
     * @param y Normalized Y coordinate [0,1] 
     * @return FFT bin index [0, NUM_BINS-1]
     * 
     * Uses logarithmic frequency mapping from 80Hz to 8kHz
     * for musically natural painting behavior.
     */
    int mapYToBin(float y) const noexcept;
    
    /**
     * @brief Apply radial frequency boost around center bin
     * @param freqData FFT frequency data (real/imag interleaved)
     * @param numBins Number of complex bins to process
     * @param centerBin Center bin for boost
     * @param intensity Boost intensity [0,1]
     * @param radius Boost radius in bins
     * 
     * Applies smooth radial falloff for natural-sounding boosts.
     */
    void applyRadialBoost(float* freqData, int numBins, int centerBin, 
                         float intensity, int radius) noexcept;
    
    /**
     * @brief Inject pink noise bed for audible strokes
     * @param freqData FFT frequency data to modify
     * @param numBins Number of complex bins
     * @param dBFS Target level in dBFS (e.g., -36.0f)
     * 
     * Adds shaped noise with 1/sqrt(f) characteristic.
     */
    void injectPinkBed(float* freqData, int numBins, float dBFS) noexcept;
    
    /**
     * @brief Generate random float [0,1] using xorshift
     * @return Pseudo-random value for noise generation
     * 
     * Fast, RT-safe random number generation.
     */
    float generateRandom() noexcept;
    
    /**
     * @brief JUCE-safe accessor for real part of FFT bin
     * @param freqData JUCE real-only packed FFT buffer
     * @param bin Bin index [0, NUM_BINS-1]
     * @return Reference to real component
     * 
     * JUCE packing: [0]=DC, [1]=Nyquist, [2k]=Re{k}, [2k+1]=Im{k}
     */
    inline float& getRealBin(float* freqData, int bin) noexcept;
    
    /**
     * @brief JUCE-safe accessor for imaginary part of FFT bin
     * @param freqData JUCE real-only packed FFT buffer
     * @param bin Bin index [0, NUM_BINS-1]
     * @return Reference to imaginary component (0 for DC/Nyquist)
     */
    inline float& getImagBin(float* freqData, int bin) noexcept;
};

} // namespace nodes