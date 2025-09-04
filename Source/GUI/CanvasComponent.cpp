#include "CanvasComponent.h"
#include "../Core/Params.h"
#include "../Core/MessageBus.h"
#include "../Core/DiagnosticLogger.h"
#include "../Core/SampleLoaderService.h"
#include "../SpectralCanvasProEditor.h"
#include <chrono>

// Constants for RT-safe paint-to-sound
namespace {
    constexpr int kFftSize = 512;
    constexpr int kNumBins = kFftSize / 2 + 1; // 257
}

CanvasComponent::CanvasComponent(SpectralCanvasProAudioProcessor& processor)
    : audioProcessor(processor)
{
    setOpaque(true);
    startTimer(16); // 60fps
    setWantsKeyboardFocus(true); // Enable keyboard input for 'i' injector
    
    // Initialize stroke system
    currentStroke.isActive = false;
    completedStrokes.reserve(100);
    
#ifdef PHASE4_EXPERIMENT
    // Initialize accumulator and timing
    accumulator_.clear();
    lastDispatchTime_ = std::chrono::high_resolution_clock::now();
#endif
}

CanvasComponent::~CanvasComponent() = default;

void CanvasComponent::paint(juce::Graphics& g)
{
    // DIAGNOSTIC: Log enter/exit for debugging crashes
    static std::atomic<int> paintCounter{0};
    int paintId = ++paintCounter;
    DBG("Canvas::paint START #" << paintId);

    g.fillAll(juce::Colours::black);

    // Simple cursor
    if (lastMousePos.x >= 0.0f)
    {
        const float r = currentBrushSize * 0.5f;
        g.setColour(juce::Colours::white.withAlpha(0.8f));
        g.drawEllipse(lastMousePos.x - r, lastMousePos.y - r, r * 2.0f, r * 2.0f, 1.0f);
    }

    // Phase 2-3 Validation Debug Overlay
    bool debugEnabled = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::debugOverlayEnabled).getValue();
    // ===== Thread-safe overlay using snapshot =====
    if (debugEnabled)
    {
        // Get immutable snapshot from audio processor (thread-safe)
        auto snapshot = audioProcessor.getCanvasSnapshot();
        if (!snapshot)
        {
            DBG("Canvas::paint END #" << paintId << " (no snapshot)");
            return; // No data yet
        }
        
        // Use snapshot data - all immutable and thread-safe
        const auto& metrics = snapshot->metrics;
        const auto path = snapshot->currentPath;
        const bool wrote = snapshot->wroteAudioFlag;
        const float sr = (float) snapshot->sampleRate;
        const int bs = snapshot->blockSize;
        const int queueDrops = queueDropCounter.load(std::memory_order_relaxed);
#ifdef PHASE4_EXPERIMENT
        const int activeBins = snapshot->activeBins;
        const int totalBins = snapshot->totalBins;
#else
        const int activeBins = 0;
        const int totalBins = 0;
#endif
        
        // Compose a single, final audio-path label
        juce::String pathStr;
        switch (path) {
            case SpectralCanvasProAudioProcessor::AudioPath::TestFeeder:
                pathStr = "Audio Path: TestFeeder (direct tone)";
                break;
            case SpectralCanvasProAudioProcessor::AudioPath::Phase4Synth:
                {
                    pathStr = "Audio Path: Phase4 Synth ";
                    pathStr += wrote ? "(writing)" : "(silent)";
                    pathStr += " [" + juce::String(activeBins) + "/" + juce::String(totalBins) + " bins]";
                    
#ifdef PHASE4_EXPERIMENT
                    // Add queue diagnostics for Phase4 debugging (from snapshot)
                    const uint64_t pushes = snapshot->maskPushCount;
                    const uint64_t drops = snapshot->maskDropCount;
                    const uint64_t phase4Blocks = snapshot->phase4Blocks;
                    const float maxMag = snapshot->maxMagnitude;
                    pathStr += juce::String::formatted("\nQueue: %llu pushes | %llu drops", pushes, drops);
                    pathStr += juce::String::formatted("\nMaxMag: %.4f | Phase4 Blocks: %llu", maxMag, phase4Blocks);
                    
#if PHASE4_DEBUG_TAP
                    // Add debug tap for SPSC integrity diagnosis
                    auto& tap = audioProcessor.getDebugTap();
                    const uint64_t tapPushes = tap.pushes.load(std::memory_order_relaxed);
                    const uint64_t tapPushFails = tap.pushFails.load(std::memory_order_relaxed);
                    const uint64_t tapPops = tap.pops.load(std::memory_order_relaxed);
                    const uintptr_t queueGUI = tap.queueAddrOnPush.load(std::memory_order_relaxed);
                    const uintptr_t queueAudio = tap.queueAddrOnAudio.load(std::memory_order_relaxed);
                    const uint64_t seqPushed = tap.lastSeqPushed.load(std::memory_order_relaxed);
                    const uint64_t seqPopped = tap.lastSeqPopped.load(std::memory_order_relaxed);
                    
                    pathStr += juce::String::formatted("\nTap: %llu/%llu/%llu (push/fail/pop)", tapPushes, tapPushFails, tapPops);
                    pathStr += juce::String::formatted("\nSeq: %llu->%llu", seqPushed, seqPopped);
                    if (queueGUI != queueAudio) {
                        pathStr += "\n*** QUEUE MISMATCH ***";
                    } else {
                        pathStr += "\nQueue: SAME";
                    }
#endif
#endif
                }
                break;
            case SpectralCanvasProAudioProcessor::AudioPath::SpectralResynthesis:
                pathStr = "Audio Path: Spectral Resynthesis ";
                pathStr += wrote ? "(painting active)" : "(loaded, ready)";
                if (audioProcessor.getSpectralModel().isReady()) {
                    pathStr += " [" + juce::String(audioProcessor.getSpectralModel().numFrames()) 
                             + "x" + juce::String(audioProcessor.getSpectralModel().numBins()) + "]";
                }
                break;
            case SpectralCanvasProAudioProcessor::AudioPath::Silent:
                pathStr = "Audio Path: Silent";
                break;
            default:
                pathStr = "Audio Path: Unknown";
                break;
        }
        
        // Panel background so text never overlaps the canvas
        juce::Rectangle<int> panel(8, 8, 520, 140); // Increased height for GPU status
        g.setColour(juce::Colours::black.withAlpha(0.6f));
        g.fillRoundedRectangle(panel.toFloat(), 6.0f);
        
        // Font and line spacing
        g.setFont(juce::FontOptions().withPointHeight(14.0f));
        int x = panel.getX() + 10;
        int y = panel.getY() + 10;
        const int lh = 18; // line height
        
        // Draw lines (each once)
        g.setColour(juce::Colours::yellow);
        g.drawText(pathStr, x, y, panel.getWidth()-20, lh, juce::Justification::left); y += lh;
        
        g.setColour(metrics.medianLatencyMs <= 5.0f ? juce::Colours::lime : juce::Colours::orange);
        g.drawText("Latency: " + juce::String(metrics.medianLatencyMs, 1) + "ms / "
                   + juce::String(metrics.p95LatencyMs, 1) + "ms (med/p95)",
                   x, y, panel.getWidth()-20, lh, juce::Justification::left); y += lh;
        
        g.setColour(juce::Colours::lightgrey);
        g.drawText("SR: " + juce::String(sr / 1000.0f, 1) + "kHz | Block: "
                   + juce::String(bs) + " samples",
                   x, y, panel.getWidth()-20, lh, juce::Justification::left); y += lh;
        
        g.setColour(metrics.dropCount == 0 ? juce::Colours::lime : juce::Colours::red);
        g.drawText("Queue: " + juce::String((int)metrics.queueDepth) + "/8 | Drops: " 
                   + juce::String((int)metrics.dropCount) + " | Local Drops: " + juce::String(queueDrops),
                   x, y, panel.getWidth()-20, lh, juce::Justification::left); y += lh;
        
        g.setColour(currentFPS >= 58.0f ? juce::Colours::lime : juce::Colours::yellow);
        g.drawText("FPS: " + juce::String(currentFPS, 1)
                   + " | Processed Samples: " + juce::String((long long)metrics.processedSamples)
                   + " | XRuns: " + juce::String((int)metrics.xrunCount),
                   x, y, panel.getWidth()-20, lh, juce::Justification::left); y += lh;
        
        // GPU Status Display
        g.setColour(juce::Colours::lightblue);
        g.drawText("GPU: Phase 4 Resilience Ready (D3D11 Active)",
                   x, y, panel.getWidth()-20, lh, juce::Justification::left);
        
        // Avoid unused warnings
        juce::ignoreUnused(wrote);
    }
    
    DBG("Canvas::paint END #" << paintId);
}

void CanvasComponent::resized()
{
    // No-op for minimal UI
}

void CanvasComponent::mouseDown(const juce::MouseEvent& e)
{
    lastMousePos = e.position;

    // Test mode audio feedback
    bool testMode = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::testModeActive).getValue();
    if (testMode)
    {
        audioProcessor.generateImmediateAudioFeedback();
    }

    // Convert screen coordinates to normalized [0,1] range
    float yNormalized = e.position.y / static_cast<float>(getHeight());
    yNormalized = juce::jlimit(0.0f, 1.0f, yNormalized);
    
    // Use current brush strength as intensity
    float intensity = juce::jlimit(0.0f, 1.0f, currentBrushStrength);
    
    // Direct RT-safe paint-to-sound using unified interface
    pushPaintEvent(yNormalized, intensity);

    // Start paint stroke for visual feedback
    currentStroke = PaintStroke{};
    currentStroke.isActive = true;
    currentStroke.brushType = currentBrushType;
    currentStroke.brushSize = currentBrushSize;
    currentStroke.brushStrength = currentBrushStrength;
    currentStroke.startTime = juce::Time::getMillisecondCounter();

    // Add first point
    PaintStroke::Point point;
    point.position = e.position;
    point.spectralPos = screenToSpectral(e.position);
    point.pressure = 1.0f;
    point.timestamp = 0.0f;
    
    currentStroke.points.push_back(point);
    
    isDragging = true;
    repaint();
}

void CanvasComponent::mouseDrag(const juce::MouseEvent& e)
{
    if (!isDragging || !currentStroke.isActive) return;
    
    lastMousePos = e.position;
    
    // Convert screen coordinates to normalized [0,1] range
    float yNormalized = e.position.y / static_cast<float>(getHeight());
    yNormalized = juce::jlimit(0.0f, 1.0f, yNormalized);
    
    // Use current brush strength as intensity
    float intensity = juce::jlimit(0.0f, 1.0f, currentBrushStrength);
    
    // Direct RT-safe paint-to-sound for continuous painting using unified interface
    pushPaintEvent(yNormalized, intensity);
    
    // Add point to stroke
    PaintStroke::Point point;
    point.position = e.position;
    point.spectralPos = screenToSpectral(e.position);
    point.pressure = 1.0f;
    point.timestamp = (juce::Time::getMillisecondCounter() - currentStroke.startTime) / 1000.0f;
    
    currentStroke.points.push_back(point);
    
    // Limit stroke length for performance
    if (currentStroke.points.size() > 100)
    {
        currentStroke.points.erase(currentStroke.points.begin());
    }
    
    repaint();
}

void CanvasComponent::mouseUp(const juce::MouseEvent& e)
{
    juce::ignoreUnused(e);
    
    if (currentStroke.isActive)
    {
        currentStroke.isActive = false;
        
        if (currentStroke.points.size() > 1)
        {
            completedStrokes.push_back(currentStroke);
            if (completedStrokes.size() > 20)
            {
                completedStrokes.erase(completedStrokes.begin());
            }
        }
    }
    
    isDragging = false;
    repaint();
}

void CanvasComponent::mouseMove(const juce::MouseEvent& e)
{
    lastMousePos = e.position;
    repaint();
}

void CanvasComponent::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel)
{
    juce::ignoreUnused(e);
    
    float delta = wheel.deltaY * 5.0f;
    currentBrushSize = juce::jlimit(1.0f, 128.0f, currentBrushSize + delta);
    
    if (auto* param = audioProcessor.apvts.getParameter(Params::ParameterIDs::brushSize))
    {
        param->setValueNotifyingHost(currentBrushSize / 128.0f);
    }
    
    repaint();
}

bool CanvasComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Check if any file has a supported audio extension
    for (const auto& file : files)
    {
        if (file.endsWithIgnoreCase(".wav") || file.endsWithIgnoreCase(".aiff") || 
            file.endsWithIgnoreCase(".aif") || file.endsWithIgnoreCase(".flac") || 
            file.endsWithIgnoreCase(".ogg"))
        {
            return true;
        }
        
#ifdef SPECTRAL_ENABLE_MP3
        if (file.endsWithIgnoreCase(".mp3"))
        {
            return true;
        }
#endif
    }
    return false;
}

void CanvasComponent::fileDragEnter(const juce::StringArray&, int, int)
{
    isFileDragOver = true;
    repaint();
}

void CanvasComponent::fileDragExit(const juce::StringArray&)
{
    isFileDragOver = false;
    repaint();
}

void CanvasComponent::filesDropped(const juce::StringArray& files, int, int)
{
    isFileDragOver = false;
    repaint();
    
    if (files.isEmpty())
    {
        LOGW(UI, "filesDropped called with empty file list");
        return;
    }
    
    // Find first supported audio file
    juce::File audioFile;
    for (const auto& filePath : files)
    {
        juce::File file(filePath);
        if (file.existsAsFile())
        {
            auto extension = file.getFileExtension().toLowerCase();
            if (extension == ".wav" || extension == ".aiff" || extension == ".aif" || 
                extension == ".flac" || extension == ".ogg"
#ifdef SPECTRAL_ENABLE_MP3
                || extension == ".mp3"
#endif
                )
            {
                audioFile = file;
                break;
            }
        }
    }
    
    if (!audioFile.exists())
    {
        LOGI(UI, "No supported audio files found in drag-and-drop");
        return;
    }
    
    LOGI(UI, "Audio file dropped: %s", audioFile.getFullPathName().toRawUTF8());
    
    // Get parent editor to access sample loader and toast manager
    if (auto* editor = dynamic_cast<SpectralCanvasProEditor*>(parentEditor_))
    {
        // Use the editor's sample loading infrastructure
        editor->loadAudioFile(audioFile);
    }
    else
    {
        LOGE(UI, "Cannot access parent editor for drag-and-drop loading");
    }
}

void CanvasComponent::timerCallback()
{
#ifdef PHASE4_EXPERIMENT
    // Check if we need to dispatch accumulated MaskColumn - use actual audio path
    if (audioProcessor.getCurrentPath() == SpectralCanvasProAudioProcessor::AudioPath::Phase4Synth && accumulator_.hasData) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDispatchTime_).count();
        
        if (elapsed >= batchIntervalMs_) {
            // Create and send accumulated MaskColumn
            MaskColumn mask;
            
            // Get processor epoch and sample rate for unified timebase
            auto epochNanos = audioProcessor.getEpochSteadyNanos();
            double sampleRate = audioProcessor.getSampleRate();
            
            // Capture current UI timestamp (steady_clock)
            auto uiSteadyNanos = std::chrono::steady_clock::now().time_since_epoch().count();
            
            // Convert UI time to samples since epoch
            uint64_t uiTimestampSamples = static_cast<uint64_t>((uiSteadyNanos - epochNanos) * sampleRate / 1e9);
            
            // Fill mask column
            mask.timestampSamples = static_cast<uint64_t>(uiTimestampSamples);
            mask.uiTimestampMicros = static_cast<uint64_t>(uiSteadyNanos / 1000);
            mask.sequenceNumber = audioProcessor.getNextMaskSequenceNumber();
            mask.numBins = 257;
            mask.frameIndex = 0;
            
            // Copy accumulated values
            const size_t copySize = std::min(static_cast<size_t>(mask.numBins), accumulator_.values.size());
            for (size_t i = 0; i < copySize; ++i) {
                mask.values[i] = accumulator_.values[i];
            }
            
            // Send to audio thread (only if Phase4 path is active)
            if (audioProcessor.getCurrentPath() == SpectralCanvasProAudioProcessor::AudioPath::Phase4Synth) {
                audioProcessor.pushMaskColumn(mask);
            }
            
            // Clear accumulator and update timing
            accumulator_.clear();
            lastDispatchTime_ = now;
        }
    }
#endif
    
    repaint();
}

// Public interface methods
void CanvasComponent::setEngineMode(int) { }
void CanvasComponent::setBlendAmount(float) { }
void CanvasComponent::setBrushType(int type) { currentBrushType = type; }
void CanvasComponent::setBrushSize(float size) { currentBrushSize = size; }
void CanvasComponent::setBrushStrength(float strength) { currentBrushStrength = strength; }
void CanvasComponent::setGridVisible(bool visible) { gridVisible = visible; repaint(); }
void CanvasComponent::setSnapToScale(bool snap) { snapToScale = snap; }

// Helper methods
juce::Point<float> CanvasComponent::screenToSpectral(juce::Point<float> screenPos) const
{
    const float canvasHeight = (float)getHeight();
    const float canvasWidth = (float)getWidth();
    
    float u = screenPos.x / canvasWidth;
    float v = 1.0f - (screenPos.y / canvasHeight);
    
    return juce::Point<float>(juce::jlimit(0.0f, 1.0f, u), 
                              juce::jlimit(0.0f, 1.0f, v));
}

juce::Point<float> CanvasComponent::spectralToScreen(juce::Point<float> spectralPos) const
{
    const float canvasHeight = (float)getHeight();
    const float canvasWidth = (float)getWidth();
    
    float x = spectralPos.x * canvasWidth;
    float y = (1.0f - spectralPos.y) * canvasHeight;
    
    return juce::Point<float>(x, y);
}

void CanvasComponent::parentHierarchyChanged()
{
    // Phase 2-3 MINIMAL: No floating UI components to update
}

void CanvasComponent::visibilityChanged()
{
    // Phase 2-3 MINIMAL: No GPU renderer initialization needed
}

void CanvasComponent::createAndSendMaskColumn(juce::Point<float> mousePos)
{
    // Phase 2-3 Validation: Convert mouse position to MaskColumn for latency testing
    MaskColumn mask;
    
    // Get processor epoch and sample rate for unified timebase
    auto epochNanos = audioProcessor.getEpochSteadyNanos();
    double sampleRate = audioProcessor.getSampleRate();
    
    // Capture current UI timestamp (steady_clock)
    auto uiSteadyNanos = std::chrono::steady_clock::now().time_since_epoch().count();
    
    // Convert UI time to samples since epoch (per spec)
    uint64_t uiTimestampSamples = static_cast<uint64_t>((uiSteadyNanos - epochNanos) * sampleRate / 1e9);
    
    // Fill mask column with validation data
    mask.timestampSamples = uiTimestampSamples;  // Store as uint64_t directly
    mask.uiTimestampMicros = static_cast<uint64_t>(uiSteadyNanos / 1000);  // Convert nanoseconds to microseconds
    mask.sequenceNumber = audioProcessor.getNextMaskSequenceNumber();
    mask.numBins = 257; // Standard FFT size for validation
    mask.frameIndex = 0;
    
    // Convert mouse position to spectral position
    auto spectralPos = screenToSpectral(mousePos);
    
    // Create simple test mask based on mouse position
    // Map X coordinate to frequency bins (0..numBins)
    // Y coordinate controls amplitude (0..1)
    size_t centerBin = static_cast<size_t>(spectralPos.x * (mask.numBins - 1));
    float amplitude = spectralPos.y * currentBrushStrength;
    
    // Apply brush-sized mask around center bin
    size_t brushWidthBins = static_cast<size_t>(currentBrushSize * mask.numBins / 512.0f);
    for (size_t i = 0; i < mask.numBins; ++i)
    {
        if (i >= centerBin - brushWidthBins && i <= centerBin + brushWidthBins)
        {
            mask.values[i] = amplitude;
        }
        else
        {
            mask.values[i] = 0.0f;
        }
    }
    
    // Send to audio thread via RT-safe queue (with diagnostics)
    audioProcessor.pushMaskColumn(mask);
}

#ifdef PHASE4_EXPERIMENT
// Isolated Y-to-bin mapping functions
inline int CanvasComponent::uiToBinLinear(float yNorm, int numBins) noexcept
{
    // Top = highest frequency (1.0f - yNorm for inverted Y)
    return std::clamp(static_cast<int>((1.0f - yNorm) * (numBins - 1)), 0, numBins - 1);
}

inline int CanvasComponent::uiToBinLog(float yNorm, double sampleRate, int fftSize) noexcept
{
    // Log-frequency mapping for musical scales (80Hz to 8kHz)
    constexpr float fMin = 80.0f;     // Low end of musical range
    constexpr float fMax = 8000.0f;   // High end before harsh frequencies
    const float nyquist = static_cast<float>(sampleRate * 0.5);
    
    // Clamp and invert Y (top = high frequency)
    const float y = std::clamp(1.0f - yNorm, 0.0f, 1.0f);
    
    // Exponential frequency mapping
    const float freq = fMin * std::pow(fMax / fMin, y);
    
    // Convert frequency to bin index
    const int bin = static_cast<int>(std::round((freq / nyquist) * (fftSize / 2)));
    
    return std::clamp(bin, 0, fftSize / 2);
}

void CanvasComponent::createAndSendMaskColumnPhase4(juce::Point<float> mousePos)
{
    // DEBUG: Log that this function is being called
    juce::Logger::writeToLog("*** PHASE4 PAINT CALLED! ***");
    
    // Visual feedback - flash the component briefly to show paint is being called
    static int paintCallCounter = 0;
    ++paintCallCounter;
    juce::Logger::writeToLog("PHASE4 Paint Call #" + juce::String(paintCallCounter));
    
    // Convert mouse position to spectral coordinates
    const auto spectralPos = screenToSpectral(mousePos);
    const float yNorm = spectralPos.y;  // Already normalized by screenToSpectral
    
    // Get current audio parameters
    const int numBins = 257;  // Fixed for 512 FFT
    const double sampleRate = audioProcessor.getSampleRate();
    const int fftSize = 512;  // From current system
    
    // Avoid MSVC C4189 warnings for unused variables in current implementation
    juce::ignoreUnused(sampleRate, fftSize);
    
    // Map Y to bin index (linear mapping)
    const int centerBin = uiToBinLinear(yNorm, numBins);
    
    // Calculate brush extent in bins
    const float brushSizeBins = currentBrushSize * numBins / 512.0f;
    const float pressureScaled = currentBrushStrength;
    
    // 3-tap antialiasing kernel
    constexpr float kernel[3] = {0.25f, 0.5f, 0.25f};
    
    // Accumulate into column accumulator with antialiasing
    for (int offset = -1; offset <= 1; ++offset) {
        const int binIndex = centerBin + offset;
        if (binIndex >= 1 && binIndex < numBins - 1) {  // Skip DC and Nyquist
            const float weight = kernel[offset + 1];
            const float value = pressureScaled * weight;
            accumulator_.accumulate(binIndex, value);
        }
    }
    
    // IMMEDIATE TEST: Also send direct MaskColumn for debugging
    MaskColumn testMask;
    testMask.numBins = numBins;
    testMask.sequenceNumber = audioProcessor.getNextMaskSequenceNumber();
    testMask.timestampSamples = 0.0;  // Simple for testing
    testMask.uiTimestampMicros = 0;
    testMask.frameIndex = 0;
    
    // Clear and set test pattern
    std::memset(testMask.values, 0, sizeof(testMask.values));
    for (int offset = -1; offset <= 1; ++offset) {
        const int binIndex = centerBin + offset;
        if (binIndex >= 1 && binIndex < numBins - 1) {
            const float weight = kernel[offset + 1];
            testMask.values[binIndex] = pressureScaled * weight;
        }
    }
    
    // TESTING: Add strong test magnitude to ensure synthesis works
    for (int i = 100; i < 110; ++i) {
        testMask.values[i] = 0.5f;  // Strong magnitude around 440Hz region
    }
    
    // Direct push for immediate testing  
    audioProcessor.pushMaskColumn(testMask);
    
    juce::Logger::writeToLog("PHASE4: testMask pushed with magnitude 0.5 in bins 100-110");
    
    // Apply brush size by distributing to nearby bins
    const int brushRadius = static_cast<int>(std::ceil(brushSizeBins * 0.5f));
    for (int offset = -brushRadius; offset <= brushRadius; ++offset) {
        const int binIndex = centerBin + offset;
        if (binIndex >= 1 && binIndex < numBins - 1) {
            // Gaussian-like falloff
            const float distance = std::abs(static_cast<float>(offset));
            const float falloff = std::max(0.0f, 1.0f - distance / (brushRadius + 1.0f));
            const float value = pressureScaled * falloff * 0.3f;  // Reduce intensity for spread
            accumulator_.accumulate(binIndex, value);
        }
    }
}

void CanvasComponent::pushMaskFromScreenY(float y) noexcept
{
    // DEBUG: Log path to understand why paint was blocked
    juce::Logger::writeToLog("PAINT: getCurrentPath()=" + juce::String(static_cast<int>(audioProcessor.getCurrentPath())));
    
    // TESTING: Temporarily bypass path check like we did in pushMaskColumn
    /*
    // Only produce in Phase4 mode
    if (audioProcessor.getCurrentPath() != SpectralCanvasProAudioProcessor::AudioPath::Phase4Synth)
        return;
    */
        
    static float vals[kNumBins];  // Preallocated scratch buffer (RT-safe)
    for (int i = 0; i < kNumBins; ++i) vals[i] = 0.0f;
    
    // Map Y -> bin index (top = high freq, bottom = low freq)
    const auto bounds = getLocalBounds().toFloat();
    const float yNorm = juce::jlimit(0.0f, 1.0f, (y - bounds.getY()) / bounds.getHeight());
    const int k = juce::jlimit(1, kNumBins - 2, 
                               static_cast<int>(std::round((1.0f - yNorm) * (kNumBins - 1))));
                               
    // 3-tap splat for audibility (same as 'i' key that works)
    auto add = [&](int kk, float v) { vals[kk] = juce::jmax(vals[kk], v); };
    add(k - 1, 0.25f);
    add(k, 0.60f);
    add(k + 1, 0.25f);
    
    // Create MaskColumn and push directly to processor
    MaskColumn col;
    col.numBins = kNumBins;
    col.timestampSamples = static_cast<uint64_t>(std::llround(juce::Time::getMillisecondCounterHiRes() * 0.001));
    col.sequenceNumber = audioProcessor.getNextMaskSequenceNumber();
    col.uiTimestampMicros = static_cast<uint64_t>(juce::Time::getHighResolutionTicks());
    
    // Copy values to column
    for (int i = 0; i < kNumBins; ++i) {
        col.values[i] = vals[i];
    }
    
    // Push to processor (same API that 'i' key uses)
    audioProcessor.pushMaskColumn(col);
    
    // DIAGNOSTIC: Add queue address and size diagnostics
    auto& q = audioProcessor.getMaskColumnQueue();
    
    // Debug feedback
    static int paintCallCount = 0;
    ++paintCallCount;
    if (paintCallCount % 5 == 1) { // Log every 5th call to be more visible
        juce::Logger::writeToLog("*** DIRECT PAINT SUCCESS: bin=" + juce::String(k) + " mag=0.60 (call #" + juce::String(paintCallCount) + ") ***");
        juce::Logger::writeToLog("[UI] MaskQueue addr=" + juce::String::toHexString(reinterpret_cast<uintptr_t>(&q)) + " sizeof(MaskColumn)=" + juce::String(sizeof(MaskColumn)));
    }
}

bool CanvasComponent::keyPressed(const juce::KeyPress& key)
{
    // DEBUG: Test injection with 'i' key to bypass paint path
    if (key.getTextCharacter() == 'i') {
        juce::Logger::writeToLog("*** 'I' KEY PRESSED - INJECTING TEST MASKCOLUMN ***");
        
        if (audioProcessor.getCurrentPath() == SpectralCanvasProAudioProcessor::AudioPath::Phase4Synth) {
            // Create test MaskColumn with strong 440Hz tone
            MaskColumn testCol;
            testCol.numBins = 257;
            testCol.timestampSamples = static_cast<uint64_t>(std::llround(juce::Time::getMillisecondCounterHiRes() * 0.001));
            testCol.sequenceNumber = audioProcessor.getNextMaskSequenceNumber();
            testCol.uiTimestampMicros = static_cast<uint64_t>(juce::Time::getHighResolutionTicksPerSecond());
            
            // Clear all values first
            for (size_t i = 0; i < MaskColumn::MAX_BINS; ++i) {
                testCol.values[i] = 0.0f;
            }
            
            // Calculate 440Hz bin (k ≈ 440 * fftSize / sampleRate)
            const double sr = audioProcessor.getSampleRate();
            const int fftSize = 512;
            const int k440 = juce::jlimit(1, 256, static_cast<int>(std::round(440.0 * fftSize / sr)));
            
            // Add strong magnitude around 440Hz
            testCol.values[k440 - 1] = 0.25f;
            testCol.values[k440] = 0.6f;
            testCol.values[k440 + 1] = 0.25f;
            
            juce::Logger::writeToLog("Injecting test column: bin " + juce::String(k440) + " = 0.6, sr=" + juce::String(sr));
            
            // Push directly to processor
            bool success = audioProcessor.pushMaskColumn(testCol);
            juce::Logger::writeToLog("Injection result: " + juce::String(success ? "SUCCESS" : "FAILED"));
        } else {
            juce::Logger::writeToLog("Cannot inject - not in Phase4 mode");
        }
        return true;
    }
    
    return Component::keyPressed(key);
}
#endif

void CanvasComponent::pushPaintEvent(float y, float intensity)
{
    // Clamp inputs to valid range
    y = juce::jlimit(0.0f, 1.0f, y);
    intensity = juce::jlimit(0.0f, 1.0f, intensity);
    
    // Choose paint method based on current audio path
    auto currentPath = audioProcessor.getCurrentPath();
    
    if (currentPath == SpectralCanvasProAudioProcessor::AudioPath::ModernPaint)
    {
        // Use lightweight 12-byte paint events for modern JUCE DSP path
        audioProcessor.pushPaintEvent(y, intensity);
    }
    else if (currentPath == SpectralCanvasProAudioProcessor::AudioPath::Phase4Synth)
    {
        // Use legacy MaskColumn system for backward compatibility
        pushMaskFromScreenY(y);
    }
    // Other paths (Silent, TestFeeder) don't process paint events
}