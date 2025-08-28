#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "../Core/RealtimeSafeTypes.h"
#include "../Core/MessageBus.h"
#include <memory>
#include <atomic>

/**
 * RT-safe sample loader for Resynth mode
 * Loads audio files and preprocesses them into spectral frames for RT-safe access
 */
class SampleLoader
{
public:
    static constexpr size_t FFT_SIZE = 512;
    static constexpr size_t HOP_SIZE = 128;
    static constexpr size_t NUM_BINS = FFT_SIZE / 2 + 1;
    static constexpr size_t MAX_SAMPLE_FRAMES = 8192; // Max spectral frames per sample
    
    SampleLoader() noexcept;
    ~SampleLoader() noexcept;
    
    // Non-copyable for RT safety (handled by JUCE macro below)
    
    // Initialization (called from UI thread - may allocate)
    void initialize(double sampleRate);
    void shutdown() noexcept;
    
    // Sample loading (UI thread - may allocate and block)
    bool loadSampleFile(const juce::File& audioFile);
    bool loadSampleFromMemory(const float* samples, int numSamples, int numChannels, double sourceSampleRate);
    
    // RT-safe queries (audio thread safe)
    bool hasSample() const noexcept { return sampleLoaded_.load(std::memory_order_acquire); }
    size_t getNumSpectralFrames() const noexcept { return numSpectralFrames_.load(std::memory_order_acquire); }
    double getSampleRate() const noexcept { return targetSampleRate_; }
    double getOriginalDuration() const noexcept { return originalDuration_.load(std::memory_order_acquire); }
    
    // RT-safe spectral frame access (audio thread only)
    bool getSpectralFrame(size_t frameIndex, float* magnitudeOut, float* phaseOut) const noexcept;
    void getSpectralFrameInterpolated(double framePosition, float* magnitudeOut, float* phaseOut) const noexcept;
    
    // Playback control (RT-safe)
    void setPlaybackSpeed(float speed) noexcept { playbackSpeed_.store(speed, std::memory_order_relaxed); }
    float getPlaybackSpeed() const noexcept { return playbackSpeed_.load(std::memory_order_relaxed); }
    
    // Sample info for UI display
    struct SampleInfo
    {
        juce::String filename;
        double duration = 0.0;
        double sampleRate = 0.0;
        int numChannels = 0;
        size_t numSpectralFrames = 0;
        bool isLoaded = false;
    };
    
    SampleInfo getSampleInfo() const noexcept;

private:
    // Pre-allocated spectral frame storage
    struct SpectralFrameData
    {
        AlignedStorage<std::array<float, NUM_BINS>> magnitude;
        AlignedStorage<std::array<float, NUM_BINS>> phase;
        
        SpectralFrameData() noexcept = default;
    };
    
    // Pre-allocated array of spectral frames
    std::unique_ptr<SpectralFrameData[]> spectralFrames_;
    
    // Sample preprocessing
    void preprocessSample(const juce::AudioBuffer<float>& audioBuffer, double sourceSampleRate);
    void performSTFT(const float* samples, int numSamples, size_t startFrame);
    void resampleIfNeeded(juce::AudioBuffer<float>& buffer, double sourceSampleRate);
    void normalizeAudio(juce::AudioBuffer<float>& buffer) noexcept;
    
    // FFT processing (allocation allowed during loading)
    std::unique_ptr<juce::dsp::FFT> forwardFFT_;
    RTWindow<FFT_SIZE> hannWindow_;
    
    // Thread-safe state
    std::atomic<bool> sampleLoaded_;
    std::atomic<size_t> numSpectralFrames_;
    std::atomic<double> originalDuration_;
    std::atomic<float> playbackSpeed_;
    
    // Configuration
    double targetSampleRate_ = 44100.0;
    
    // Sample info for UI
    mutable std::mutex sampleInfoMutex_; // Only for getSampleInfo() - not used in audio thread
    SampleInfo sampleInfo_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleLoader)
};