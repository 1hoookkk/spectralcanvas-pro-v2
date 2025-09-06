#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
// New spectral pipeline headers
#include "Audio/SampleManager.h"
#include "DSP/SpectralModel.h"
#include "DSP/SpectralMask.h"
#include "DSP/SpectralPlayer.h"
#include "Core/Params.h"
#include "Core/MessageBus.h"
#include "Core/LatencyTracker.h"
#include "Core/Phase4DebugTap.h"
#include "Core/RealtimeMemoryManager.h"
#include "Core/AtlasIds.h"
#include "DSP/SpectralEngine.h"
#include "DSP/SampleLoader.h"
#include "DSP/MaskTestFeeder.h"
#include "Core/AudioPathId.h"
#include "SpectralPaintProcessor.h"
#include "DSP/OfflineStftAnalyzer.h"
#include "DSP/HopScheduler.h"
#include "DSP/RealtimeSpectral.h"
#include "DSP/HopFifo.h"
#include "Core/TiledAtlas.h"
#include "Core/MaskColumn.h"
#include "Core/MaskColumnDelta.h"
#include "Core/RealtimeMemorySystem.h"
#include "Core/UndoManager.h"

#ifdef PHASE4_EXPERIMENT
#include "DSP/KeyFilter.h"
#include "DSP/SpectralEngineStub.h"
#endif

class SpectralCanvasProAudioProcessor : public juce::AudioProcessor,
                                       public juce::AudioProcessorValueTreeState::Listener
{
public:
    // Audio path tracking enum
    // --- Audio path selection (mutually exclusive) ---
    enum class AudioPath : uint8_t {
        Silent = 0,
        TestFeeder,
        Phase4Synth,
        ModernPaint,        // New JUCE DSP-based spectral painting
        SpectralResynthesis // Spectral resynthesis from loaded audio file
    };

    SpectralCanvasProAudioProcessor();
    ~SpectralCanvasProAudioProcessor() override;

    void setAudioPathFromParams(); // call on parameter change thread

    // AudioProcessor interface
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void suspendProcessing(bool shouldBeSuspended);
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) noexcept override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    
    // Custom latency reporting - atomic pattern with JUCE base class
    void updateReportedLatency(int samples) noexcept;
    int getAtomicLatencySamples() const noexcept;

    // --- Control helpers (callable from editor/loader) ---
    /** Explicitly enable/disable the diagnostic TestFeeder. */
    void setUseTestFeeder(bool shouldUse) noexcept;

    /** Notify processor that a real sample was loaded; resets playhead and length. */
    void onSampleLoaded(std::shared_ptr<juce::AudioBuffer<float>> newBuffer, double sourceSR) noexcept;

private:
    /** Helper: safe sample rendering (adds to buffer), no modulo unless looping */
    void addSampleBlock(juce::AudioBuffer<float>& out) noexcept;
    
public:
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override { juce::ignoreUnused(index); }
    const juce::String getProgramName(int index) override { juce::ignoreUnused(index); return {}; }
    void changeProgramName(int index, const juce::String& newName) override { juce::ignoreUnused(index, newName); }
    
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameter management
    juce::AudioProcessorValueTreeState& getValueTreeState() { return apvts; }
    
    // Inter-thread communication
    SpectralDataQueue& getSpectralDataQueue() { return spectralDataQueue; }
    ParameterQueue& getParameterQueue() { return parameterQueue; }
    MaskColumnQueue& getMaskColumnQueue() { return maskColumnQueue; }
    
    // Sample loading (UI thread access) - using existing SampleLoader
    SampleLoader& getSampleLoader() { return sampleLoader; }
    
    // Test feedback system (UI thread access)
    void generateImmediateAudioFeedback();
    
    // Undo functionality
    void saveUndoState();
    bool canUndo() const;
    void performUndo();

    // Phase 2-3 debug info getters
    int getBlockSize() const;
    double getSampleRate() const;
    
    // Helpers for Editor wiring
    static constexpr int getFftSize() noexcept { return AtlasConfig::FFT_SIZE; }
    static constexpr int getHopSize() noexcept { return AtlasConfig::HOP_SIZE; }
    
    // Phase 2-3 validation metrics (UI thread access)
    struct PerformanceMetrics
    {
        float medianLatencyMs = 0.0f;
        float p95LatencyMs = 0.0f;
        size_t queueDepth = 0;
        size_t dropCount = 0;
        size_t xrunCount = 0;
        uint64_t processedSamples = 0;
    };
    PerformanceMetrics getPerformanceMetrics() const;
    
    // Lock-free GUI snapshot for thread-safe paint operations
    struct CanvasSnapshot
    {
        PerformanceMetrics metrics;
        AudioPath currentPath = AudioPath::TestFeeder;
        bool wroteAudioFlag = false;
        double sampleRate = 48000.0;
        int blockSize = 512;
        
        #ifdef PHASE4_EXPERIMENT
        int activeBins = 0;
        int totalBins = 257;
        uint64_t maskPushCount = 0;
        uint64_t maskDropCount = 0;
        float maxMagnitude = 0.0f;
        uint64_t phase4Blocks = 0;
        #endif
        
        // Timestamp for freshness validation
        double timestampMs = 0.0;
    };
    
    // Double-buffered snapshot bus - no shared_ptr atomics, no heap churn, no races  
    class SnapshotBus {
    public:
        // Producer (audio/hop thread): write into the "next" buffer, then publish
        void publish(const CanvasSnapshot& s) noexcept {
            const uint32_t next = (index_.load(std::memory_order_relaxed) ^ 1u);
            buffers_[next] = s; // copy; avoid allocations in RT by pre-sizing path if needed
            // Publish: bump sequence with buffer index in LSB so readers can verify
            const uint64_t seq = seq_.load(std::memory_order_relaxed);
            seq_.store(((seq + 1ull) & ~1ull) | next, std::memory_order_release);
            index_.store(next, std::memory_order_relaxed);
        }

        // Consumer (GUI paint): read a consistent snapshot with retry
        bool tryLoad(CanvasSnapshot& out) const noexcept {
            for (int tries = 0; tries < 3; ++tries) {
                const uint64_t s1 = seq_.load(std::memory_order_acquire);
                const uint32_t i  = static_cast<uint32_t>(s1 & 1ull);
                out = buffers_[i]; // copy
                const uint64_t s2 = seq_.load(std::memory_order_acquire);
                if (s1 == s2) return true; // no publish happened mid-read
            }
            return false;
        }

    private:
        mutable CanvasSnapshot buffers_[2];
        std::atomic<uint32_t> index_{0};
        std::atomic<uint64_t> seq_{0}; // seq || index(LSB)
    };
    
    bool getCanvasSnapshot(CanvasSnapshot& out) const;
    void publishCanvasSnapshot() const;
    
    // Tripwire counter access for PerfHUD
    uint32_t getBadBinSkips() const noexcept { return badBinSkips_.load(std::memory_order_relaxed); }
    uint32_t getBadColSkips() const noexcept { return badColSkips_.load(std::memory_order_relaxed); }
    
    // Perf HUD helpers (non-RT consumers read these)
    void collectPerfCounters(uint64_t& outTotalBlocks,
                             uint64_t& outTotalSamples,
                             int& outLastBlockSize,
                             double& outSampleRate) const noexcept
    {
        outTotalBlocks   = totalBlocksProcessed_.load(std::memory_order_relaxed);
        outTotalSamples  = totalSamplesProcessed_.load(std::memory_order_relaxed);
        outLastBlockSize = lastBlockSize_.load(std::memory_order_relaxed);
        outSampleRate    = currentSampleRate;
    }
    
    // Sequence number generation for mask ordering (UI thread)
    uint32_t getNextMaskSequenceNumber() { return nextMaskSequenceNumber_.fetch_add(1, std::memory_order_relaxed); }
    
    // Unified timebase epoch accessor (UI thread)
    uint64_t getEpochSteadyNanos() const { return epochSteadyNanos_.load(std::memory_order_relaxed); }
    
    // Enhanced MaskColumn with debug sequence
    struct MaskColumnEx : MaskColumn {
#if PHASE4_DEBUG_TAP
        uint64_t debugSeq = 0;
#endif
    };
    
    // Phase 4 mask column push method (UI thread) with diagnostics and debug tap
    bool pushMaskColumn(const MaskColumn& mask);
    
    // Modern paint event push method (UI thread) - lightweight 12-byte events
    bool pushPaintEvent(float y, float intensity, uint32_t timestampMs = 0) noexcept;
    
    // Sample loading API (called from Editor)
    bool loadSampleFile (const juce::File& f);
    
    // Renderer activation API for sample loading pipeline
    bool hasActiveRenderer() const noexcept;
    bool activateSampleRenderer(const juce::AudioBuffer<float>& sampleData, double sourceSampleRate);
    void deactivateRenderer();
    
    // Spectral pipeline access for editor
    SpectralModel& getSpectralModel() { return spectralModel; }
    SpectralMask& getSpectralMask() { return spectralMask; }
    
    // Debug tap access for overlay
    Phase4DebugTap& getDebugTap() noexcept { return debugTap_; }
    
    // UI-safe readout for audio path diagnostics  
    AudioPath getCurrentPath() const noexcept { return currentPath_.load(std::memory_order_acquire); }
    bool getWroteAudioFlag() const noexcept { return wroteAudioFlag_.load(std::memory_order_relaxed); }
    
    // Queue diagnostics (UI thread access)
    uint64_t getMaskPushCount() const noexcept { return maskPushCount_.load(std::memory_order_relaxed); }
    uint64_t getMaskDropCount() const noexcept { return maskDropCount_.load(std::memory_order_relaxed); }
    
#ifdef PHASE4_EXPERIMENT
    // SpectralEngineStub diagnostics
    uint64_t getMaskPopCount() const noexcept { return spectralStub.getPopCount(); }
    float getMaxMagnitude() const noexcept { return spectralStub.getMaxMagnitude(); }
#endif
    
#ifdef PHASE4_EXPERIMENT
    int getActiveBinCount() const noexcept; // Thread-safe read of active bin count
    int getNumBins() const noexcept; // Thread-safe read of total bin count for diagnostics
    uint64_t getPhase4Blocks() const noexcept { return phase4Blocks_.load(std::memory_order_relaxed); }
    
    // Paint→Audio wiring diagnostics (RT-safe reads)
    uint64_t getPaintsEnqueued() const noexcept { return paintsEnqueued_.load(std::memory_order_relaxed); }
    uint64_t getMasksApplied() const noexcept { return masksApplied_.load(std::memory_order_relaxed); }
    uint64_t getLastMaskTimestamp() const noexcept { return lastMaskTimestamp_.load(std::memory_order_relaxed); }
    
    // Helper for timestamp (RT-safe)
    static uint64_t getCurrentTimeUs() noexcept;
#endif
    
    // AudioProcessorValueTreeState::Listener interface
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    // Public APVTS access for editor (matches plugin designer's convention)
    juce::AudioProcessorValueTreeState apvts;

private:
    
    // Audio path tracking state (state machine)
    std::atomic<AudioPath> currentPath_ { AudioPath::Silent };
    AudioPath lastPath_ { AudioPath::Silent }; // accessed only from audio thread
    std::atomic<bool> wroteAudioFlag_ { false }; // set per block on audio thread
    
    // Audio generation helpers
    void generateFallbackBeep(juce::AudioBuffer<float>& buffer, int numSamples) noexcept;
    void fallbackBeep(juce::AudioBuffer<float>& buffer) noexcept;

#ifdef PHASE4_EXPERIMENT
    // RT-safe state resets for path transitions
    void rtResetPhase4_() noexcept;
    void rtResetTestFeeder_() noexcept;
    void rtResetModernPaint_() noexcept;
#endif
    
    // Inter-thread communication (lock-free)
    SpectralDataQueue spectralDataQueue;
    ParameterQueue parameterQueue;
    MaskColumnQueue maskColumnQueue;
    SampleQueue sampleQueue;
    
    // Tiled atlas message queues
    MaskDeltaQueue maskDeltaQueue;
    AtlasUpdateQueue atlasUpdateQueue;
    PageManagementQueue pageManagementQueue;
    
    // Handle-based sample state (audio thread only)
    std::optional<SampleView> currentSample;
    uint32_t currentSampleHandle = 0;
    uint64_t samplePosition = 0;
    
    // Audio processing state
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
    
    // RT-safe spectral processing
    std::unique_ptr<SpectralEngine> spectralEngine;
    
    // RT-safe sample loading for Resynth mode
    SampleLoader sampleLoader;
    
    // RT-safe test harness for paint-to-audio validation
    MaskTestFeeder maskTestFeeder;
    
    // Modern JUCE DSP-based spectral painting processor
    std::unique_ptr<SpectralPaintProcessor> spectralPaintProcessor;
    
    // Phase 4 (HEAR): RT-safe STFT masking
    HopScheduler hop_;
    RealtimeSpectral rt_;
    
#ifdef PHASE4_EXPERIMENT
    // Phase 4 experimental oscillator bank and key filter
    dsp::SpectralEngineStub spectralStub;
    dsp::KeyFilter keyFilter;
    
    // KeyFilter rebuild queue for thread safety
    struct KeyFilterRebuildRequest {
        int rootNote;
        dsp::ScaleType scale;
    };
    SpscRingBuffer<KeyFilterRebuildRequest, 8> keyFilterRebuildQueue;
    
    // Parameter atomics for RT-safety
    std::atomic<bool> useTestFeeder_{false};
    std::atomic<bool> keyFilterEnabled_{true};
    std::atomic<float> oscGain_{0.2f};
    std::atomic<int>   mode_{0};         // 0=Synth, 1=Resynth, 2=Hybrid
    std::atomic<float> blend_{0.0f};     // 0=synth, 1=resynth
    std::atomic<float> respeed_{1.0f};   // 0.25..4.0
    std::atomic<float> brushSize_{8.0f};      // Brush size target
    std::atomic<float> brushStrength_{0.8f};  // Brush strength target
    std::atomic<int> scaleType_{1};  // 0=Chromatic, 1=Major, 2=Minor
    
    // Diagnostic telemetry atomics
    std::atomic<uint8_t> currentAudioPath_{0};  // AudioPathId enum
    std::atomic<uint32_t> pushesObserved_{0};   // UI thread mask pushes
    std::atomic<uint32_t> popsObserved_{0};     // Audio thread mask pops
    std::atomic<int> rootNote_{0};   // 0-11
    std::atomic<bool> useModernPaint_{false};
    
    // Diagnostic counter to prove Phase4 branch executes
    std::atomic<uint64_t> phase4Blocks_{0};
    
    // Paint→Audio wiring diagnostics (RT-safe)
    std::atomic<uint64_t> paintsEnqueued_{0};
    std::atomic<uint64_t> masksApplied_{0};
    std::atomic<uint64_t> lastMaskTimestamp_{0};
    
    // Preallocated mask ring for bounded pop (RT-safe, no allocations)
    static constexpr int MAX_MASKS_PER_BLOCK = 4;
    MaskColumn maskRing_[MAX_MASKS_PER_BLOCK];
    int maskRingIdx_ = 0;
    
    // Pre-calculated timestamp for RT safety (Phase4 only)
    std::atomic<uint64_t> rtTimestampUs_{0};
#endif
    
    // Phase 2-3 validation infrastructure
    RTLatencyTracker latencyTracker_;
    std::atomic<uint64_t> processedSampleCount_{0};
    std::atomic<uint64_t> epochSteadyNanos_{0};        // Unified timebase epoch
    std::atomic<uint32_t> nextMaskSequenceNumber_{1};
    std::atomic<uint64_t> xrunCount_{0};              // RT-safe XRun detection counter
    
    // Phase 4 queue diagnostics  
    std::atomic<uint64_t> maskPushCount_{0};
    std::atomic<uint64_t> maskDropCount_{0};
    
    // Tripwire counters for heap corruption detection
    std::atomic<uint32_t> badBinSkips_{0};      // Invalid bin count detections
    std::atomic<uint32_t> badColSkips_{0};      // Invalid column position detections  
    
    // RT-safe diagnostic counters (no logging in RT paths)
    std::atomic<uint64_t> pushMaskAttempts_{0};
    std::atomic<uint64_t> pushMaskRejects_{0};
    std::atomic<uint64_t> silenceBlockCount_{0};
    
    // Debug tap for SPSC integrity diagnosis
    Phase4DebugTap debugTap_;
    
    // ===== Spectral pipeline (MVP) =====
    SampleManager  sampleManager;
    SpectralModel  spectralModel;
    SpectralMask   spectralMask;
    SpectralPlayer spectralPlayer;
    bool           spectralReady { false };
    
    // Undo Manager
    UndoManager undoManager;

    // Renderer activation state for HUD
    std::atomic<bool> hasActiveRenderer_{ false };
    
    // RT-safe parameter smoothers for fast-changing controls
    juce::SmoothedValue<float> oscGainSmoother_;
    juce::SmoothedValue<float> brushSizeSmoother_;
    juce::SmoothedValue<float> brushStrengthSmoother_;
    double smoothingTimeSec_ = 0.050; // 50ms default
    
    // RT-safe perf counters (no clocks on audio thread)
    std::atomic<uint64_t> totalBlocksProcessed_{0};
    std::atomic<uint64_t> totalSamplesProcessed_{0};
    std::atomic<int> lastBlockSize_{0};
    
    // Paint telemetry atomics (read-and-reset for HUD)
    std::atomic<uint32_t> drainsThisBlock_{0};
    std::atomic<uint32_t> maskQueueDepth_{0};

    // Hybrid mode scratch buffer (preallocated in prepareToPlay)
    juce::AudioBuffer<float> hybridBuffer_;

    

    // ---- Sample playback state (cross-mode) ----
    // Shared buffer managed by loader/editor; read on audio thread.
    std::shared_ptr<juce::AudioBuffer<float>> currentSample_;
    int64_t  loadedSampleLength_{0};
    int64_t  playhead_{0};
    bool     sampleLooping_{false};
    double   sourceSampleRate_{0.0};

    // Diagnostic / test path flag (handled by atomic at line 331)

    // Helper to fetch APVTS values safely (call on audio thread)
    inline float getParamFast(const juce::String& paramId) const noexcept
    {
        if (auto* p = apvts.getRawParameterValue(paramId))
            return p->load();
        return 0.0f;
    }
    
    // Tiled atlas system components
    std::shared_ptr<TiledAtlas> tiledAtlas_;
    std::unique_ptr<OfflineStftAnalyzer> offlineAnalyzer_;
    
    // Atlas integration methods
    void initializeTiledAtlas() noexcept;
    void shutdownTiledAtlas() noexcept;
    void processTiledAtlasMessages() noexcept; // Called from audio thread
    void convertMaskColumnsToDeltas(uint64_t currentSamplePos) noexcept; // RT-safe conversion bridge
    
    // Atlas public access for editor
public:
    std::shared_ptr<TiledAtlas> getTiledAtlas() const noexcept { return tiledAtlas_; }
    MaskDeltaQueue& getMaskDeltaQueue() noexcept { return maskDeltaQueue; }
    uint32_t getDeltaDrainsPerBlock() const noexcept { return hop_.drainsPerBlock(); }
    AtlasUpdateQueue& getAtlasUpdateQueue() noexcept { return atlasUpdateQueue; }
    PageManagementQueue& getPageManagementQueue() noexcept { return pageManagementQueue; }
    OfflineStftAnalyzer* getOfflineAnalyzer() const noexcept { return offlineAnalyzer_.get(); }
    
    // Thread-safe telemetry accessors for HUD (read-and-reset)
    uint32_t getDrainsThisBlockAndReset() noexcept { return drainsThisBlock_.exchange(0, std::memory_order_acq_rel); }
    uint32_t getMaskQueueDepth() const noexcept { return maskQueueDepth_.load(std::memory_order_relaxed); }
    uint64_t getProcessedBlockCount() const noexcept { return totalBlocksProcessed_.load(std::memory_order_relaxed); }
    
    // Phase 1: Current column tracking for UI synchronization
    uint32_t getCurrentColumnIndex() const noexcept { return currentColumnIndex_.load(std::memory_order_relaxed); }
    
private:
    // Current processing position (Phase 1 completion)
    std::atomic<uint32_t> currentColumnIndex_{0};
    
    // Thread-safe latency and GUI snapshot storage
    mutable std::atomic<int> latencySamples_{AtlasConfig::FFT_SIZE - AtlasConfig::HOP_SIZE}; // Default: FFT_SIZE - HOP_SIZE
    mutable SnapshotBus snapshotBus_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralCanvasProAudioProcessor)
};
