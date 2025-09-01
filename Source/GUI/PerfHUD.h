#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <chrono>
#include <array>
#include "HudFormatter.h"

// Forward declarations to avoid circular dependencies
class SpectralCanvasProAudioProcessor;

/**
 * @file PerfHUD.h
 * @brief Performance HUD overlay component for real-time metrics display
 * 
 * Lightweight overlay showing paint→audio latency, CPU usage, GPU frame time,
 * queue depths, xrun count, and device status. Updates at 30Hz via timer
 * without polling the audio thread.
 * 
 * Thread Safety:
 * - UI thread only: All rendering and timer callbacks
 * - Read-only atomic access: Samples telemetry from audio processor
 * - No locks: Uses atomic loads with memory_order_acquire
 */
class PerfHUD : public juce::Component,
                private juce::Timer
{
public:
    explicit PerfHUD(SpectralCanvasProAudioProcessor& processor);
    ~PerfHUD() override;
    
    // Component interface
    void paint(juce::Graphics& g) override;
    void resized() override;
    void setVisible(bool shouldBeVisible) override;
    
    // Toggle HUD visibility
    void toggleVisibility();
    
    // Performance monitoring
    struct Metrics
    {
        float latencyP50Ms = 0.0f;
        float latencyP99Ms = 0.0f;
        float cpuPercent = 0.0f;
        uint32_t gpuFrameTimeUs = 16666; // ~60fps
        uint32_t gpuPeakFrameUs = 16666;
        bool isWarpMode = false;
        bool deviceOk = true;
        uint32_t recoveryCount = 0;
        uint64_t lastRecoveryTimestamp = 0;
        size_t queueDepthUI = 0;
        size_t queueDepthGPU = 0;
        uint32_t dropCount = 0;
        uint32_t xrunCount = 0;
        double sampleRate = 48000.0;
        int blockSize = 512;
    };
    
    // Get last sampled metrics for external use (e.g., automation)
    const Metrics& getLastMetrics() const noexcept { return lastMetrics_; }

private:
    // Timer callback for metric sampling (30Hz)
    void timerCallback() override;
    
    // Sample current metrics from audio processor (atomic reads only)
    Metrics sampleMetrics() noexcept;
    
    // Render HUD overlay with current metrics
    void renderHUD(juce::Graphics& g, const Metrics& metrics);
    
    // Get current timestamp for recovery time calculation
    uint64_t getCurrentTimestamp() const noexcept;
    
    SpectralCanvasProAudioProcessor& audioProcessor_;
    
    // Cached metrics from last update
    Metrics lastMetrics_;
    
    // UI layout constants
    static constexpr int HUD_WIDTH = 180;
#ifdef JUCE_DEBUG
    static constexpr int HUD_HEIGHT = 120;  // Extra room for debug metrics
#else
    static constexpr int HUD_HEIGHT = 100;
#endif
    static constexpr int MARGIN = 8;
    static constexpr int LINE_HEIGHT = 14;
    static constexpr float BACKGROUND_ALPHA = 0.85f;
    
    // Update timing
    static constexpr int UPDATE_INTERVAL_MS = 33; // ~30 Hz
    
    // Font for monospace display
    juce::Font monoFont_;
    
#ifdef JUCE_DEBUG
    // Debug-only perf tracking members
    double lastUpdateMs_ = 0.0;
    uint64_t prevBlocks_ = 0;
    uint64_t prevSamples_ = 0;
    double callbackRateHz_ = 0.0;
    double throughputKSps_ = 0.0;
    double sampleRateUI_ = 0.0;
    int lastBlockSizeUI_ = 0;
#endif
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerfHUD)
};