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
    
    // Reset diagnostics
    popCount_.store(0, std::memory_order_relaxed);
    maxMagnitude_.store(0.0f, std::memory_order_relaxed);
}

void SpectralEngineStub::setStride(int stride) {
    baseStride_ = std::clamp(stride, minStride_, maxStride_);
}

void SpectralEngineStub::popAllMaskColumnsInto(MaskColumnQueue& queue) noexcept {
    if (!magnitudes_.get()) return;
    
    // Clear magnitudes at start of each block for proper accumulation
    std::fill_n(magnitudes_.get(), numBins_, 0.0f);
    
    // Drain all available MaskColumns from queue (non-blocking)
    std::optional<MaskColumn> columnOpt;
    bool gotAny = false;
    float maxMagnitudeReceived = 0.0f;
    
    while ((columnOpt = queue.pop()).has_value()) {
        const MaskColumn& column = *columnOpt;
        gotAny = true;
        
        // Update pop counter
        popCount_.fetch_add(1, std::memory_order_relaxed);
        
#if PHASE4_DEBUG_TAP
        // Track debug sequence if available (requires processor debug tap access)
        // Note: This would need processor reference to update debugTap_.pops and lastSeqPopped
        // For now, popCount_ serves as the main diagnostic
#endif
        
        // Max-accumulate mask values (as per sprint brief)
        const int safeBins = std::min(numBins_, static_cast<int>(column.numBins));
        for (int k = 0; k < safeBins; ++k) {
            maxMagnitudeReceived = std::max(maxMagnitudeReceived, column.values[k]);
            magnitudes_[k] = std::max(magnitudes_[k], column.values[k]);
            // Clamp to valid range
            magnitudes_[k] = std::clamp(magnitudes_[k], 0.0f, 1.0f);
        }
        
        // Sample max magnitude every 16 pops for overlay (cheap stride sample)
        if ((popCount_.load(std::memory_order_relaxed) & 0xF) == 0) {
            float maxMag = 0.0f;
            for (int k = 0; k < numBins_; k += 8) { // Stride sample
                maxMag = std::max(maxMag, magnitudes_[k]);
            }
            maxMagnitude_.store(maxMag, std::memory_order_relaxed);
        }
    }
    
    if (!gotAny) {
        // Apply gentle decay only when no new data received this block
        for (int k = 0; k < numBins_; ++k) {
            magnitudes_[k] *= decayRate_; // 0.98f per block
        }
    }
    
    // Debug logging when data is received (reduced frequency)
    #if JUCE_DEBUG
    static int logCounter = 0;
    if (gotAny || ++logCounter % 240 == 0) { // Log processing or every 50ms if no data
        const uint64_t totalPops = popCount_.load(std::memory_order_relaxed);
        
        // Check actual magnitude values in smoothed array
        float actualMaxMag = 0.0f;
        for (int k = 0; k < numBins_; ++k) {
            if (magnitudes_[k] > actualMaxMag) actualMaxMag = magnitudes_[k];
        }
        
        juce::Logger::writeToLog(juce::String::formatted(
            "SpectralEngineStub: gotAny=%s, totalPops=%llu, receivedMag=%.6f, actualMag=%.6f", 
            gotAny ? "true" : "false", totalPops, maxMagnitudeReceived, actualMaxMag));
    }
    #endif
}

void SpectralEngineStub::process(juce::AudioBuffer<float>& outBuffer, float oscGain) noexcept {
    if (!smoothed_.get() || !phases_.get() || !binIncr_.get()) return;
    
    const int numSamples = outBuffer.getNumSamples();
    if (numSamples <= 0) return;
    
    // Debug synthesis parameters
    #if JUCE_DEBUG
    static int processLogCounter = 0;
    if (++processLogCounter % 240 == 0) {  // Every ~50ms
        juce::Logger::writeToLog(juce::String::formatted(
            "SpectralEngineStub::process: oscGain=%.3f, activeBins=%d", 
            oscGain, activeBinCount_));
    }
    #endif
    
    // Apply magnitude smoothing and decay
    smoothMagnitudes();
    
    // Count active bins and adapt stride
    activeBinCount_ = countActiveBins();
    currentStride_ = calculateStride(activeBinCount_);
    
    // Debug logging every 480 blocks (~100ms at 48kHz)
    #if JUCE_DEBUG
    static int debugCounter = 0;
    if (++debugCounter % 480 == 0) {
        float maxMag = 0.0f;
        for (int k = 0; k < numBins_; ++k) {
            maxMag = std::max(maxMag, smoothed_[k]);
        }
        juce::Logger::writeToLog(juce::String::formatted(
            "SpectralEngineStub: activeBins=%d, maxMag=%.6f, oscGain=%.3f", 
            activeBinCount_, maxMag, oscGain));
    }
    #endif
    
    // Skip DC (k=0) and Nyquist (k=numBins-1) bins to prevent rumble/aliasing
    const int startBin = 1;
    const int endBin = numBins_ - 1;
    
    // Synthesize active bins - use current stride but ensure it's reasonable
    const int safeStride = std::max(1, std::min(currentStride_, 2)); // Force max stride=2 for debugging
    int synthesizedBins = 0;
    float totalAmplitude = 0.0f;
    
    for (int k = startBin; k < endBin; k += safeStride) {
        const float amplitude = smoothed_[k] * oscGain;
        
        // Skip bins below threshold (using lowered eps from header)
        if (amplitude <= eps_) continue;
        
        // Force minimum audible amplitude for painted bins to ensure they're heard
        const float minAmplitude = smoothed_[k] > 0.0f ? std::max(amplitude, 0.01f) : amplitude;
        
        // Debug: track synthesis activity
        synthesizedBins++;
        totalAmplitude += minAmplitude;
        
        // Render this bin's contribution
        renderBinAdd(outBuffer, k, minAmplitude);
    }
    
    // Debug log synthesis activity
    #if JUCE_DEBUG
    static int detailedLogCount = 0;
    if (synthesizedBins > 0 && ++detailedLogCount % 60 == 0) { // Every ~1.25 seconds when synthesizing
        juce::Logger::writeToLog(juce::String::formatted(
            "SYNTHESIS: %d bins synthesized, totalAmp=%.3f, oscGain=%.3f", 
            synthesizedBins, totalAmplitude, oscGain));
    }
    #endif
    
    // Debug: Add a quiet test tone at 440Hz if no paint data exists
    #if JUCE_DEBUG
    if (activeBinCount_ == 0) {
        // Calculate 440Hz bin (approximately)
        const int testBin = static_cast<int>((440.0 * fftSize_) / sampleRate_);
        if (testBin > 0 && testBin < numBins_ - 1) {
            renderBinAdd(outBuffer, testBin, 0.01f * oscGain);  // Very quiet test tone
        }
    }
    
    // Debug logging to verify synthesis is working
    static int synthLogCount = 0;
    if (++synthLogCount % 120 == 0) { // Every ~2.5 seconds
        float outputRMS = outBuffer.getNumChannels() > 0 ? outBuffer.getRMSLevel(0, 0, numSamples) : 0.0f;
        
        // Check actual smoothed values
        float maxSmoothed = 0.0f;
        for (int k = 0; k < numBins_; ++k) {
            if (smoothed_[k] > maxSmoothed) maxSmoothed = smoothed_[k];
        }
        
        juce::Logger::writeToLog(juce::String::formatted(
            "SYNTH: activeBins=%d, oscGain=%.3f, maxSmoothed=%.6f, outputRMS=%.6f", 
            activeBinCount_, oscGain, maxSmoothed, outputRMS));
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
        
        // NOTE: magnitudes_[k] NOT cleared here - will be cleared at start of popAllMaskColumnsInto for proper accumulation
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