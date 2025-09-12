#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Utils/SpscQueueLite.h"
#include "../DSP/SpectralProcessor.h"
#include "../DSP/SynthEngineStub.h"
#include "MaskColumn.h"
#include "Params.h"
#include <atomic>

class SpectralCanvasLiteProcessor : public juce::AudioProcessor
{
public:
    enum class ProcessingMode { Effect = 0, Synth, Hybrid };

    SpectralCanvasLiteProcessor();
    ~SpectralCanvasLiteProcessor() override = default;

    // AudioProcessor
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // Program/State
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "SpectralCanvasLite"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return "Default"; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // APVTS
    juce::AudioProcessorValueTreeState& getAPVTS() noexcept { return apvts_; }

    // Queue push (from Editor)
    void pushPaintEvent(const MaskColumn& e) noexcept { 
        bool success = queue_.tryPush(e);
        if (success) {
            eventsEnqueued_.fetch_add(1, std::memory_order_relaxed);
        } else {
            eventsDropped_.fetch_add(1, std::memory_order_relaxed);
        }
    }

    // Peak GR telemetry (from SpectralProcessor)
    float getPeakGrDb() const noexcept { return spectral_.getPeakGrDb(); }

    // Performance counters for Tracy/HUD
    struct PerfStats {
        uint64_t eventsEnqueued = 0;
        uint64_t eventsDrained = 0;
        uint64_t eventsDropped = 0;
    };
    PerfStats getPerfStats() const noexcept {
        return {eventsEnqueued_.load(), eventsDrained_.load(), eventsDropped_.load()};
    }

private:
    juce::AudioProcessorValueTreeState apvts_;
    SpectralProcessor  spectral_;
    SynthEngineStub    synth_;

    SpscQueueLite<MaskColumn, 1024> queue_;

    // Performance counters (thread-safe)
    std::atomic<uint64_t> eventsEnqueued_{0};
    std::atomic<uint64_t> eventsDrained_{0};
    std::atomic<uint64_t> eventsDropped_{0};

    // Cached parameter pointers for RT access
    std::atomic<float>* pMaskFloorDb_  = nullptr;
    std::atomic<float>* pOutputTrimDb_ = nullptr;
    std::atomic<float>* pMode_         = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralCanvasLiteProcessor)
};
