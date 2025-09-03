#include "OfflineStftAnalyzer.h"
#include <juce_audio_formats/juce_audio_formats.h>
#include <algorithm>
#include <cmath>

OfflineStftAnalyzer::OfflineStftAnalyzer() noexcept
    : shouldStop_(false)
    , analyzing_(false)
    , paused_(false)
    , fileLoaded_(false)
    , totalSamples_(0)
    , fileSampleRate_(44100.0)
    , currentSamplePosition_(0)
    , playbackSyncPosition_(0)
    , analysisSpeed_(1.0f)
    , columnStreamingRate_(60) // 60 columns per second default
    , sequenceCounter_(0)
    , windowMethod_(juce::dsp::WindowingFunction<float>::hann)
    , overlapFactor_(0.75f) // 75% overlap default
    , outputQueue_(nullptr)
    , lastColumnTimestamp_(0)
    , sampleRate_(44100.0)
    , samplesPerHop_(HOP_SIZE)
{
    currentAtlasPosition_.store(AtlasPosition{}, std::memory_order_release);
}

OfflineStftAnalyzer::~OfflineStftAnalyzer() noexcept {
    shutdown();
}

bool OfflineStftAnalyzer::initialize(double sampleRate, MaskDeltaQueue& outputQueue) noexcept {
    std::lock_guard<std::mutex> lock(stateMutex_);
    
    sampleRate_ = sampleRate;
    outputQueue_ = &outputQueue;
    
    // Calculate hop size based on overlap factor
    const float overlap = overlapFactor_.load(std::memory_order_acquire);
    samplesPerHop_ = static_cast<size_t>(FFT_SIZE * (1.0f - overlap));
    
    // Initialize FFT
    const int fftOrder = static_cast<int>(std::log2(FFT_SIZE));
    fft_ = std::make_unique<juce::dsp::FFT>(fftOrder);
    
    if (!fft_) return false;
    
    // Initialize windowing function
    const auto method = windowMethod_.load(std::memory_order_acquire);
    window_ = std::make_unique<juce::dsp::WindowingFunction<float>>(FFT_SIZE, method);
    
    // Allocate processing buffers
    frameBuffer_.resize(FFT_SIZE);
    windowedBuffer_.resize(FFT_SIZE);
    fftBuffer_.resize(FFT_SIZE * 2); // Complex FFT buffer
    magnitudeBuffer_.resize(NUM_BINS);
    phaseBuffer_.resize(NUM_BINS);
    
    return true;
}

void OfflineStftAnalyzer::shutdown() noexcept {
    stopAnalysis();
    
    if (analysisThread_ && analysisThread_->joinable()) {
        analysisThread_->join();
        analysisThread_.reset();
    }
    
    releaseAudioData();
    outputQueue_ = nullptr;
}

bool OfflineStftAnalyzer::loadAudioFile(const juce::File& audioFile) noexcept {
    std::lock_guard<std::mutex> lock(stateMutex_);
    
    stopAnalysis();
    
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    auto reader = std::unique_ptr<juce::AudioFormatReader>(formatManager.createReaderFor(audioFile));
    if (!reader) return false;
    
    const auto numSamples = reader->lengthInSamples;
    const auto numChannels = reader->numChannels;
    
    if (numSamples == 0 || numChannels == 0) return false;
    
    // Load audio data (mix to mono)
    audioBuffer_.setSize(1, static_cast<int>(numSamples));
    
    if (numChannels == 1) {
        reader->read(&audioBuffer_, 0, static_cast<int>(numSamples), 0, true, false);
    } else {
        // Mix multiple channels to mono
        juce::AudioBuffer<float> tempBuffer(static_cast<int>(numChannels), static_cast<int>(numSamples));
        reader->read(&tempBuffer, 0, static_cast<int>(numSamples), 0, true, true);
        
        auto* monoData = audioBuffer_.getWritePointer(0);
        for (int sample = 0; sample < static_cast<int>(numSamples); ++sample) {
            float sum = 0.0f;
            for (int ch = 0; ch < static_cast<int>(numChannels); ++ch) {
                sum += tempBuffer.getSample(ch, sample);
            }
            monoData[sample] = sum / static_cast<float>(numChannels);
        }
    }
    
    totalSamples_.store(numSamples, std::memory_order_release);
    fileSampleRate_.store(reader->sampleRate, std::memory_order_release);
    currentSamplePosition_.store(0, std::memory_order_release);
    fileLoaded_.store(true, std::memory_order_release);
    
    return true;
}

void OfflineStftAnalyzer::startAnalysis() noexcept {
    if (analyzing_.load(std::memory_order_acquire)) return;
    if (!fileLoaded_.load(std::memory_order_acquire)) return;
    
    shouldStop_.store(false, std::memory_order_release);
    paused_.store(false, std::memory_order_release);
    
    analysisThread_ = std::make_unique<std::thread>(&OfflineStftAnalyzer::analysisThreadMain, this);
}

void OfflineStftAnalyzer::stopAnalysis() noexcept {
    shouldStop_.store(true, std::memory_order_release);
    analyzing_.store(false, std::memory_order_release);
    
    if (analysisThread_ && analysisThread_->joinable()) {
        analysisThread_->join();
        analysisThread_.reset();
    }
}

void OfflineStftAnalyzer::pauseAnalysis() noexcept {
    paused_.store(true, std::memory_order_release);
}

void OfflineStftAnalyzer::resumeAnalysis() noexcept {
    paused_.store(false, std::memory_order_release);
}

void OfflineStftAnalyzer::analysisThreadMain() noexcept {
    analyzing_.store(true, std::memory_order_release);
    
    const auto totalSamples = totalSamples_.load(std::memory_order_acquire);
    const auto sampleRate = fileSampleRate_.load(std::memory_order_acquire);
    const auto streamingRate = columnStreamingRate_.load(std::memory_order_acquire);
    const auto microsBetweenColumns = 1000000 / streamingRate; // Convert to microseconds
    
    auto* audioData = audioBuffer_.getReadPointer(0);
    uint64_t samplePos = currentSamplePosition_.load(std::memory_order_acquire);
    uint64_t lastStreamTime = TimeUtils::getCurrentTimeUs();
    
    auto atlasPos = currentAtlasPosition_.load(std::memory_order_acquire);
    uint32_t sequence = sequenceCounter_.load(std::memory_order_acquire);
    
    while (!shouldStop_.load(std::memory_order_acquire) && samplePos + FFT_SIZE < totalSamples) {
        // Check pause state
        if (paused_.load(std::memory_order_acquire)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        
        // Rate limiting for smooth streaming
        const auto currentTime = TimeUtils::getCurrentTimeUs();
        if (currentTime - lastStreamTime < microsBetweenColumns) {
            std::this_thread::sleep_for(std::chrono::microseconds(microsBetweenColumns / 4));
            continue;
        }
        
        // Extract frame and apply windowing
        const size_t framesToCopy = std::min(FFT_SIZE, totalSamples - samplePos);
        std::copy(audioData + samplePos, audioData + samplePos + framesToCopy, frameBuffer_.begin());
        
        // Zero-pad if necessary
        if (framesToCopy < FFT_SIZE) {
            std::fill(frameBuffer_.begin() + framesToCopy, frameBuffer_.end(), 0.0f);
        }
        
        // Apply window and process
        std::copy(frameBuffer_.begin(), frameBuffer_.end(), windowedBuffer_.begin());
        if (window_) {
            window_->multiplyWithWindowingTable(windowedBuffer_.data(), FFT_SIZE);
        }
        
        processFrame(windowedBuffer_.data(), FFT_SIZE);
        
        // Generate and send mask column delta
        MaskColumnDelta delta;
        delta.position = atlasPos;
        delta.metadata.samplePosition = samplePos;
        delta.metadata.timeSeconds = static_cast<float>(samplePos) / static_cast<float>(sampleRate);
        delta.metadata.sequenceNumber = sequence++;
        
        computeSpectralFeatures(magnitudeBuffer_.data(), delta.metadata);
        generateMaskColumn(magnitudeBuffer_.data(), phaseBuffer_.data(), delta);
        
        // Send to output queue (non-blocking)
        if (outputQueue_ && !outputQueue_->push(delta)) {
            // Queue full - could log warning or implement backpressure
        }
        
        // Update position
        samplePos += samplesPerHop_;
        atlasPos.columnInTile++;
        if (atlasPos.columnInTile >= AtlasConfig::TILE_WIDTH) {
            atlasPos.columnInTile = 0;
            atlasPos.tileX = (atlasPos.tileX + 1) % AtlasConfig::NUM_TILES_X;
        }
        
        // Update state
        currentSamplePosition_.store(samplePos, std::memory_order_release);
        currentAtlasPosition_.store(atlasPos, std::memory_order_release);
        sequenceCounter_.store(sequence, std::memory_order_release);
        lastStreamTime = currentTime;
        
        // Respect analysis speed setting
        const auto speed = analysisSpeed_.load(std::memory_order_acquire);
        if (speed < 1.0f) {
            const auto delayUs = static_cast<uint64_t>((1.0f / speed - 1.0f) * microsBetweenColumns);
            if (delayUs > 0) {
                std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
            }
        }
    }
    
    analyzing_.store(false, std::memory_order_release);
}

void OfflineStftAnalyzer::processFrame(const float* inputSamples, size_t numSamples) noexcept {
    jassert(numSamples == FFT_SIZE);
    
    // Copy to complex buffer (real part only)
    for (size_t i = 0; i < FFT_SIZE; ++i) {
        fftBuffer_[i * 2] = inputSamples[i];     // Real
        fftBuffer_[i * 2 + 1] = 0.0f;            // Imaginary
    }
    
    // Perform FFT
    fft_->performRealOnlyForwardTransform(fftBuffer_.data());
    
    // Extract magnitude and phase
    for (size_t bin = 0; bin < NUM_BINS; ++bin) {
        const float real = fftBuffer_[bin * 2];
        const float imag = fftBuffer_[bin * 2 + 1];
        
        magnitudeBuffer_[bin] = std::sqrt(real * real + imag * imag);
        phaseBuffer_[bin] = std::atan2(imag, real);
    }
}

void OfflineStftAnalyzer::computeSpectralFeatures(const float* magnitude, ColumnMetadata& metadata) noexcept {
    // Compute spectral centroid
    float weightedSum = 0.0f;
    float magnitudeSum = 0.0f;
    
    const auto sampleRate = fileSampleRate_.load(std::memory_order_acquire);
    const float binToHz = static_cast<float>(sampleRate) / FFT_SIZE;
    
    for (size_t bin = 1; bin < NUM_BINS; ++bin) { // Skip DC
        const float freq = bin * binToHz;
        const float mag = magnitude[bin];
        weightedSum += freq * mag;
        magnitudeSum += mag;
    }
    
    metadata.spectralCentroid = (magnitudeSum > 0.0f) ? (weightedSum / magnitudeSum) : 0.0f;
    
    // Simple fundamental frequency estimation (peak picking)
    size_t peakBin = 1;
    float peakMag = magnitude[1];
    
    for (size_t bin = 2; bin < NUM_BINS / 4; ++bin) { // Search lower frequencies
        if (magnitude[bin] > peakMag) {
            peakMag = magnitude[bin];
            peakBin = bin;
        }
    }
    
    metadata.fundamentalHz = peakBin * binToHz;
}

void OfflineStftAnalyzer::generateMaskColumn(const float* magnitude, const float* phase, MaskColumnDelta& delta) noexcept {
    juce::ignoreUnused(phase); // Phase not used in current mask generation
    
    // Normalize magnitude to [0,1] range for mask values
    float maxMag = 0.0f;
    for (size_t bin = 0; bin < NUM_BINS; ++bin) {
        maxMag = std::max(maxMag, magnitude[bin]);
    }
    
    if (maxMag > 0.0f) {
        const float invMaxMag = 1.0f / maxMag;
        for (size_t bin = 0; bin < NUM_BINS; ++bin) {
            delta.values[bin] = magnitude[bin] * invMaxMag;
        }
    } else {
        // Silence - zero mask
        std::fill(std::begin(delta.values), std::begin(delta.values) + NUM_BINS, 0.0f);
    }
}

void OfflineStftAnalyzer::setWindowFunction(juce::dsp::WindowingFunction<float>::WindowingMethod method) noexcept {
    windowMethod_.store(method, std::memory_order_release);
    
    // Update window if not currently analyzing
    if (!analyzing_.load(std::memory_order_acquire)) {
        window_ = std::make_unique<juce::dsp::WindowingFunction<float>>(FFT_SIZE, method);
    }
}

void OfflineStftAnalyzer::setOverlapFactor(float factor) noexcept {
    const float clampedFactor = juce::jlimit(0.0f, 0.9f, factor);
    overlapFactor_.store(clampedFactor, std::memory_order_release);
    
    // Update hop size
    samplesPerHop_ = static_cast<size_t>(FFT_SIZE * (1.0f - clampedFactor));
}

void OfflineStftAnalyzer::setAtlasPosition(const AtlasPosition& startPosition) noexcept {
    currentAtlasPosition_.store(startPosition, std::memory_order_release);
}

void OfflineStftAnalyzer::releaseAudioData() noexcept {
    audioBuffer_.setSize(0, 0);
    fileLoaded_.store(false, std::memory_order_release);
    totalSamples_.store(0, std::memory_order_release);
    currentSamplePosition_.store(0, std::memory_order_release);
}