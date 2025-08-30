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
    
    // Reset carrier phase
    carrierPhase_ = 0.0f;
    
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
        const int clampedHop = std::max(0, currentHop);  // FIX: Prevent negative hop
        const int samplesToProcess = std::min(samplesRemaining, clampedHop);
        
        // Read output samples from circular buffer
        if (samplesToProcess > 0)
        {
            outputBuffer_.readBlock(outputBuffer + outputIndex, samplesToProcess);
            samplesRemaining -= samplesToProcess;
            outputIndex += samplesToProcess;
            samplesUntilNextFrame_.store(currentHop - samplesToProcess, std::memory_order_relaxed);
        }
        
        // Process new frame when hop is complete
        if (samplesUntilNextFrame_.load(std::memory_order_relaxed) <= 0)
        {
            if (inputBuffer_.getAvailableForReading() >= HOP_SIZE)  // FIX: Check for HOP_SIZE, not FFT_SIZE
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
                // Avoid negative hop in next iteration
                samplesUntilNextFrame_.store(0, std::memory_order_relaxed);
                break;
            }
        }
    }
}

void SpectralEngine::performSTFT() noexcept
{
    // FIX: Maintain overlapping frame - shift left by HOP_SIZE
    std::memmove(fftBuffer_.get().data(),
                 fftBuffer_.get().data() + HOP_SIZE,
                 (FFT_SIZE - HOP_SIZE) * sizeof(float));
    
    // Append HOP_SIZE fresh samples from input ring
    inputBuffer_.readBlock(fftBuffer_.get().data() + (FFT_SIZE - HOP_SIZE), HOP_SIZE);
    
    // Copy to windowed buffer and apply window
    std::memcpy(windowedBuffer_.get().data(), fftBuffer_.get().data(), FFT_SIZE * sizeof(float));
    hannWindow_.apply(windowedBuffer_.get().data());
    
    // Perform forward FFT (in-place)
    forwardFFT_->performRealOnlyForwardTransform(windowedBuffer_.get().data(), true);
    
    // FIX: Extract magnitude and phase with correct JUCE real-only FFT packing
    for (size_t bin = 0; bin < NUM_BINS; ++bin)
    {
        float real = 0.0f, imag = 0.0f;

        if (bin == 0)                   // DC component
        {
            real = windowedBuffer_.get()[0];
            imag = 0.0f;
        }
        else if (bin == NUM_BINS - 1)   // Nyquist component
        {
            real = windowedBuffer_.get()[1];
            imag = 0.0f;
        }
        else                            // Regular bins
        {
            const size_t idx = bin * 2;
            real = windowedBuffer_.get()[idx];
            imag = windowedBuffer_.get()[idx + 1];
        }

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
    
    // NEW: Generative synthesis path for paint-to-audio
    const float* mask = currentMask_.load(std::memory_order_acquire);
    
    // Compute input energy to detect silence
    float inputEnergy = 0.0f;
    for (size_t bin = 0; bin < NUM_BINS; ++bin)
        inputEnergy += currentMagnitude_.get()[bin] * currentMagnitude_.get()[bin];
    
    if (mask != nullptr)
    {
        float maskSum = 0.0f;
        for (size_t bin = 0; bin < NUM_BINS; ++bin)
            maskSum += mask[bin];

        const bool inputSilent = (inputEnergy < 1e-10f);
        const float mix = std::clamp(spectralMix_.load(std::memory_order_relaxed), 0.0f, 1.0f);

        // Thread-local phase accumulator for coherent synthesis
        static thread_local std::array<float, NUM_BINS> phaseAcc = { {} };

        if (inputSilent && maskSum > 0.0f)
        {
            // Synthesize magnitudes from mask with phase continuity
            const float excitation = 0.02f; // Base level for audibility
            for (size_t bin = 1; bin < NUM_BINS - 1; ++bin)
            {
                const float targetMag = excitation * mask[bin];
                // Crossfade with existing spectrum (should be ~0 when silent)
                currentMagnitude_.get()[bin] = (1.0f - mix) * currentMagnitude_.get()[bin] + mix * targetMag;

                // Bin-centered phase increment for coherent partials
                phaseAcc[bin] += 2.0f * float(M_PI) * float(HOP_SIZE) * float(bin) / float(FFT_SIZE);
                if (phaseAcc[bin] > float(M_PI))  phaseAcc[bin] -= 2.0f * float(M_PI);
                if (phaseAcc[bin] < -float(M_PI)) phaseAcc[bin] += 2.0f * float(M_PI);
                currentPhase_.get()[bin] = phaseAcc[bin];
            }
        }
        else
        {
            // Gate/shape existing spectrum by mask (with small floor)
            const float maskFloor = 0.02f;
            for (size_t bin = 0; bin < NUM_BINS; ++bin)
            {
                const float scale = maskFloor + (1.0f - maskFloor) * mask[bin];
                currentMagnitude_.get()[bin] *= scale;
            }
        }
    }
}

void SpectralEngine::performiFFT() noexcept
{
    // FIX: Reconstruct complex spectrum with correct JUCE real-only FFT packing
    for (size_t bin = 0; bin < NUM_BINS; ++bin)
    {
        const float magnitude = currentMagnitude_.get()[bin];
        const float phase = currentPhase_.get()[bin];
        
        const float real = magnitude * std::cos(phase);
        const float imag = magnitude * std::sin(phase);

        if (bin == 0)                   // DC component
        {
            ifftBuffer_.get()[0] = real;
        }
        else if (bin == NUM_BINS - 1)   // Nyquist component
        {
            ifftBuffer_.get()[1] = real;
        }
        else                            // Regular bins
        {
            const size_t idx = bin * 2;
            ifftBuffer_.get()[idx]     = real;
            ifftBuffer_.get()[idx + 1] = imag;
        }
    }
    
    // Perform inverse FFT
    inverseFFT_->performRealOnlyInverseTransform(ifftBuffer_.get().data());
    
    // FIX: Add normalization to prevent amplitude buildup
    const float invN = 1.0f / static_cast<float>(FFT_SIZE);
    for (size_t i = 0; i < FFT_SIZE; ++i)
        ifftBuffer_.get()[i] *= invN;
    
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

void SpectralEngine::applyMaskColumn(const MaskColumn& maskColumn) noexcept
{
    // Validate bin count alignment
    const size_t maskBins = std::min(static_cast<size_t>(maskColumn.numBins), NUM_BINS);
    
    // Apply mask directly to current magnitude spectrum
    auto& magnitude = currentMagnitude_.get();
    
    // Check for silence - if magnitude is too low, inject carrier
    bool needsCarrier = true;
    if (carrierEnabled_.load(std::memory_order_relaxed))
    {
        float totalEnergy = 0.0f;
        for (size_t bin = 0; bin < NUM_BINS; ++bin)
        {
            totalEnergy += magnitude[bin] * magnitude[bin];
        }
        
        // If we have sufficient energy, no need for carrier
        if (totalEnergy > 1e-8f) // -80dB threshold
        {
            needsCarrier = false;
        }
    }
    
    // Inject carrier sine wave if needed (RT-safe static phase accumulator)
    if (needsCarrier)
    {
        const float sampleRate = static_cast<float>(sampleRate_.load(std::memory_order_relaxed));
        const float carrierBin = (CARRIER_FREQ / sampleRate) * FFT_SIZE;
        const size_t bin = static_cast<size_t>(carrierBin);
        
        if (bin < NUM_BINS)
        {
            magnitude[bin] += CARRIER_AMP;
        }
        
        // Update phase (will wrap naturally)
        carrierPhase_ += 2.0f * M_PI * CARRIER_FREQ / sampleRate;
    }
    
    // Apply mask values to magnitude spectrum
    for (size_t bin = 0; bin < maskBins; ++bin)
    {
        const float maskValue = maskColumn.values[bin];
        
        // Clamp mask to reasonable range (0.1x to 10x)
        const float clampedMask = std::clamp(maskValue, 0.1f, 10.0f);
        
        // Apply mask scaling to magnitude with boost for audibility
        magnitude[bin] *= clampedMask * 2.0f;
    }
}