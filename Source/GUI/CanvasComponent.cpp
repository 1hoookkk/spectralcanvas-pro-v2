#include "CanvasComponent.h"
#include "../Core/Params.h"
#include <chrono>

CanvasComponent::CanvasComponent(SpectralCanvasProAudioProcessor& processor)
    : audioProcessor(processor)
{
    setOpaque(true);
    startTimer(16); // 60fps
    
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
    if (debugEnabled)
    {
        auto metrics = audioProcessor.getPerformanceMetrics();
        
        g.setColour(juce::Colours::lime);
        g.setFont(juce::FontOptions(10.0f, juce::Font::bold));
        
        // Title and basic status
        g.drawText("Phase 2-3 Validation Metrics", 8, 8, 400, 14, juce::Justification::left);
        
        bool useTestFeeder = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::useTestFeeder).getValue();
        g.drawText(juce::String::formatted("Test Mode: %s", useTestFeeder ? "TestFeeder" : "Phase4"), 
                   8, 22, 200, 14, juce::Justification::left);
        
        // Show active audio path and write status
        auto path = audioProcessor.getCurrentPath();
        const char* pathStr = (path == SpectralCanvasProAudioProcessor::AudioPath::TestFeeder) ? "TestFeeder" :
                              (path == SpectralCanvasProAudioProcessor::AudioPath::Phase4Synth) ? "Phase4 Synth" :
                              (path == SpectralCanvasProAudioProcessor::AudioPath::Fallback) ? "Fallback" : "None";
        
        const bool wrote = audioProcessor.getWroteAudioFlag();
        
        g.setColour(juce::Colours::yellow);
        g.drawText(juce::String("Audio Path: ") + pathStr + (wrote ? "  (writing)" : "  (silent)"),
                   8, 50, 400, 14, juce::Justification::left);
        
        // Latency metrics  
        g.setColour(metrics.medianLatencyMs <= 5.0f ? juce::Colours::lime : juce::Colours::orange);
        g.drawText(juce::String::formatted("Latency: %.1fms / %.1fms (med/p95)", 
                   metrics.medianLatencyMs, metrics.p95LatencyMs),
                   8, 64, 400, 14, juce::Justification::left);
        
        // Queue and performance stats
        g.setColour(metrics.dropCount == 0 ? juce::Colours::lime : juce::Colours::red);
        g.drawText(juce::String::formatted("Queue: %zu/%d | Drops: %zu | Local Drops: %d",
                   metrics.queueDepth, 8, metrics.dropCount, 
                   queueDropCounter.load(std::memory_order_relaxed)),
                   8, 78, 400, 14, juce::Justification::left);
        
        // Frame rate and audio stats
        g.setColour(currentFPS >= 58.0f ? juce::Colours::lime : juce::Colours::yellow);
        g.drawText(juce::String::formatted("FPS: %.1f | Processed Samples: %llu | XRuns: %zu",
                   currentFPS, metrics.processedSamples, metrics.xrunCount),
                   8, 92, 400, 14, juce::Justification::left);
        
        // Sample rate and block size
        g.setColour(juce::Colours::lightgrey);
        g.drawText(juce::String::formatted("SR: %.1fkHz | Block: %d samples",
                   audioProcessor.getSampleRate() / 1000.0, audioProcessor.getBlockSize()),
                   8, 78, 400, 14, juce::Justification::left);
    }
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

#ifdef PHASE4_EXPERIMENT
    bool useTestFeeder = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::useTestFeeder).getValue();
    if (!useTestFeeder) {
        createAndSendMaskColumnPhase4(e.position);
    } else {
#endif
        // Phase 2-3 Validation: Generate MaskColumn with proper timestamping
        createAndSendMaskColumn(e.position);
#ifdef PHASE4_EXPERIMENT
    }
#endif

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
    
#ifdef PHASE4_EXPERIMENT
    bool useTestFeeder = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::useTestFeeder).getValue();
    if (!useTestFeeder) {
        createAndSendMaskColumnPhase4(e.position);
    } else {
#endif
        // Phase 2-3 Validation: Generate MaskColumn for continuous painting
        createAndSendMaskColumn(e.position);
#ifdef PHASE4_EXPERIMENT
    }
#endif
    
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
    for (const auto& file : files)
    {
        if (file.endsWithIgnoreCase(".wav") || file.endsWithIgnoreCase(".aiff"))
            return true;
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
    if (files.size() > 0)
    {
        juce::Logger::writeToLog("Loading: " + files[0]);
    }
    repaint();
}

void CanvasComponent::timerCallback()
{
#ifdef PHASE4_EXPERIMENT
    // Check if we need to dispatch accumulated MaskColumn
    bool useTestFeeder = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::useTestFeeder).getValue();
    
    if (!useTestFeeder && accumulator_.hasData) {
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
            mask.timestampSamples = static_cast<double>(uiTimestampSamples);
            mask.uiTimestampMicros = uiSteadyNanos / 1000;
            mask.sequenceNumber = audioProcessor.getNextMaskSequenceNumber();
            mask.numBins = 257;
            mask.frameIndex = 0;
            
            // Copy accumulated values
            const size_t copySize = std::min(static_cast<size_t>(mask.numBins), accumulator_.values.size());
            for (size_t i = 0; i < copySize; ++i) {
                mask.values[i] = accumulator_.values[i];
            }
            
            // Send to audio thread
            audioProcessor.pushMaskColumn(mask);
            
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
    mask.timestampSamples = static_cast<double>(uiTimestampSamples);  // Store as double for compatibility
    mask.uiTimestampMicros = uiSteadyNanos / 1000;                    // Keep microseconds for backward compat
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
    
    // Send to audio thread via RT-safe queue
    if (!audioProcessor.getMaskColumnQueue().push(mask))
    {
        // Queue full - increment drop counter for debug overlay
        queueDropCounter.fetch_add(1, std::memory_order_relaxed);
    }
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
    // Future: Log-frequency mapping for musical scales
    // For now, fallback to linear
    const int numBins = fftSize / 2 + 1;
    return uiToBinLinear(yNorm, numBins);
}

void CanvasComponent::createAndSendMaskColumnPhase4(juce::Point<float> mousePos)
{
    // Convert mouse position to spectral coordinates
    const auto spectralPos = screenToSpectral(mousePos);
    const float yNorm = spectralPos.y;  // Already normalized by screenToSpectral
    
    // Get current audio parameters
    const int numBins = 257;  // Fixed for 512 FFT
    const double sampleRate = audioProcessor.getSampleRate();
    const int fftSize = 512;  // From current system
    
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
#endif