#include "KeyFilter.h"
#include <algorithm>

namespace dsp {

constexpr std::array<int, 7> KeyFilter::major_;
constexpr std::array<int, 7> KeyFilter::naturalMinor_;

void KeyFilter::prepare(int fftSize, int numBins, double sampleRate) {
    currentFftSize_ = fftSize;
    currentNumBins_ = numBins;
    currentSampleRate_ = sampleRate;
    
    // Initialize double buffers
    lutA_ = std::make_unique<KeyFilterLUT>(numBins);
    lutB_ = std::make_unique<KeyFilterLUT>(numBins);
    
    lutA_->fftSize = fftSize;
    lutA_->numBins = numBins;
    lutA_->sampleRate = sampleRate;
    
    lutB_->fftSize = fftSize;
    lutB_->numBins = numBins;
    lutB_->sampleRate = sampleRate;
    
    // Build initial LUT (C major by default)
    buildInto(*lutA_, 0, ScaleType::Major);
    current_.store(lutA_.get(), std::memory_order_release);
}

void KeyFilter::rebuildAsync(int rootPcParam, ScaleType scale) {
    if (!lutA_ || !lutB_) return;
    
    // Choose the buffer that's not currently active
    auto* inactive = (current_.load() == lutA_.get()) ? lutB_.get() : lutA_.get();
    
    // Build new LUT in inactive buffer
    buildInto(*inactive, rootPcParam, scale);
    
    // Atomically swap to new LUT
    current_.store(inactive, std::memory_order_release);
}

void KeyFilter::apply(float* magnitudes, int numBins) const noexcept {
    if (!enabled.load(std::memory_order_relaxed)) return;
    
    const auto* lut = current_.load(std::memory_order_acquire);
    if (!lut || !lut->mask) return;
    
    const int safeNumBins = std::min(numBins, lut->numBins);
    
    // Gate DC and Nyquist to prevent rumble/aliasing
    if (safeNumBins > 0) magnitudes[0] = 0.0f;  // DC
    if (safeNumBins > 1) magnitudes[safeNumBins - 1] = 0.0f;  // Nyquist
    
    // Apply mask to frequency bins (skip DC and Nyquist)
    for (int k = 1; k < safeNumBins - 1; ++k) {
        magnitudes[k] *= static_cast<float>(lut->mask[k]);
    }
}

void KeyFilter::buildInto(KeyFilterLUT& dst, int rootPcParam, ScaleType scale) const {
    if (!dst.mask) return;
    
    // Build 12-element pitch-class mask
    std::uint8_t scaleMask[12];
    buildScaleMask(scaleMask, rootPcParam, scale);
    
    // Map each bin to its pitch class and apply mask
    for (int k = 0; k < dst.numBins; ++k) {
        // Skip DC and Nyquist - always gate them
        if (k == 0 || k == dst.numBins - 1) {
            dst.mask[k] = 0;
            continue;
        }
        
        // Calculate frequency for this bin
        const double freq = (dst.sampleRate / dst.fftSize) * k;
        
        // Skip invalid frequencies
        if (freq <= 0.0 || !std::isfinite(freq)) {
            dst.mask[k] = 0;
            continue;
        }
        
        // Convert to MIDI note number
        const double midi = std::round(69.0 + 12.0 * std::log2(freq / 440.0));
        
        // Extract pitch class (0-11)
        const int pc = static_cast<int>(std::fmod(std::fmod(midi, 12.0) + 12.0, 12.0));
        
        // Apply scale mask
        dst.mask[k] = scaleMask[pc];
    }
}

void KeyFilter::buildScaleMask(std::uint8_t* out12, int rootPc, ScaleType scale) {
    std::memset(out12, 0, 12);
    
    switch (scale) {
        case ScaleType::Chromatic:
            // Allow all pitch classes
            std::memset(out12, 1, 12);
            break;
            
        case ScaleType::Major:
            for (int pc : major_) {
                out12[(pc + rootPc + 12) % 12] = 1;
            }
            break;
            
        case ScaleType::NaturalMinor:
            for (int pc : naturalMinor_) {
                out12[(pc + rootPc + 12) % 12] = 1;
            }
            break;
    }
}

}  // namespace dsp