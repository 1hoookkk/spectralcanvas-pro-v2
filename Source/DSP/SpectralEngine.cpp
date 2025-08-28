#include "SpectralEngine.h"
#include <algorithm>
#include <cmath>

SpectralEngine::SpectralEngine() noexcept
    : hannWindow_(RTWindow<FFT_SIZE>::Hann)
    , initialized_(false)
    , samplesUntilNextFrame_(HOP_SIZE)
    , sampleRate_(44100.0)
    , spectralGain_(1.0f)
    , spectralMix_(1.0f)
    , currentMask_(nullptr)
    , currentTimeInSamples_(0.0)
    , spectralCentroid_(0.0f)
    , fundamentalFreq_(0.0f)
{
    RT_SAFE_LOG("SpectralEngine constructor called");
}

SpectralEngine::~SpectralEngine() noexcept
{
    RT_SAFE_LOG("SpectralEngine destructor called");
}

void SpectralEngine::initialize(double sampleRate, int maxBlockSize)
{
    RT_SAFE_LOG("Initializing SpectralEngine with sample rate: " + juce::String(sampleRate));
    
    sampleRate_ = sampleRate;
    
    // Initialize FFT engines (allocation allowed here)
    const int fftOrder = (int)std::log2(FFT_SIZE);
    forwardFFT_ = std::make_unique<juce::dsp::FFT>(fftOrder);
    inverseFFT_ = std::make_unique<juce::dsp::FFT>(fftOrder);
    
    // Clear all buffers
    fftBuffer_.get().fill(0.0f);
    ifftBuffer_.get().fill(0.0f);
    windowedBuffer_.get().fill(0.0f);
    overlapBuffer_.get().fill(0.0f);
    currentMagnitude_.get().fill(0.0f);
    currentPhase_.get().fill(0.0f);
    previousPhase_.get().fill(0.0f);
    
    // Reset state
    samplesUntilNextFrame_.store(HOP_SIZE, std::memory_order_relaxed);
    currentTimeInSamples_.store(0.0, std::memory_order_relaxed);
    sequenceGenerator_.reset();
    
    initialized_.store(true, std::memory_order_release);
    
    RT_SAFE_LOG("SpectralEngine initialization complete");
}

void SpectralEngine::reset() noexcept
{
    RT_SAFE_LOG("Resetting SpectralEngine");
    
    // Clear buffers (RT-safe)
    fftBuffer_.get().fill(0.0f);
    ifftBuffer_.get().fill(0.0f);
    windowedBuffer_.get().fill(0.0f);
    overlapBuffer_.get().fill(0.0f);
    currentMagnitude_.get().fill(0.0f);
    currentPhase_.get().fill(0.0f);
    previousPhase_.get().fill(0.0f);
    
    // Reset timing
    samplesUntilNextFrame_.store(HOP_SIZE, std::memory_order_relaxed);
    currentTimeInSamples_.store(0.0, std::memory_order_relaxed);
    sequenceGenerator_.reset();
}

void SpectralEngine::processBlock(const float* inputBuffer, float* outputBuffer, int numSamples) noexcept
{
    if (!initialized_.load(std::memory_order_acquire))
    {
        // Pass through unchanged if not initialized
        std::memcpy(outputBuffer, inputBuffer, numSamples * sizeof(float));
        return;
    }
    
    // Write input samples to circular buffer
    inputBuffer_.writeBlock(inputBuffer, numSamples);
    
    // Process frames as they become available
    int samplesRemaining = numSamples;
    int outputIndex = 0;
    
    while (samplesRemaining > 0)
    {
        const int currentHop = samplesUntilNextFrame_.load(std::memory_order_relaxed);
        const int samplesToProcess = std::min(samplesRemaining, currentHop);
        
        // Read output samples from circular buffer
        outputBuffer_.readBlock(outputBuffer + outputIndex, samplesToProcess);
        
        samplesRemaining -= samplesToProcess;
        outputIndex += samplesToProcess;
        
        const int newHop = currentHop - samplesToProcess;
        samplesUntilNextFrame_.store(newHop, std::memory_order_relaxed);
        
        // Process new frame when hop is complete
        if (newHop <= 0)
        {
            if (inputBuffer_.getAvailableForReading() >= FFT_SIZE)
            {
                performSTFT();
                applySpectralProcessing();
                performiFFT();
                
                // Advance to next frame
                samplesUntilNextFrame_.store(HOP_SIZE, std::memory_order_relaxed);
                
                // Update time tracking
                const double currentTime = currentTimeInSamples_.load(std::memory_order_relaxed);
                currentTimeInSamples_.store(currentTime + HOP_SIZE, std::memory_order_relaxed);
            }
            else
            {
                // Not enough input data yet
                break;
            }
        }
    }
}

void SpectralEngine::performSTFT() noexcept
{
    // Read windowed frame from input buffer
    inputBuffer_.readBlock(fftBuffer_.get().data(), FFT_SIZE);
    
    // Copy to windowed buffer and apply window
    std::memcpy(windowedBuffer_.get().data(), fftBuffer_.get().data(), FFT_SIZE * sizeof(float));
    hannWindow_.apply(windowedBuffer_.get().data());
    
    // Perform forward FFT (in-place)
    forwardFFT_->performRealOnlyForwardTransform(windowedBuffer_.get().data(), true);
    
    // Extract magnitude and phase
    for (size_t bin = 0; bin < NUM_BINS; ++bin)
    {
        const size_t realIndex = bin * 2;
        const size_t imagIndex = bin * 2 + 1;
        
        const float real = (bin == 0 || bin == NUM_BINS - 1) ? 
                          windowedBuffer_.get()[bin] : windowedBuffer_.get()[realIndex];
        const float imag = (bin == 0 || bin == NUM_BINS - 1) ? 
                          0.0f : windowedBuffer_.get()[imagIndex];
        
        currentMagnitude_.get()[bin] = std::sqrt(real * real + imag * imag);
        currentPhase_.get()[bin] = std::atan2(imag, real);
    }
    
    // Update spectral features
    updateSpectralFeatures();
}

void SpectralEngine::updateSpectralFeatures() noexcept
{
    // Calculate spectral centroid
    float weightedSum = 0.0f;
    float magnitudeSum = 0.0f;
    
    const float binWidth = static_cast<float>(sampleRate_) / FFT_SIZE;
    
    for (size_t bin = 1; bin < NUM_BINS - 1; ++bin) // Skip DC and Nyquist
    {
        const float magnitude = currentMagnitude_.get()[bin];
        const float frequency = bin * binWidth;
        
        weightedSum += frequency * magnitude;
        magnitudeSum += magnitude;
    }
    
    const float centroid = (magnitudeSum > 0.0f) ? weightedSum / magnitudeSum : 0.0f;
    spectralCentroid_.store(centroid, std::memory_order_relaxed);
    
    // Simple fundamental frequency estimation (find peak in low frequency range)
    float maxMagnitude = 0.0f;
    size_t peakBin = 0;
    
    const size_t maxBin = static_cast<size_t>(std::min(2000.0f / binWidth, static_cast<float>(NUM_BINS - 1)));
    
    for (size_t bin = 1; bin < maxBin; ++bin)
    {
        if (currentMagnitude_.get()[bin] > maxMagnitude)
        {
            maxMagnitude = currentMagnitude_.get()[bin];
            peakBin = bin;
        }
    }
    
    const float fundamental = (peakBin > 0) ? peakBin * binWidth : 0.0f;
    fundamentalFreq_.store(fundamental, std::memory_order_relaxed);
}

void SpectralEngine::applySpectralProcessing() noexcept
{
    const float gain = spectralGain_.load(std::memory_order_relaxed);
    
    // Apply gain to magnitude spectrum
    if (gain != 1.0f)
    {
        for (size_t bin = 0; bin < NUM_BINS; ++bin)
        {
            currentMagnitude_.get()[bin] *= gain;
        }
    }
    
    // Apply spectral mask from paint gestures
    const float* mask = currentMask_.load(std::memory_order_acquire);
    if (mask != nullptr)
    {
        const float maskFloor = 0.02f; // Minimum attenuation to avoid complete silence
        
        for (size_t bin = 0; bin < NUM_BINS; ++bin)
        {
            const float maskValue = mask[bin];
            const float scale = maskFloor + (1.0f - maskFloor) * maskValue;
            currentMagnitude_.get()[bin] *= scale;
        }
    }
}

void SpectralEngine::performiFFT() noexcept
{
    // Reconstruct complex spectrum from magnitude and phase
    for (size_t bin = 0; bin < NUM_BINS; ++bin)
    {
        const float magnitude = currentMagnitude_.get()[bin];
        const float phase = currentPhase_.get()[bin];
        
        const size_t realIndex = bin * 2;
        const size_t imagIndex = bin * 2 + 1;
        
        if (bin == 0 || bin == NUM_BINS - 1)
        {
            ifftBuffer_.get()[bin] = magnitude * std::cos(phase);
        }
        else
        {
            ifftBuffer_.get()[realIndex] = magnitude * std::cos(phase);
            ifftBuffer_.get()[imagIndex] = magnitude * std::sin(phase);
        }
    }
    
    // Perform inverse FFT
    inverseFFT_->performRealOnlyInverseTransform(ifftBuffer_.get().data());
    
    // Apply window and overlap-add
    hannWindow_.apply(ifftBuffer_.get().data());
    
    // Add to overlap buffer
    for (size_t i = 0; i < FFT_SIZE; ++i)
    {
        overlapBuffer_.get()[i] += ifftBuffer_.get()[i];
    }
    
    // Write HOP_SIZE samples to output buffer  
    outputBuffer_.writeBlock(overlapBuffer_.get().data(), HOP_SIZE);
    
    // Shift overlap buffer
    std::memmove(overlapBuffer_.get().data(), 
                 overlapBuffer_.get().data() + HOP_SIZE,
                 (FFT_SIZE - HOP_SIZE) * sizeof(float));
    
    // Clear the end of overlap buffer
    std::memset(overlapBuffer_.get().data() + (FFT_SIZE - HOP_SIZE), 
                0, HOP_SIZE * sizeof(float));
}

bool SpectralEngine::extractSpectralFrame(SpectralFrame& frame) noexcept
{
    if (!initialized_.load(std::memory_order_acquire))
        return false;
    
    // Copy current spectral data
    std::memcpy(frame.magnitude, currentMagnitude_.get().data(), NUM_BINS * sizeof(float));
    std::memcpy(frame.phase, currentPhase_.get().data(), NUM_BINS * sizeof(float));
    
    // Set frame metadata
    frame.sequenceNumber = sequenceGenerator_.getNext();
    frame.timestampSamples = currentTimeInSamples_.load(std::memory_order_relaxed);
    frame.fundamentalFreq = fundamentalFreq_.load(std::memory_order_relaxed);
    frame.spectralCentroid = spectralCentroid_.load(std::memory_order_relaxed);
    
    return true;
}

void SpectralEngine::updateCurrentMask(const MaskColumn* maskColumn) noexcept
{
    if (maskColumn == nullptr || maskColumn->numBins == 0)
    {
        // Clear mask (no masking)
        currentMask_.store(nullptr, std::memory_order_release);
    }
    else
    {
        // Update mask pointer (RT-safe atomic pointer swap)
        currentMask_.store(maskColumn->values, std::memory_order_release);
    }
}

void SpectralEngine::updateCurrentMask(const float* maskPtr, int numBins) noexcept
{
    if (maskPtr == nullptr || numBins <= 0)
    {
        // Clear mask (no masking)
        currentMask_.store(nullptr, std::memory_order_release);
    }
    else
    {
        // Update mask pointer (RT-safe atomic pointer swap)
        // Note: Caller must ensure maskPtr lifetime exceeds usage
        currentMask_.store(maskPtr, std::memory_order_release);
    }
}