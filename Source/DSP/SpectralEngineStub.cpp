#include "SpectralEngineStub.h"
#include <algorithm>
#include <cmath>

namespace dsp {

void SpectralEngineStub::prepare(double sampleRate, int fftSize, int numBins, int channels) {
    sampleRate_ = sampleRate;
    fftSize_ = fftSize;
    numBins_ = numBins;
    channels_ = channels;
    
    // Pre-allocate all RT-safe buffers
    magnitudes_.allocate(numBins, true);  // Zero-initialized
    smoothed_.allocate(numBins, true);
    phases_.allocate(numBins, true);
    binIncr_.allocate(numBins, true);
    
    // Precompute per-bin phase increments (2π * k / fftSize)
    const double twoPiOverFft = juce::MathConstants<double>::twoPi / fftSize;
    for (int k = 0; k < numBins; ++k) {
        binIncr_[k] = static_cast<float>(twoPiOverFft * k);
    }
    
    reset();
}

void SpectralEngineStub::reset() {
    if (!magnitudes_.get()) return;
    
    // Clear all state
    std::fill_n(magnitudes_.get(), numBins_, 0.0f);
    std::fill_n(smoothed_.get(), numBins_, 0.0f);
    std::fill_n(phases_.get(), numBins_, 0.0f);
    
    activeBinCount_ = 0;
    currentStride_ = baseStride_;
}

void SpectralEngineStub::setStride(int stride) {
    baseStride_ = std::clamp(stride, minStride_, maxStride_);
}

void SpectralEngineStub::popAllMaskColumnsInto(MaskColumnQueue& queue) noexcept {
    if (!magnitudes_.get()) return;
    
    // Drain all available MaskColumns from queue (non-blocking)
    std::optional<MaskColumn> columnOpt;
    while ((columnOpt = queue.pop()).has_value()) {
        const MaskColumn& column = *columnOpt;
        // Accumulate mask values using max operation (as specified)  
        const int safeBins = std::min(numBins_, static_cast<int>(column.numBins));
        for (int k = 0; k < safeBins; ++k) {
            magnitudes_[k] = std::max(magnitudes_[k], column.values[k]);
            // Clamp to valid range
            magnitudes_[k] = std::clamp(magnitudes_[k], 0.0f, 1.0f);
        }
    }
}

void SpectralEngineStub::process(juce::AudioBuffer<float>& outBuffer, float oscGain) noexcept {
    if (!smoothed_.get() || !phases_.get() || !binIncr_.get()) return;
    
    const int numSamples = outBuffer.getNumSamples();
    if (numSamples <= 0) return;
    
    // Apply magnitude smoothing and decay
    smoothMagnitudes();
    
    // Count active bins and adapt stride
    activeBinCount_ = countActiveBins();
    currentStride_ = calculateStride(activeBinCount_);
    
    // Skip DC (k=0) and Nyquist (k=numBins-1) bins to prevent rumble/aliasing
    const int startBin = 1;
    const int endBin = numBins_ - 1;
    
    // Synthesize active bins with adaptive stride
    for (int k = startBin; k < endBin; k += currentStride_) {
        const float amplitude = smoothed_[k] * oscGain;
        
        // Skip bins below threshold
        if (amplitude <= eps_) continue;
        
        // Render this bin's contribution
        renderBinAdd(outBuffer, k, amplitude);
    }
    
    // Debug: Add a quiet test tone at 440Hz if no paint data exists
    #if JUCE_DEBUG
    if (activeBinCount_ == 0) {
        // Calculate 440Hz bin (approximately)
        const int testBin = static_cast<int>((440.0 * fftSize_) / sampleRate_);
        if (testBin > 0 && testBin < numBins_ - 1) {
            renderBinAdd(outBuffer, testBin, 0.01f * oscGain);  // Very quiet test tone
        }
    }
    #endif
}

int SpectralEngineStub::calculateStride(int activeBinCount) const noexcept {
    // Adaptive stride based on active bin count
    if (activeBinCount < 128) return 1;      // Full resolution when sparse
    if (activeBinCount < 256) return 2;      // Half resolution for medium density
    return std::max(baseStride_, 3);         // Conservative stride for dense paint
}

void SpectralEngineStub::smoothMagnitudes() noexcept {
    if (!magnitudes_.get() || !smoothed_.get()) return;
    
    // Per-bin smoothing with age decay
    for (int k = 0; k < numBins_; ++k) {
        // Smooth towards target with 1st-order filter
        smoothed_[k] += smoothingRate_ * (magnitudes_[k] - smoothed_[k]);
        
        // Age decay to prevent stuck tones
        smoothed_[k] *= decayRate_;
        
        // Clear target after smoothing (for next accumulation)
        magnitudes_[k] = 0.0f;
    }
}

int SpectralEngineStub::countActiveBins() const noexcept {
    if (!smoothed_.get()) return 0;
    
    int count = 0;
    for (int k = 1; k < numBins_ - 1; ++k) {  // Skip DC and Nyquist
        if (smoothed_[k] > eps_) {
            ++count;
        }
    }
    return count;
}

void SpectralEngineStub::renderBinAdd(juce::AudioBuffer<float>& outBuffer, int bin, float amplitude) noexcept {
    if (!phases_.get() || !binIncr_.get()) return;
    if (bin < 0 || bin >= numBins_) return;
    
    const int numSamples = outBuffer.getNumSamples();
    const int numChannels = std::min(outBuffer.getNumChannels(), channels_);
    const float phaseIncr = binIncr_[bin];
    float phase = phases_[bin];
    
    // Per-sample synthesis with phase continuity
    for (int sample = 0; sample < numSamples; ++sample) {
        const float sineValue = std::sin(phase) * amplitude;
        
        // Add to all output channels (mono synthesis, centered)
        for (int ch = 0; ch < numChannels; ++ch) {
            outBuffer.addSample(ch, sample, sineValue);
        }
        
        // Advance phase with wraparound
        phase += phaseIncr;
        if (phase > juce::MathConstants<float>::twoPi) {
            phase -= juce::MathConstants<float>::twoPi;
        }
    }
    
    // Store updated phase
    phases_[bin] = phase;
}

}  // namespace dsp