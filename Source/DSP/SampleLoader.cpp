#include "SampleLoader.h"
#include <juce_audio_formats/juce_audio_formats.h>
#include <algorithm>
#include <cmath>

SampleLoader::SampleLoader() noexcept
    : sampleLoaded_(false)
    , numSpectralFrames_(0)
    , originalDuration_(0.0)
    , playbackSpeed_(1.0f)
    , hannWindow_(RTWindow<FFT_SIZE>::Hann)
{
    RT_SAFE_LOG("SampleLoader constructor called");
}

SampleLoader::~SampleLoader() noexcept
{
    RT_SAFE_LOG("SampleLoader destructor called");
    shutdown();
}

void SampleLoader::initialize(double sampleRate)
{
    RT_SAFE_LOG("Initializing SampleLoader with sample rate: " + juce::String(sampleRate));
    
    targetSampleRate_ = sampleRate;
    
    // Initialize FFT engines (allocation allowed here)
    const int fftOrder = (int)std::log2(FFT_SIZE);
    forwardFFT_ = std::make_unique<juce::dsp::FFT>(fftOrder);
    
    // Pre-allocate spectral frame storage
    spectralFrames_ = std::make_unique<SpectralFrameData[]>(MAX_SAMPLE_FRAMES);
    
    RT_SAFE_LOG("SampleLoader initialization complete");
}

void SampleLoader::shutdown() noexcept
{
    RT_SAFE_LOG("Shutting down SampleLoader");
    
    sampleLoaded_.store(false, std::memory_order_release);
    numSpectralFrames_.store(0, std::memory_order_release);
    originalDuration_.store(0.0, std::memory_order_release);
    
    // Clear sample info
    {
        std::lock_guard<std::mutex> lock(sampleInfoMutex_);
        sampleInfo_ = SampleInfo{};
    }
}

bool SampleLoader::loadSampleFile(const juce::File& audioFile)
{
    if (!audioFile.existsAsFile())
    {
        juce::Logger::writeToLog("SampleLoader: File does not exist - " + audioFile.getFullPathName());
        return false;
    }
    
    juce::Logger::writeToLog("SampleLoader: Loading file - " + audioFile.getFullPathName());
    
    // Create audio format manager
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    // Create reader for the file
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(audioFile));
    if (!reader)
    {
        juce::Logger::writeToLog("SampleLoader: Could not create reader for file");
        return false;
    }
    
    // Read the entire file into buffer
    const auto numSamples = static_cast<int>(reader->lengthInSamples);
    const auto numChannels = static_cast<int>(reader->numChannels);
    const auto sourceSampleRate = reader->sampleRate;
    
    if (numSamples <= 0 || numChannels <= 0)
    {
        juce::Logger::writeToLog("SampleLoader: Invalid sample data");
        return false;
    }
    
    juce::AudioBuffer<float> audioBuffer(numChannels, numSamples);
    reader->read(&audioBuffer, 0, numSamples, 0, true, true);
    
    // Store sample info for UI
    {
        std::lock_guard<std::mutex> lock(sampleInfoMutex_);
        sampleInfo_.filename = audioFile.getFileNameWithoutExtension();
        sampleInfo_.duration = numSamples / sourceSampleRate;
        sampleInfo_.sampleRate = sourceSampleRate;
        sampleInfo_.numChannels = numChannels;
        sampleInfo_.isLoaded = false; // Will be set to true after preprocessing
    }
    
    // Preprocess the sample
    preprocessSample(audioBuffer, sourceSampleRate);
    
    juce::Logger::writeToLog("SampleLoader: Successfully loaded " + juce::String(numSamples) + " samples");
    return true;
}

bool SampleLoader::loadSampleFromMemory(const float* samples, int numSamples, int numChannels, double sourceSampleRate)
{
    if (!samples || numSamples <= 0 || numChannels <= 0)
    {
        juce::Logger::writeToLog("SampleLoader: Invalid sample data provided");
        return false;
    }
    
    // Copy to audio buffer
    juce::AudioBuffer<float> audioBuffer(numChannels, numSamples);
    for (int ch = 0; ch < numChannels; ++ch)
    {
        audioBuffer.copyFrom(ch, 0, samples + (ch * numSamples), numSamples);
    }
    
    // Store sample info for UI
    {
        std::lock_guard<std::mutex> lock(sampleInfoMutex_);
        sampleInfo_.filename = "Memory Sample";
        sampleInfo_.duration = numSamples / sourceSampleRate;
        sampleInfo_.sampleRate = sourceSampleRate;
        sampleInfo_.numChannels = numChannels;
        sampleInfo_.isLoaded = false;
    }
    
    // Preprocess the sample
    preprocessSample(audioBuffer, sourceSampleRate);
    
    juce::Logger::writeToLog("SampleLoader: Successfully loaded from memory");
    return true;
}

void SampleLoader::preprocessSample(const juce::AudioBuffer<float>& audioBuffer, double sourceSampleRate)
{
    juce::Logger::writeToLog("SampleLoader: Preprocessing sample...");
    
    // Make a working copy
    auto workingBuffer = audioBuffer;
    
    // Resample if needed
    if (std::abs(sourceSampleRate - targetSampleRate_) > 0.1)
    {
        resampleIfNeeded(workingBuffer, sourceSampleRate);
    }
    
    // Mix to mono if multi-channel
    if (workingBuffer.getNumChannels() > 1)
    {
        for (int sample = 0; sample < workingBuffer.getNumSamples(); ++sample)
        {
            float sum = 0.0f;
            for (int ch = 0; ch < workingBuffer.getNumChannels(); ++ch)
            {
                sum += workingBuffer.getSample(ch, sample);
            }
            workingBuffer.setSample(0, sample, sum / workingBuffer.getNumChannels());
        }
        workingBuffer.setSize(1, workingBuffer.getNumSamples(), true);
    }
    
    // Normalize audio
    normalizeAudio(workingBuffer);
    
    // Perform STFT on the preprocessed audio
    const int numSamples = workingBuffer.getNumSamples();
    const float* samples = workingBuffer.getReadPointer(0);
    
    performSTFT(samples, numSamples, 0);
    
    // Update atomic state - this makes the sample available to audio thread
    originalDuration_.store(numSamples / targetSampleRate_, std::memory_order_relaxed);
    sampleLoaded_.store(true, std::memory_order_release);
    
    // Update sample info
    {
        std::lock_guard<std::mutex> lock(sampleInfoMutex_);
        sampleInfo_.isLoaded = true;
        sampleInfo_.numSpectralFrames = numSpectralFrames_.load();
    }
    
    juce::Logger::writeToLog("SampleLoader: Preprocessing complete - " + 
                            juce::String(numSpectralFrames_.load()) + " spectral frames generated");
}

void SampleLoader::performSTFT(const float* samples, int numSamples, size_t startFrame)
{
    if (!forwardFFT_)
        return;
    
    const size_t maxFrames = std::min(MAX_SAMPLE_FRAMES - startFrame, 
                                     static_cast<size_t>((numSamples - FFT_SIZE) / HOP_SIZE + 1));
    
    // Working buffer for FFT
    std::array<float, FFT_SIZE> fftBuffer;
    std::array<float, FFT_SIZE> windowedBuffer;
    
    size_t frameCount = 0;
    
    for (size_t samplePos = 0; samplePos + FFT_SIZE <= static_cast<size_t>(numSamples) && frameCount < maxFrames; 
         samplePos += HOP_SIZE, ++frameCount)
    {
        const size_t frameIndex = startFrame + frameCount;
        
        // Copy samples to working buffer
        std::memcpy(fftBuffer.data(), samples + samplePos, FFT_SIZE * sizeof(float));
        std::memcpy(windowedBuffer.data(), fftBuffer.data(), FFT_SIZE * sizeof(float));
        
        // Apply window
        hannWindow_.apply(windowedBuffer.data());
        
        // Perform FFT
        forwardFFT_->performRealOnlyForwardTransform(windowedBuffer.data(), true);
        
        // Extract magnitude and phase
        auto& magnitude = spectralFrames_[frameIndex].magnitude.get();
        auto& phase = spectralFrames_[frameIndex].phase.get();
        
        for (size_t bin = 0; bin < NUM_BINS; ++bin)
        {
            const size_t realIndex = bin * 2;
            const size_t imagIndex = bin * 2 + 1;
            
            const float real = (bin == 0 || bin == NUM_BINS - 1) ? 
                              windowedBuffer[bin] : windowedBuffer[realIndex];
            const float imag = (bin == 0 || bin == NUM_BINS - 1) ? 
                              0.0f : windowedBuffer[imagIndex];
            
            magnitude[bin] = std::sqrt(real * real + imag * imag);
            phase[bin] = std::atan2(imag, real);
        }
    }
    
    numSpectralFrames_.store(startFrame + frameCount, std::memory_order_release);
}

void SampleLoader::resampleIfNeeded(juce::AudioBuffer<float>& buffer, double sourceSampleRate)
{
    if (std::abs(sourceSampleRate - targetSampleRate_) < 0.1)
        return; // No resampling needed
    
    const double ratio = targetSampleRate_ / sourceSampleRate;
    const int newSize = static_cast<int>(buffer.getNumSamples() * ratio);
    
    juce::AudioBuffer<float> resampledBuffer(buffer.getNumChannels(), newSize);
    
    // Simple linear interpolation resampling
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        const float* input = buffer.getReadPointer(ch);
        float* output = resampledBuffer.getWritePointer(ch);
        
        for (int i = 0; i < newSize; ++i)
        {
            const double sourcePos = i / ratio;
            const int index0 = static_cast<int>(sourcePos);
            const int index1 = std::min(index0 + 1, buffer.getNumSamples() - 1);
            const float fraction = static_cast<float>(sourcePos - index0);
            
            output[i] = input[index0] * (1.0f - fraction) + input[index1] * fraction;
        }
    }
    
    buffer = std::move(resampledBuffer);
}

void SampleLoader::normalizeAudio(juce::AudioBuffer<float>& buffer) noexcept
{
    float maxLevel = 0.0f;
    
    // Find peak level
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        const float channelMax = buffer.getMagnitude(ch, 0, buffer.getNumSamples());
        maxLevel = std::max(maxLevel, channelMax);
    }
    
    // Apply normalization
    if (maxLevel > 0.0001f) // Avoid division by zero
    {
        const float gain = 0.95f / maxLevel; // Leave some headroom
        buffer.applyGain(gain);
    }
}

bool SampleLoader::getSpectralFrame(size_t frameIndex, float* magnitudeOut, float* phaseOut) const noexcept
{
    if (!hasSample() || frameIndex >= numSpectralFrames_.load(std::memory_order_acquire))
        return false;
    
    const auto& frame = spectralFrames_[frameIndex];
    std::memcpy(magnitudeOut, frame.magnitude.get().data(), NUM_BINS * sizeof(float));
    std::memcpy(phaseOut, frame.phase.get().data(), NUM_BINS * sizeof(float));
    
    return true;
}

void SampleLoader::getSpectralFrameInterpolated(double framePosition, float* magnitudeOut, float* phaseOut) const noexcept
{
    if (!hasSample())
    {
        std::memset(magnitudeOut, 0, NUM_BINS * sizeof(float));
        std::memset(phaseOut, 0, NUM_BINS * sizeof(float));
        return;
    }
    
    const size_t numFrames = numSpectralFrames_.load(std::memory_order_acquire);
    if (numFrames == 0)
    {
        std::memset(magnitudeOut, 0, NUM_BINS * sizeof(float));
        std::memset(phaseOut, 0, NUM_BINS * sizeof(float));
        return;
    }
    
    // Clamp and wrap frame position
    const double wrappedPos = framePosition - std::floor(framePosition / numFrames) * numFrames;
    const size_t frame0 = static_cast<size_t>(wrappedPos) % numFrames;
    const size_t frame1 = (frame0 + 1) % numFrames;
    const float fraction = static_cast<float>(wrappedPos - frame0);
    
    // Get the two frames
    const auto& frameData0 = spectralFrames_[frame0];
    const auto& frameData1 = spectralFrames_[frame1];
    
    // Interpolate magnitude and phase
    for (size_t bin = 0; bin < NUM_BINS; ++bin)
    {
        const float mag0 = frameData0.magnitude.get()[bin];
        const float mag1 = frameData1.magnitude.get()[bin];
        magnitudeOut[bin] = mag0 * (1.0f - fraction) + mag1 * fraction;
        
        // Phase interpolation (handle wraparound)
        float phase0 = frameData0.phase.get()[bin];
        float phase1 = frameData1.phase.get()[bin];
        
        // Handle phase wraparound
        if (std::abs(phase1 - phase0) > M_PI)
        {
            if (phase1 > phase0)
                phase0 += 2.0f * M_PI;
            else
                phase1 += 2.0f * M_PI;
        }
        
        phaseOut[bin] = phase0 * (1.0f - fraction) + phase1 * fraction;
    }
}

SampleLoader::SampleInfo SampleLoader::getSampleInfo() const noexcept
{
    std::lock_guard<std::mutex> lock(sampleInfoMutex_);
    return sampleInfo_;
}