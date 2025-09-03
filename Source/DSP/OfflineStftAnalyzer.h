#pragma once
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <thread>
#include <atomic>
#include <memory>
#include "../Core/AtlasIds.h"
#include "../Core/MessageBus.h"
#include "../Core/TimeUtils.h"

/**
 * Progressive offline STFT analyzer for tiled atlas system.
 * Runs in background thread to analyze audio files and stream
 * spectral columns to the tiled atlas via message queues.
 */
class OfflineStftAnalyzer {
public:
    static constexpr size_t FFT_SIZE = 512;
    static constexpr size_t HOP_SIZE = 128;
    static constexpr size_t NUM_BINS = FFT_SIZE / 2 + 1;
    
    OfflineStftAnalyzer() noexcept;
    ~OfflineStftAnalyzer() noexcept;
    
    // Non-copyable
    OfflineStftAnalyzer(const OfflineStftAnalyzer&) = delete;
    OfflineStftAnalyzer& operator=(const OfflineStftAnalyzer&) = delete;
    
    // Initialization
    bool initialize(double sampleRate, MaskDeltaQueue& outputQueue) noexcept;
    void shutdown() noexcept;
    
    // Audio file analysis
    bool loadAudioFile(const juce::File& audioFile) noexcept;
    void startAnalysis() noexcept;
    void stopAnalysis() noexcept;
    void pauseAnalysis() noexcept;
    void resumeAnalysis() noexcept;
    
    // Playback synchronization
    void setPlaybackPosition(uint64_t samplePosition) noexcept;
    void setAnalysisSpeed(float speedMultiplier) noexcept; // 1.0 = real-time, >1.0 = faster
    
    // State queries
    bool isAnalyzing() const noexcept { return analyzing_.load(std::memory_order_acquire); }
    bool isFileLoaded() const noexcept { return fileLoaded_.load(std::memory_order_acquire); }
    uint64_t getCurrentPosition() const noexcept { return currentSamplePosition_.load(std::memory_order_acquire); }
    uint64_t getTotalSamples() const noexcept { return totalSamples_.load(std::memory_order_acquire); }
    float getProgress() const noexcept;
    
    // Analysis parameters
    void setWindowFunction(juce::dsp::WindowingFunction<float>::WindowingMethod method) noexcept;
    void setOverlapFactor(float factor) noexcept; // 0.5 = 50% overlap, 0.75 = 75% overlap
    
    // Column streaming parameters
    void setColumnStreamingRate(int columnsPerSecond) noexcept; // Rate limiting for smooth streaming
    void setAtlasPosition(const AtlasPosition& startPosition) noexcept;

    // UI progressive spectrogram callback (background thread safe)
    using ColumnCallback = std::function<void(int64_t column, const float* mags, size_t numBins)>;
    void setColumnCallback(ColumnCallback cb) noexcept { columnCallback_ = std::move(cb); }
    
private:
    // Analysis thread main loop
    void analysisThreadMain() noexcept;
    
    // STFT processing
    void processFrame(const float* inputSamples, size_t numSamples) noexcept;
    void computeSpectralFeatures(const float* magnitude, ColumnMetadata& metadata) noexcept;
    void generateMaskColumn(const float* magnitude, const float* phase, MaskColumnDelta& delta) noexcept;
    
    // File I/O helpers
    bool loadAudioData() noexcept;
    void releaseAudioData() noexcept;
    
    // Thread management
    std::unique_ptr<std::thread> analysisThread_;
    std::atomic<bool> shouldStop_;
    std::atomic<bool> analyzing_;
    std::atomic<bool> paused_;
    
    // File data
    juce::AudioBuffer<float> audioBuffer_;
    std::atomic<bool> fileLoaded_;
    std::atomic<uint64_t> totalSamples_;
    std::atomic<double> fileSampleRate_;
    
    // Analysis state
    std::atomic<uint64_t> currentSamplePosition_;
    std::atomic<uint64_t> playbackSyncPosition_;
    std::atomic<float> analysisSpeed_;
    std::atomic<int> columnStreamingRate_;
    
    // Atlas positioning
    std::atomic<AtlasPosition> currentAtlasPosition_;
    std::atomic<uint32_t> sequenceCounter_;
    
    // STFT processing
    std::unique_ptr<juce::dsp::FFT> fft_;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> window_;
    std::atomic<juce::dsp::WindowingFunction<float>::WindowingMethod> windowMethod_;
    std::atomic<float> overlapFactor_;
    
    // Processing buffers (background thread only - no RT constraints)
    std::vector<float> frameBuffer_;
    std::vector<float> windowedBuffer_;
    std::vector<float> fftBuffer_;
    std::vector<float> magnitudeBuffer_;
    std::vector<float> phaseBuffer_;
    
    // Output queue reference
    MaskDeltaQueue* outputQueue_;
    
    // Timing control
    std::atomic<uint64_t> lastColumnTimestamp_;
    
    // Sample rate and hop calculations
    std::atomic<double> sampleRate_;
    size_t samplesPerHop_;
    
    // Thread safety helpers
    mutable std::mutex stateMutex_;

    // Optional UI callback target (set from message thread)
    ColumnCallback columnCallback_;
};

// Inline performance-critical methods

inline float OfflineStftAnalyzer::getProgress() const noexcept {
    const auto total = totalSamples_.load(std::memory_order_acquire);
    if (total == 0) return 0.0f;
    
    const auto current = currentSamplePosition_.load(std::memory_order_acquire);
    return static_cast<float>(current) / static_cast<float>(total);
}

inline void OfflineStftAnalyzer::setPlaybackPosition(uint64_t samplePosition) noexcept {
    playbackSyncPosition_.store(samplePosition, std::memory_order_release);
}

inline void OfflineStftAnalyzer::setAnalysisSpeed(float speedMultiplier) noexcept {
    analysisSpeed_.store(juce::jlimit(0.1f, 10.0f, speedMultiplier), std::memory_order_release);
}

inline void OfflineStftAnalyzer::setColumnStreamingRate(int columnsPerSecond) noexcept {
    columnStreamingRate_.store(juce::jlimit(10, 1000, columnsPerSecond), std::memory_order_release);
}
