#include "PerfHUD.h"
#include "../SpectralCanvasProAudioProcessor.h"
#include "../Core/ContinuousVerification.h"
#include "../Core/GPUStatus.h"
#include "../Viz/GpuRenderer.h"

#ifdef DISABLE_PERF_HUD
// Compile-time rollback option
PerfHUD::PerfHUD(SpectralCanvasProAudioProcessor& processor, GpuRenderer* renderer) 
    : audioProcessor_(processor), gpuRenderer_(renderer) 
{
    setVisible(false);
}

PerfHUD::~PerfHUD() = default;
void PerfHUD::paint(juce::Graphics&) {}
void PerfHUD::resized() {}
void PerfHUD::setVisible(bool) {}
void PerfHUD::toggleVisibility() {}
void PerfHUD::timerCallback() {}
PerfHUD::Metrics PerfHUD::sampleMetrics() noexcept { return {}; }
void PerfHUD::renderHUD(juce::Graphics&, const Metrics&) {}
uint64_t PerfHUD::getCurrentTimestamp() const noexcept { return 0; }

#else

PerfHUD::PerfHUD(SpectralCanvasProAudioProcessor& processor, GpuRenderer* renderer)
    : audioProcessor_(processor), gpuRenderer_(renderer)
    , monoFont_(juce::FontOptions().withName(juce::Font::getDefaultMonospacedFontName()).withHeight(10.0f))
{
    // Start hidden by default (controlled by parameter)
    setVisible(false);
    
    // Set fixed size for consistent layout
    setSize(HUD_WIDTH, HUD_HEIGHT);
    
    // Position in top-right corner (will be repositioned by parent)
    setTopRightPosition(0, MARGIN);
    
    // Start metric sampling timer
    startTimer(UPDATE_INTERVAL_MS);
    
    // Initial metrics sample
    lastMetrics_ = sampleMetrics();
    
#ifdef JUCE_DEBUG
    // Initialize debug tracking
    lastUpdateMs_ = 0.0;
    prevBlocks_ = 0;
    prevSamples_ = 0;
    callbackRateHz_ = 0.0;
    throughputKSps_ = 0.0;
    sampleRateUI_ = 0.0;
    lastBlockSizeUI_ = 0;
#endif
}

PerfHUD::~PerfHUD()
{
    stopTimer();
}

void PerfHUD::paint(juce::Graphics& g)
{
    if (!isVisible()) return;
    
    // Measure render time for self-monitoring
    auto startTime = std::chrono::high_resolution_clock::now();
    
    renderHUD(g, lastMetrics_);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto renderDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    // Self-monitor: warn if render takes >2ms (should be <1ms typically)
    if (renderDuration.count() > 2000) {
        DBG("PerfHUD render took " + juce::String(renderDuration.count()) + " microseconds");
    }
}

void PerfHUD::resized()
{
    // Fixed size component - no dynamic resizing needed
}

void PerfHUD::setVisible(bool shouldBeVisible)
{
    Component::setVisible(shouldBeVisible);
    
    if (shouldBeVisible) {
        // Ensure we're sampling when visible
        if (!isTimerRunning()) {
            startTimer(UPDATE_INTERVAL_MS);
        }
        
        // Immediate update on show
        lastMetrics_ = sampleMetrics();
        repaint();
    } else {
        // Stop sampling when hidden to save CPU
        stopTimer();
    }
}

void PerfHUD::toggleVisibility()
{
    setVisible(!isVisible());
}

void PerfHUD::timerCallback()
{
    if (!isVisible()) return;
    
    // Sample current metrics (atomic reads only)
    lastMetrics_ = sampleMetrics();
    
#ifdef JUCE_DEBUG
    // Pull counters (non-RT)
    uint64_t totalBlocks = 0, totalSamples = 0;
    int lastBlock = 0;
    double sr = 0.0;
    audioProcessor_.collectPerfCounters(totalBlocks, totalSamples, lastBlock, sr);
    
    const double nowMs = juce::Time::getMillisecondCounterHiRes();
    const double dtSec = (lastUpdateMs_ > 0.0) ? (nowMs - lastUpdateMs_) * 0.001 : 0.0;
    
    if (dtSec > 0.0)
    {
        const uint64_t dBlocks = totalBlocks - prevBlocks_;
        const uint64_t dSamples = totalSamples - prevSamples_;
        callbackRateHz_ = static_cast<double>(dBlocks) / dtSec;
        throughputKSps_ = static_cast<double>(dSamples) / dtSec / 1000.0;
        lastBlockSizeUI_ = lastBlock;
        sampleRateUI_ = sr;
    }
    
    prevBlocks_ = totalBlocks;
    prevSamples_ = totalSamples;
    lastUpdateMs_ = nowMs;
#endif
    
    // Trigger repaint
    repaint();
}

PerfHUD::Metrics PerfHUD::sampleMetrics() noexcept
{
    Metrics metrics;
    
    try {
        // Sample performance metrics from audio processor
        auto perfMetrics = audioProcessor_.getPerformanceMetrics();
        metrics.latencyP50Ms = perfMetrics.medianLatencyMs;
        metrics.latencyP99Ms = perfMetrics.p95LatencyMs; // Using P95 as P99 approximation
        metrics.queueDepthUI = perfMetrics.queueDepth;
        metrics.dropCount = static_cast<uint32_t>(perfMetrics.dropCount);
        metrics.xrunCount = static_cast<uint32_t>(perfMetrics.xrunCount);
        
        // Sample audio configuration
        metrics.sampleRate = audioProcessor_.getSampleRate();
        metrics.blockSize = audioProcessor_.getBlockSize();
        
        // Estimate CPU usage from processing metrics (Debug builds only)
#ifdef JUCE_DEBUG
        // Simple CPU estimate: (processed samples / sample rate) * 100 / elapsed time
        static auto lastSampleTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSampleTime).count();
        
        if (elapsedMs > 100) {  // Update every 100ms
            static uint64_t lastProcessedSamples = 0;
            uint64_t currentProcessedSamples = perfMetrics.processedSamples;
            uint64_t samplesDelta = currentProcessedSamples - lastProcessedSamples;
            
            if (samplesDelta > 0 && elapsedMs > 0) {
                float processingRatio = (samplesDelta / metrics.sampleRate) * 1000.0f / static_cast<float>(elapsedMs);
                metrics.cpuPercent = juce::jlimit(0.0f, 100.0f, processingRatio * 100.0f);
            }
            
            lastProcessedSamples = currentProcessedSamples;
            lastSampleTime = currentTime;
        }
#else
        metrics.cpuPercent = 0.0f; // No CPU monitoring in release builds
#endif
        
        // Sample GPU status from renderer with safety checks  
        if (gpuRenderer_ && gpuRenderer_->isInitialized()) {
            try {
                // Safe access to GPU metrics - may fail during device transitions
                metrics.gpuFrameTimeUs = static_cast<uint32_t>(gpuRenderer_->getFrameTime() * 1000.0f);
                metrics.gpuPeakFrameUs = metrics.gpuFrameTimeUs; // TODO: track peak separately
                metrics.isWarpMode = false; // TODO: query from D3D11Renderer
                metrics.deviceOk = gpuRenderer_->checkDeviceStatus() && audioProcessor_.hasActiveRenderer();
            } catch (...) {
                // GPU access failed (device lost, etc.) - use safe defaults
                metrics.gpuFrameTimeUs = 0;
                metrics.gpuPeakFrameUs = 0;
                metrics.isWarpMode = false;
                metrics.deviceOk = false;
            }
        } else {
            // No renderer or not initialized
            metrics.gpuFrameTimeUs = 0;
            metrics.gpuPeakFrameUs = 0;
            metrics.isWarpMode = false;
            metrics.deviceOk = audioProcessor_.hasActiveRenderer();
        }
        metrics.recoveryCount = 0;
        metrics.lastRecoveryTimestamp = 0;
        metrics.queueDepthGPU = 0;
        
        // Sample tripwire counters for heap corruption detection
        metrics.badBinSkips = audioProcessor_.getBadBinSkips();
        metrics.badColSkips = audioProcessor_.getBadColSkips();
        metrics.deltaDrains = audioProcessor_.getDeltaDrainsPerBlock();
        
    } catch (...) {
        // Defensive: never crash the UI thread from metric sampling
        DBG("PerfHUD: Exception during metric sampling");
    }
    
    return metrics;
}

void PerfHUD::renderHUD(juce::Graphics& g, const Metrics& metrics)
{
#ifdef JUCE_DEBUG
    // Only show detailed HUD in Debug builds
    // Semi-transparent dark background
    g.setColour(juce::Colours::black.withAlpha(BACKGROUND_ALPHA));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0f);
    
    // Light border
    g.setColour(juce::Colours::lightgrey.withAlpha(0.3f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 4.0f, 1.0f);
    
    // Set monospace font for aligned columns
    g.setFont(monoFont_);
    
    int y = MARGIN;
    
    // Row 1: Latency p50/p99, CPU %
    g.setColour(juce::Colours::lightcyan);
    auto latencyText = "Lat:" + HudFormatter::formatLatency(metrics.latencyP50Ms) 
                     + "/" + HudFormatter::formatLatency(metrics.latencyP99Ms) + "ms";
    g.drawText(latencyText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT, 
               juce::Justification::left);
    
    auto cpuText = "CPU:" + HudFormatter::formatCpuPercent(metrics.cpuPercent) + "%";
    g.drawText(cpuText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT, 
               juce::Justification::right);
    y += LINE_HEIGHT;
    
    // Row 2: GPU frame time, Device mode
    g.setColour(juce::Colours::lightgreen);
    auto gpuText = "GPU:" + HudFormatter::formatFrameTime(metrics.gpuFrameTimeUs) + "ms";
    g.drawText(gpuText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::left);
    
    auto deviceText = HudFormatter::formatDeviceMode(metrics.isWarpMode, metrics.deviceOk);
    g.drawText(deviceText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::right);
    y += LINE_HEIGHT;
    
    // Row 3: Recovery count/time
    g.setColour(juce::Colours::lightyellow);
    uint64_t currentTime = getCurrentTimestamp();
    auto recoveryText = "Rec:" + std::to_string(metrics.recoveryCount) + " " +
                       HudFormatter::formatRecoveryTime(metrics.lastRecoveryTimestamp, currentTime);
    g.drawText(recoveryText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::left);
    y += LINE_HEIGHT;
    
    // Row 4: Queue depths and drops
    g.setColour(metrics.dropCount > 0 ? juce::Colours::orange : juce::Colours::white);
    auto queueText = "Q:" + std::to_string(metrics.queueDepthUI) + "/" +
                     std::to_string(metrics.queueDepthGPU) + " D:" + 
                     std::to_string(metrics.dropCount);
    g.drawText(queueText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::left);
    
    // XRuns (highlight if > 0)
    g.setColour(metrics.xrunCount > 0 ? juce::Colours::red : juce::Colours::white);
    auto xrunText = "XR:" + std::to_string(metrics.xrunCount);
    g.drawText(xrunText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::right);
    y += LINE_HEIGHT;
    
    // Row 5: Tripwire counters (heap corruption protection)
    if (metrics.badBinSkips > 0 || metrics.badColSkips > 0 || metrics.deltaDrains > 0) {
        g.setColour(juce::Colours::red); // Highlight corruption protection in red
        auto tripwireText = "TW:" + std::to_string(metrics.badBinSkips) + "/" + 
                           std::to_string(metrics.badColSkips) + " ΔD:" + 
                           std::to_string(metrics.deltaDrains);
        g.drawText(tripwireText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
                   juce::Justification::left);
        y += LINE_HEIGHT;
    }
    
    // Footer: Audio config and toggle hint
    g.setColour(juce::Colours::lightgrey);
    auto configText = HudFormatter::formatAudioConfig(metrics.sampleRate, metrics.blockSize);
    g.drawText(configText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::left);
    
    g.drawText("[H]ide", MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::right);
    
    // Add debug performance metrics
    y += LINE_HEIGHT;
    g.setColour(juce::Colours::cyan);
    auto perfText = "CB:" + juce::String(callbackRateHz_, 1) + "Hz T:" + 
                    juce::String(throughputKSps_, 1) + "kS/s";
    g.drawText(perfText, MARGIN, y, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::left);
#else
    // Minimal release build display - just show audio config
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0f);
    
    g.setColour(juce::Colours::white);
    g.setFont(monoFont_);
    
    auto configText = HudFormatter::formatAudioConfig(metrics.sampleRate, metrics.blockSize);
    g.drawText(configText, MARGIN, MARGIN, getWidth() - 2*MARGIN, LINE_HEIGHT,
               juce::Justification::left);
#endif
}

uint64_t PerfHUD::getCurrentTimestamp() const noexcept
{
    auto now = std::chrono::steady_clock::now();
    return static_cast<uint64_t>(now.time_since_epoch().count());
}

#endif // DISABLE_PERF_HUD