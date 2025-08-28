#include "SpectralEngine_Fixed.h"
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SpectralEngine::SpectralEngine() noexcept
    : initialized_(false)
    , samplesUntilNextFrame_(HOP_SIZE)
    , sampleRate_(44100.0)
    , spectralGain_(1.0f)
    , spectralMix_(1.0f)
    , sequenceNumber_(0)
    , currentTimeInSamples_(0.0)
    , spectralCentroid_(0.0f)
    , fundamentalFreq_(0.0f)
{
    // Initialize buffers
    fftBuffer_.fill(0.0f);
    ifftBuffer_.fill(0.0f);
    windowedBuffer_.fill(0.0f);
    overlapBuffer_.fill(0.0f);
    currentMagnitude_.fill(0.0f);
    currentPhase_.fill(0.0f);
    previousPhase_.fill(0.0f);
    inputBuffer_.fill(0.0f);
    outputBuffer_.fill(0.0f);
    
    // Generate Hann window
    for (size_t i = 0; i < FFT_SIZE; ++i)
    {
        hannWindow_[i] = 0.5f * (1.0f - std::cos(2.0f * M_PI * i / (FFT_SIZE - 1)));
    }
}

SpectralEngine::~SpectralEngine() noexcept = default;

void SpectralEngine::initialize(double sampleRate, int maxBlockSize)
{
    juce::ignoreUnused(maxBlockSize);
    
    sampleRate_ = sampleRate;
    
    // Initialize FFT engines (allocation allowed here)
    const int fftOrder = (int)std::log2(FFT_SIZE);
    forwardFFT_ = std::make_unique<juce::dsp::FFT>(fftOrder);
    inverseFFT_ = std::make_unique<juce::dsp::FFT>(fftOrder);
    
    // Reset state
    samplesUntilNextFrame_.store(HOP_SIZE, std::memory_order_relaxed);
    currentTimeInSamples_.store(0.0, std::memory_order_relaxed);
    sequenceNumber_.store(0, std::memory_order_relaxed);
    
    initialized_.store(true, std::memory_order_release);
}

void SpectralEngine::reset() noexcept
{
    // Clear buffers (RT-safe)
    fftBuffer_.fill(0.0f);
    ifftBuffer_.fill(0.0f);
    windowedBuffer_.fill(0.0f);
    overlapBuffer_.fill(0.0f);
    currentMagnitude_.fill(0.0f);
    currentPhase_.fill(0.0f);
    previousPhase_.fill(0.0f);
    inputBuffer_.fill(0.0f);
    outputBuffer_.fill(0.0f);
    
    // Reset positions
    inputWritePos_.store(0, std::memory_order_relaxed);
    inputReadPos_.store(0, std::memory_order_relaxed);
    outputWritePos_.store(0, std::memory_order_relaxed);
    outputReadPos_.store(0, std::memory_order_relaxed);
    
    // Reset timing
    samplesUntilNextFrame_.store(HOP_SIZE, std::memory_order_relaxed);
    currentTimeInSamples_.store(0.0, std::memory_order_relaxed);
    sequenceNumber_.store(0, std::memory_order_relaxed);
}

void SpectralEngine::processBlock(const float* inputBuffer, float* outputBuffer, int numSamples) noexcept
{
    if (!initialized_.load(std::memory_order_acquire))
    {
        // Pass through unchanged if not initialized
        std::memcpy(outputBuffer, inputBuffer, numSamples * sizeof(float));
        return;
    }
    
    // Simple pass-through for now with basic spectral analysis
    // This is a minimal implementation to get compilation working
    
    // Copy input to output (pass-through)
    std::memcpy(outputBuffer, inputBuffer, numSamples * sizeof(float));
    
    // Store input data for spectral analysis
    size_t writePos = inputWritePos_.load(std::memory_order_relaxed);
    for (int i = 0; i < numSamples; ++i)
    {
        inputBuffer_[writePos] = inputBuffer[i];
        writePos = (writePos + 1) % BUFFER_SIZE;
    }
    inputWritePos_.store(writePos, std::memory_order_relaxed);
    
    // Update timing
    double currentTime = currentTimeInSamples_.load(std::memory_order_relaxed);
    currentTimeInSamples_.store(currentTime + numSamples, std::memory_order_relaxed);
    
    // Trigger spectral analysis every HOP_SIZE samples
    int samplesLeft = samplesUntilNextFrame_.load(std::memory_order_relaxed) - numSamples;
    if (samplesLeft <= 0)
    {
        performSTFT();
        updateSpectralFeatures();
        samplesUntilNextFrame_.store(HOP_SIZE + samplesLeft, std::memory_order_relaxed);
    }
    else
    {
        samplesUntilNextFrame_.store(samplesLeft, std::memory_order_relaxed);
    }
}

void SpectralEngine::performSTFT() noexcept
{
    if (!forwardFFT_) return;
    
    // Read FFT_SIZE samples from circular buffer
    size_t readPos = inputReadPos_.load(std::memory_order_relaxed);
    size_t writePos = inputWritePos_.load(std::memory_order_relaxed);
    
    // Check if we have enough samples
    size_t available = (writePos >= readPos) ? 
        (writePos - readPos) : (BUFFER_SIZE - readPos + writePos);
    
    if (available < FFT_SIZE) return;
    
    // Copy data and apply window
    for (size_t i = 0; i < FFT_SIZE; ++i)
    {
        size_t pos = (readPos + i) % BUFFER_SIZE;
        windowedBuffer_[i] = inputBuffer_[pos] * hannWindow_[i];
    }
    
    // Advance read position by HOP_SIZE
    inputReadPos_.store((readPos + HOP_SIZE) % BUFFER_SIZE, std::memory_order_relaxed);
    
    // Perform forward FFT (in-place)
    std::copy(windowedBuffer_.begin(), windowedBuffer_.end(), fftBuffer_.begin());
    forwardFFT_->performRealOnlyForwardTransform(fftBuffer_.data(), true);
    
    // Extract magnitude and phase
    for (size_t bin = 0; bin < NUM_BINS; ++bin)
    {
        const size_t realIndex = bin * 2;
        const size_t imagIndex = bin * 2 + 1;
        
        const float real = (bin == 0 || bin == NUM_BINS - 1) ? 
                          fftBuffer_[bin] : fftBuffer_[realIndex];
        const float imag = (bin == 0 || bin == NUM_BINS - 1) ? 
                          0.0f : fftBuffer_[imagIndex];
        
        currentMagnitude_[bin] = std::sqrt(real * real + imag * imag);
        currentPhase_[bin] = std::atan2(imag, real);
    }
}

void SpectralEngine::updateSpectralFeatures() noexcept
{
    // Calculate spectral centroid
    float weightedSum = 0.0f;
    float magnitudeSum = 0.0f;
    
    const float binWidth = static_cast<float>(sampleRate_) / FFT_SIZE;
    
    for (size_t bin = 1; bin < NUM_BINS - 1; ++bin) // Skip DC and Nyquist
    {
        const float magnitude = currentMagnitude_[bin];
        const float frequency = bin * binWidth;
        
        weightedSum += frequency * magnitude;
        magnitudeSum += magnitude;
    }
    
    const float centroid = (magnitudeSum > 0.0f) ? weightedSum / magnitudeSum : 0.0f;
    spectralCentroid_.store(centroid, std::memory_order_relaxed);
    
    // Simple fundamental frequency estimation
    float maxMagnitude = 0.0f;
    size_t peakBin = 0;
    
    const size_t maxBin = static_cast<size_t>(std::min(2000.0f / binWidth, static_cast<float>(NUM_BINS - 1)));
    
    for (size_t bin = 1; bin < maxBin; ++bin)
    {
        if (currentMagnitude_[bin] > maxMagnitude)
        {
            maxMagnitude = currentMagnitude_[bin];
            peakBin = bin;
        }
    }
    
    const float fundamental = (peakBin > 0) ? peakBin * binWidth : 0.0f;
    fundamentalFreq_.store(fundamental, std::memory_order_relaxed);
}

void SpectralEngine::performiFFT() noexcept
{
    // Stub for now - not needed for basic visualization
}

void SpectralEngine::applySpectralProcessing() noexcept
{
    const float gain = spectralGain_.load(std::memory_order_relaxed);
    
    // Apply gain to magnitude spectrum
    if (gain != 1.0f)
    {
        for (size_t bin = 0; bin < NUM_BINS; ++bin)
        {
            currentMagnitude_[bin] *= gain;
        }
    }
}

bool SpectralEngine::extractSpectralFrame(SpectralFrame& frame) noexcept
{
    if (!initialized_.load(std::memory_order_acquire))
        return false;
    
    // Copy current spectral data
    std::copy(currentMagnitude_.begin(), currentMagnitude_.end(), frame.magnitude);
    std::copy(currentPhase_.begin(), currentPhase_.end(), frame.phase);
    
    // Set frame metadata
    frame.sequenceNumber = sequenceNumber_.fetch_add(1, std::memory_order_relaxed);
    frame.timestampSamples = currentTimeInSamples_.load(std::memory_order_relaxed);
    frame.fundamentalFreq = fundamentalFreq_.load(std::memory_order_relaxed);
    frame.spectralCentroid = spectralCentroid_.load(std::memory_order_relaxed);
    
    return true;
}