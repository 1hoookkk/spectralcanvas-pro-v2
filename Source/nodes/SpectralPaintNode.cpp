#include "SpectralPaintNode.h"
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace nodes {

SpectralPaintNode::SpectralPaintNode() 
    : fft_(std::make_unique<juce::dsp::FFT>(FFT_ORDER))
    , window_(std::make_unique<juce::dsp::WindowingFunction<float>>(FFT_SIZE, juce::dsp::WindowingFunction<float>::hann))
{
    // Pre-allocate all buffers to avoid RT allocations
    timeBuffer_.resize(FFT_SIZE);
    freqBuffer_.resize(FFT_SIZE * 2); // Real/imag interleaved
    overlapBuffer_.resize(FFT_SIZE);
    windowedFrame_.resize(FFT_SIZE);  // Temporary windowed frame buffer
    pinkPhases_.resize(NUM_BINS);  // One phase per bin
    
    // Initialize to zero
    std::fill(timeBuffer_.begin(), timeBuffer_.end(), 0.0f);
    std::fill(freqBuffer_.begin(), freqBuffer_.end(), 0.0f);
    std::fill(overlapBuffer_.begin(), overlapBuffer_.end(), 0.0f);
    std::fill(windowedFrame_.begin(), windowedFrame_.end(), 0.0f);
    std::fill(pinkPhases_.begin(), pinkPhases_.end(), 0.0f);
}

void SpectralPaintNode::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate_ = spec.sampleRate;
    writeIndex_ = 0;
    
    // Reset state
    std::fill(overlapBuffer_.begin(), overlapBuffer_.end(), 0.0f);
    pinkSeed_ = 0xdeadbeefu;
}

void SpectralPaintNode::reset()
{
    writeIndex_ = 0;
    std::fill(overlapBuffer_.begin(), overlapBuffer_.end(), 0.0f);
    std::fill(timeBuffer_.begin(), timeBuffer_.end(), 0.0f);
    std::fill(freqBuffer_.begin(), freqBuffer_.end(), 0.0f);
    std::fill(windowedFrame_.begin(), windowedFrame_.end(), 0.0f);
    std::fill(pinkPhases_.begin(), pinkPhases_.end(), 0.0f);
    pinkSeed_ = 0xdeadbeefu;
}

void SpectralPaintNode::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();
    
    const auto numSamples = static_cast<int>(inputBlock.getNumSamples());
    const auto numChannels = static_cast<int>(inputBlock.getNumChannels());
    
    // Process mono - mix down if stereo input
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Mix input channels to mono
        float inputSample = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
        {
            inputSample += inputBlock.getSample(ch, sample);
        }
        if (numChannels > 1) inputSample /= static_cast<float>(numChannels);
        
        // Store in time buffer at write position
        timeBuffer_[writeIndex_] = inputSample;
        
        // Check if we have enough samples for FFT frame (every HOP_SIZE samples)
        if ((writeIndex_ % HOP_SIZE) == 0 && writeIndex_ >= (FFT_SIZE - HOP_SIZE))
        {
            // Process FFT frame
            processFFTFrame();
        }
        
        // Output is overlap buffer at current position
        float outputSample = overlapBuffer_[writeIndex_ % FFT_SIZE];
        
        // Copy to all output channels
        for (int ch = 0; ch < static_cast<int>(outputBlock.getNumChannels()); ++ch)
        {
            outputBlock.setSample(ch, sample, outputSample);
        }
        
        // Advance write index
        writeIndex_ = (writeIndex_ + 1) % FFT_SIZE;
    }
}

bool SpectralPaintNode::pushMask(const rt::MaskColumnLite& mask) noexcept
{
    return maskQueue_.try_push(mask);
}

void SpectralPaintNode::processFFTFrame() noexcept
{
    // Copy time domain samples to temp buffer
    for (size_t i = 0; i < FFT_SIZE; ++i)
    {
        const size_t bufferIndex = (writeIndex_ + i) % FFT_SIZE;
        windowedFrame_[i] = timeBuffer_[bufferIndex];
    }
    
    // Apply window to time domain data
    window_->multiplyWithWindowingTable(windowedFrame_.data(), FFT_SIZE);
    
    // Copy windowed data to FFT buffer (real part only)
    for (size_t i = 0; i < FFT_SIZE; ++i)
    {
        freqBuffer_[i * 2] = windowedFrame_[i]; // Real
        freqBuffer_[i * 2 + 1] = 0.0f; // Imaginary
    }
    
    // Forward FFT
    fft_->performRealOnlyForwardTransform(freqBuffer_.data());
    
    // Apply pink noise bed if enabled
    if (enablePinkBed_.load(std::memory_order_relaxed))
    {
        injectPinkBed(freqBuffer_.data(), NUM_BINS, -36.0f);
    }
    
    // Process paint masks from queue
    rt::MaskColumnLite mask;
    while (maskQueue_.try_pop(mask))
    {
        const int centerBin = mapYToBin(mask.y);
        const int radius = brushRadius_.load(std::memory_order_relaxed);
        applyRadialBoost(freqBuffer_.data(), NUM_BINS, centerBin, mask.intensity, radius);
    }
    
    // Inverse FFT
    fft_->performRealOnlyInverseTransform(freqBuffer_.data());
    
    // Apply synthesis window and overlap-add
    // COLA condition: for 75% overlap with Hann window, gain = 1.5
    const float colaGain = 1.5f;
    const float normalizationFactor = colaGain / static_cast<float>(FFT_SIZE);
    
    for (size_t i = 0; i < FFT_SIZE; ++i)
    {
        // Get synthesis window value (same as analysis window)
        const float windowValue = window_->getWindowingFactor(i);
        
        // Apply window and normalization
        const float windowedSample = freqBuffer_[i * 2] * windowValue * normalizationFactor;
        
        const size_t outputIndex = (writeIndex_ + i) % FFT_SIZE;
        overlapBuffer_[outputIndex] += windowedSample;
    }
}

int SpectralPaintNode::mapYToBin(float y) const noexcept
{
    // Clamp input to [0,1]
    y = std::max(0.0f, std::min(1.0f, y));
    
    // Log frequency mapping: 80Hz to 8kHz
    constexpr float MIN_FREQ = 80.0f;
    constexpr float MAX_FREQ = 8000.0f;
    constexpr float LOG_RATIO = MAX_FREQ / MIN_FREQ; // 100.0
    
    // Logarithmic interpolation
    const float freq = MIN_FREQ * std::pow(LOG_RATIO, y);
    
    // Convert frequency to FFT bin
    const float binFloat = freq * static_cast<float>(FFT_SIZE) / static_cast<float>(sampleRate_);
    
    // Clamp to valid bin range (avoid DC and Nyquist)
    const int bin = static_cast<int>(binFloat + 0.5f);
    return std::max(1, std::min(NUM_BINS - 2, bin)); // Skip DC (0) and Nyquist (NUM_BINS-1)
}

void SpectralPaintNode::applyRadialBoost(float* freqData, int numBins, int centerBin, 
                                       float intensity, int radius) noexcept
{
    if (intensity <= 0.0f || radius <= 0) return;
    
    // Clamp center bin to valid range (avoid DC and Nyquist)
    centerBin = std::max(1, std::min(numBins - 2, centerBin));
    
    // Apply boost with radial falloff
    for (int bin = 1; bin < numBins - 1; ++bin) // Skip DC and Nyquist
    {
        const int distance = std::abs(bin - centerBin);
        if (distance <= radius)
        {
            // Smooth radial falloff using cosine
            const float normalizedDistance = static_cast<float>(distance) / static_cast<float>(radius);
            const float falloff = 0.5f * (1.0f + std::cos(normalizedDistance * static_cast<float>(M_PI)));
            const float boost = 1.0f + (intensity * falloff * 2.0f); // Up to 2x boost at center
            
            // Apply to real and imaginary parts using JUCE-safe accessors
            getRealBin(freqData, bin) *= boost;
            getImagBin(freqData, bin) *= boost;
        }
    }
}

void SpectralPaintNode::injectPinkBed(float* freqData, int numBins, float dBFS) noexcept
{
    const float amplitude = std::pow(10.0f, dBFS / 20.0f);
    const float phaseIncrement = static_cast<float>(HOP_SIZE) / static_cast<float>(sampleRate_);
    
    for (int bin = 1; bin < numBins - 1; ++bin) // Skip DC and Nyquist
    {
        // Pink noise has 1/sqrt(f) characteristic
        const float freq = static_cast<float>(bin) * static_cast<float>(sampleRate_) / static_cast<float>(FFT_SIZE);
        const float pinkScale = 1.0f / std::sqrt(std::max(1.0f, freq / 100.0f)); // Normalize to 100Hz
        const float scaledAmplitude = amplitude * pinkScale * 0.05f; // Reduced scaling to prevent harshness
        
        // Advance phase continuously for each bin (avoids random phase jumps)
        pinkPhases_[bin] += 2.0f * static_cast<float>(M_PI) * freq * phaseIncrement;
        if (pinkPhases_[bin] > 2.0f * static_cast<float>(M_PI)) 
            pinkPhases_[bin] -= 2.0f * static_cast<float>(M_PI);
            
        // Generate smooth complex sinusoid for this bin
        const float phase = pinkPhases_[bin];
        const float cosPhase = std::cos(phase);
        const float sinPhase = std::sin(phase);
        
        // Add to existing spectrum using JUCE-safe accessors
        getRealBin(freqData, bin) += scaledAmplitude * cosPhase;
        getImagBin(freqData, bin) += scaledAmplitude * sinPhase;
    }
}

float SpectralPaintNode::generateRandom() noexcept
{
    // Xorshift32 for fast RT-safe random generation
    pinkSeed_ ^= pinkSeed_ << 13;
    pinkSeed_ ^= pinkSeed_ >> 17;
    pinkSeed_ ^= pinkSeed_ << 5;
    
    // Convert to [0,1] float
    return static_cast<float>(pinkSeed_) / static_cast<float>(UINT32_MAX);
}

inline float& SpectralPaintNode::getRealBin(float* freqData, int bin) noexcept
{
    if (bin == 0) 
        return freqData[0];  // DC component
    else if (bin == NUM_BINS - 1) 
        return freqData[1];  // Nyquist frequency
    else 
        return freqData[bin * 2];  // Regular bins: real part
}

inline float& SpectralPaintNode::getImagBin(float* freqData, int bin) noexcept
{
    if (bin == 0 || bin == NUM_BINS - 1)
    {
        // DC and Nyquist have no imaginary component in real-only FFT
        // Return reference to a buffer location we can safely ignore
        // For JUCE real-only: DC is at [0], Nyquist at [1], so we use a safe dummy location
        static thread_local float dummy = 0.0f;
        dummy = 0.0f;  // Reset to ensure it stays zero
        return dummy;
    }
    else 
    {
        return freqData[bin * 2 + 1];  // Regular bins: imaginary part
    }
}

} // namespace nodes