#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "Core/Params.h"
#include "Core/MessageBus.h"
#include "Core/LatencyTracker.h"
#include "Core/Phase4DebugTap.h"
#include "DSP/SpectralEngine.h"
#include "DSP/SampleLoader.h"
#include "DSP/MaskTestFeeder.h"
#include "SpectralPaintProcessor.h"

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
        ModernPaint  // New JUCE DSP-based spectral painting
    };

    SpectralCanvasProAudioProcessor();
    ~SpectralCanvasProAudioProcessor() override;

    void setAudioPathFromParams(); // call on parameter change thread

    // AudioProcessor interface
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) noexcept override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    
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
    
    // Sample loading (UI thread access)
    bool loadSampleFile(const juce::File& audioFile);
    SampleLoader& getSampleLoader() { return sampleLoader; }
    
    // Test feedback system (UI thread access)
    void generateImmediateAudioFeedback();
    
    // Phase 2-3 debug info getters
    int getBlockSize() const;
    double getSampleRate() const;
    
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
    
#ifdef PHASE4_EXPERIMENT
    // Phase 4 experimental oscillator bank and key filter
    dsp::SpectralEngineStub spectralStub;
    dsp::KeyFilter keyFilter;
    
    // Parameter atomics for RT-safety
    std::atomic<bool> useTestFeeder_{false};
    std::atomic<bool> keyFilterEnabled_{true};
    std::atomic<float> oscGain_{0.2f};
    std::atomic<int> scaleType_{1};  // 0=Chromatic, 1=Major, 2=Minor
    std::atomic<int> rootNote_{0};   // 0-11
    
    // Diagnostic counter to prove Phase4 branch executes
    std::atomic<uint64_t> phase4Blocks_{0};
#endif
    
    // Phase 2-3 validation infrastructure
    RTLatencyTracker latencyTracker_;
    std::atomic<uint64_t> processedSampleCount_{0};
    std::atomic<uint64_t> epochSteadyNanos_{0};        // Unified timebase epoch
    std::atomic<uint32_t> nextMaskSequenceNumber_{1};
    
    // Phase 4 queue diagnostics  
    std::atomic<uint64_t> maskPushCount_{0};
    std::atomic<uint64_t> maskDropCount_{0};
    
    // RT-safe diagnostic counters (no logging in RT paths)
    std::atomic<uint64_t> pushMaskAttempts_{0};
    std::atomic<uint64_t> pushMaskRejects_{0};
    std::atomic<uint64_t> silenceBlockCount_{0};
    
    // Debug tap for SPSC integrity diagnosis
    Phase4DebugTap debugTap_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralCanvasProAudioProcessor)
};