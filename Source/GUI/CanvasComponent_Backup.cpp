#include "CanvasComponent.h"
#include "TopStrip.h"
#include "BottomBar.h"
#include "../Viz/backends/D3D11Renderer.h"

CanvasComponent::CanvasComponent(SpectralCanvasProAudioProcessor& processor)
    : audioProcessor(processor)
{
    // GPU renderer will be initialized lazily in visibilityChanged()
    juce::Logger::writeToLog("CanvasComponent constructor started");
    
    // Phase 2-3 MINIMAL: No floating UI components
    // topStrip = std::make_unique<TopStrip>(audioProcessor, *this);
    // bottomBar = std::make_unique<BottomBar>(audioProcessor, *this);
    
    // addAndMakeVisible(*topStrip);
    // addAndMakeVisible(*bottomBar);
    
    // Start 60fps timer for smooth nebula animation
    startTimer(16); // ~60fps (16.67ms)
    
    juce::Logger::writeToLog("CanvasComponent constructor completed");
    
    // Initialize stroke system
    currentStroke.isActive = false;
    completedStrokes.reserve(100); // Preallocate for performance
}

CanvasComponent::~CanvasComponent()
{
    stopTimer();
    // Phase 2-3: No GPU renderer
    // gpuRenderer.reset();
}

void CanvasComponent::paint(juce::Graphics& g)
{
    // Phase 2-3 MINIMAL: Simple dark background only
    g.fillAll(juce::Colour(0xff0a0a0f)); // Dark background
    
    // Draw simple brush cursor
    if (isMouseOverOrDragging())
    {
        const float x = lastMousePos.x;
        const float y = lastMousePos.y;
        const float radius = currentBrushSize * 0.5f;
        
        g.setColour(juce::Colours::white.withAlpha(0.8f));
        g.drawEllipse(x - radius, y - radius, radius * 2, radius * 2, 1.0f);
    }
    
    // Debug overlay if enabled
    bool debugEnabled = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::debugOverlayEnabled).getValue();
    if (debugEnabled)
    {
        g.setColour(juce::Colours::lime);
        g.setFont(juce::FontOptions(12.0f));
        
        int yPos = 50;
        g.drawText("Phase 2-3 Minimal UI Active", 8, yPos, 300, 14, juce::Justification::left);
        yPos += 14;
        
        bool testActive = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::testModeActive).getValue();
        g.drawText(juce::String::formatted("Test Mode: %s", testActive ? "ACTIVE" : "OFF"), 
                   8, yPos, 300, 14, juce::Justification::left);
    }
}

void CanvasComponent::paintSoftwareFallback(juce::Graphics& g)
{
    // Create living nebula background (purple → cyan)
    const int width = getWidth();
    const int height = getHeight();
    
    // Base nebula gradient
    juce::ColourGradient gradient(
        juce::Colour(0xff4a148c),  // Deep purple
        0.0f, height * 0.8f,
        juce::Colour(0xff00bcd4),  // Cyan
        0.0f, height * 0.2f,
        false
    );
    gradient.addColour(0.3, juce::Colour(0xff7b1fa2)); // Purple mid
    gradient.addColour(0.7, juce::Colour(0xff26c6da)); // Light cyan
    
    g.setGradientFill(gradient);
    g.fillAll();
    
    // Add animated spectral bars (if we have spectral data)
    auto& spectralQueue = audioProcessor.getSpectralDataQueue();
    if (spectralQueue.hasDataAvailable())
    {
        // Process latest spectral frame
        while (auto frame = spectralQueue.pop())
        {
            // Draw spectral bins as nebula texture
            const int numBins = SpectralFrame::NUM_BINS;
            const float binWidth = (float)width / numBins;
            
            for (int bin = 0; bin < numBins; ++bin)
            {
                float magnitude = frame->magnitude[bin];
                float frequency = (float)bin / numBins;
                
                // Convert magnitude to visual intensity
                float dbMagnitude = 20.0f * std::log10(std::max(magnitude, 1e-6f));
                float brightness = juce::jlimit(0.0f, 1.0f, (dbMagnitude + 80.0f) / 80.0f);
                
                // Frequency-based color (purple low → cyan high)
                float hue = juce::jlimit(0.0f, 1.0f, 0.76f - frequency * 0.24f); // 0.76=purple, 0.52=cyan
                juce::Colour binColor = juce::Colour::fromHSV(hue, 0.9f, brightness * 0.8f, brightness * 0.6f);
                
                g.setColour(binColor);
                float x = bin * binWidth;
                float barHeight = brightness * height * 0.3f; // Max 30% of height
                g.fillRect(x, height - barHeight, binWidth, barHeight);
            }
        }
    }
    else
    {
        // No audio data - show gentle animated stars/particles
        static float starPhase = 0.0f;
        starPhase += 0.02f;
        
        g.setColour(juce::Colours::white.withAlpha(0.4f));
        for (int i = 0; i < 50; ++i)
        {
            float x = (i * 37) % width; // Pseudo-random distribution
            float y = (i * 73) % height;
            float twinkle = 0.5f + 0.3f * std::sin(starPhase + i * 0.5f);
            
            g.setColour(juce::Colours::white.withAlpha(twinkle * 0.6f));
            g.fillEllipse(x - 1, y - 1, 2, 2);
        }
    }
    
    // Draw completed strokes as glowing trails
    for (const auto& stroke : completedStrokes)
    {
        if (stroke.points.size() < 2) continue;
        
        juce::Path strokePath;
        strokePath.startNewSubPath(stroke.points[0].position);
        for (size_t i = 1; i < stroke.points.size(); ++i)
        {
            strokePath.lineTo(stroke.points[i].position);
        }
        
        // Stroke glow effect
        g.setColour(getNebulaAccentColor().withAlpha(0.3f));
        g.strokePath(strokePath, juce::PathStrokeType(stroke.brushSize * 0.5f));
        
        g.setColour(getNebulaAccentColor().withAlpha(0.8f));
        g.strokePath(strokePath, juce::PathStrokeType(2.0f));
    }
    
    // Draw active stroke
    if (currentStroke.isActive && currentStroke.points.size() > 1)
    {
        juce::Path activePath;
        activePath.startNewSubPath(currentStroke.points[0].position);
        for (size_t i = 1; i < currentStroke.points.size(); ++i)
        {
            activePath.lineTo(currentStroke.points[i].position);
        }
        
        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.strokePath(activePath, juce::PathStrokeType(3.0f));
    }
}

void CanvasComponent::resized()
{
    // Phase 2-3 MINIMAL: No GPU renderer or floating components to resize
    // Canvas fills entire area, top strip handled by parent editor
}

void CanvasComponent::mouseDown(const juce::MouseEvent& e)
{
    // Phase 2-3: Test mode audio feedback
    bool testMode = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::testModeActive).getValue();
    if (testMode)
    {
        audioProcessor.generateImmediateAudioFeedback();
    }
    
    // Start new paint stroke
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
    point.pressure = e.mods.isLeftButtonDown() ? 1.0f : 0.5f;
    point.timestamp = 0.0f; // First point is time 0
    
    currentStroke.points.push_back(point);
    
    isDragging = true;
    lastMousePos = e.position;
    
    repaint();
}

void CanvasComponent::mouseDrag(const juce::MouseEvent& e)
{
    if (!isDragging || !currentStroke.isActive) return;
    
    // Add point to current stroke
    PaintStroke::Point point;
    point.position = e.position;
    point.spectralPos = screenToSpectral(e.position);
    point.pressure = e.mods.isLeftButtonDown() ? 1.0f : 0.5f;
    point.timestamp = (juce::Time::getMillisecondCounter() - currentStroke.startTime) / 1000.0f;
    
    currentStroke.points.push_back(point);
    
    // METASYNTH-STYLE: Generate immediate spectral feedback
    generateImmediateAudioFeedback(point);
    
    // Limit stroke length for performance
    if (currentStroke.points.size() > 200)
    {
        currentStroke.points.erase(currentStroke.points.begin());
        // Update timestamps
        float baseTime = currentStroke.points[0].timestamp;
        for (auto& p : currentStroke.points)
            p.timestamp -= baseTime;
    }
    
    lastMousePos = e.position;
    repaint();
}

void CanvasComponent::mouseUp(const juce::MouseEvent& e)
{
    juce::ignoreUnused(e);
    
    if (currentStroke.isActive)
    {
        // Finalize stroke and send to processor
        currentStroke.isActive = false;
        
        if (currentStroke.points.size() > 1)
        {
            sendGestureToProcessor(currentStroke);
            
            // Add to completed strokes for visual history
            completedStrokes.push_back(currentStroke);
            if (completedStrokes.size() > 50) // Limit history
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
    repaint(); // For brush cursor
}

void CanvasComponent::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel)
{
    juce::ignoreUnused(e);
    
    // Adjust brush size with mouse wheel
    float delta = wheel.deltaY * 5.0f;
    currentBrushSize = juce::jlimit(1.0f, 128.0f, currentBrushSize + delta);
    
    // Update parameter
    if (auto* param = audioProcessor.getValueTreeState().getParameter("brush_size"))
    {
        param->setValueNotifyingHost(currentBrushSize / 128.0f);
    }
    
    repaint();
}

bool CanvasComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (const auto& file : files)
    {
        if (file.endsWithIgnoreCase(".wav") || 
            file.endsWithIgnoreCase(".aiff") ||
            file.endsWithIgnoreCase(".flac") ||
            file.endsWithIgnoreCase(".mp3"))
        {
            return true;
        }
    }
    return false;
}

void CanvasComponent::fileDragEnter(const juce::StringArray& files, int x, int y)
{
    juce::ignoreUnused(files, x, y);
    isFileDragOver = true;
    repaint();
}

void CanvasComponent::fileDragExit(const juce::StringArray& files)
{
    juce::ignoreUnused(files);
    isFileDragOver = false;
    repaint();
}

void CanvasComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::ignoreUnused(x, y);
    
    isFileDragOver = false;
    
    if (files.size() > 0)
    {
        juce::File audioFile(files[0]);
        if (audioFile.existsAsFile())
        {
            // TODO: Send to AsyncSampleLoader
            juce::Logger::writeToLog("Loading sample: " + audioFile.getFullPathName());
            
            // For now, just switch to Resynth mode when file is loaded
            if (auto* param = audioProcessor.getValueTreeState().getParameter("mode"))
            {
                param->setValueNotifyingHost(1.0f / 2.0f); // Resynth mode
            }
        }
    }
    
    repaint();
}

void CanvasComponent::timerCallback()
{
    processSpectralData();
    updatePerformanceMetrics();
    repaint();
}

// Public interface methods
void CanvasComponent::setEngineMode(int mode) { /* TODO: Implement */ }
void CanvasComponent::setBlendAmount(float blend) { /* TODO: Implement */ }
void CanvasComponent::setBrushType(int type) { currentBrushType = type; }
void CanvasComponent::setBrushSize(float size) { currentBrushSize = size; }
void CanvasComponent::setBrushStrength(float strength) { currentBrushStrength = strength; }
void CanvasComponent::setGridVisible(bool visible) { gridVisible = visible; repaint(); }
void CanvasComponent::setSnapToScale(bool snap) { snapToScale = snap; }

// Private helper methods

juce::Point<float> CanvasComponent::screenToSpectral(juce::Point<float> screenPos) const
{
    const float canvasHeight = (float)getHeight();
    const float canvasWidth = (float)getWidth();
    
    // Account for floating UI margins
    const float topMargin = 80.0f;  // TopStrip + margin
    const float bottomMargin = 70.0f; // BottomBar + margin
    const float usableHeight = canvasHeight - topMargin - bottomMargin;
    
    float u = screenPos.x / canvasWidth;                           // Time: 0..1 (left to right)
    float v = 1.0f - ((screenPos.y - topMargin) / usableHeight);   // Frequency: 0..1 (bottom to top)
    
    return juce::Point<float>(juce::jlimit(0.0f, 1.0f, u), 
                              juce::jlimit(0.0f, 1.0f, v));
}

juce::Point<float> CanvasComponent::spectralToScreen(juce::Point<float> spectralPos) const
{
    const float canvasHeight = (float)getHeight();
    const float canvasWidth = (float)getWidth();
    
    const float topMargin = 80.0f;
    const float bottomMargin = 70.0f;
    const float usableHeight = canvasHeight - topMargin - bottomMargin;
    
    float x = spectralPos.x * canvasWidth;
    float y = topMargin + (1.0f - spectralPos.y) * usableHeight;
    
    return juce::Point<float>(x, y);
}

void CanvasComponent::sendGestureToProcessor(const PaintStroke& stroke)
{
    // TODO: Convert stroke to MaskAtlas updates
    // For now, just log the gesture
    juce::Logger::writeToLog("Paint stroke: " + juce::String(stroke.points.size()) + 
                             " points, type " + juce::String(stroke.brushType));
}

void CanvasComponent::processSpectralData()
{
    // Process any pending spectral frames from audio thread
    auto& spectralQueue = audioProcessor.getSpectralDataQueue();
    while (auto frame = spectralQueue.pop())
    {
        if (gpuRenderer)
        {
            gpuRenderer->updateSpectralTexture(*frame);
        }
    }
}

void CanvasComponent::updatePerformanceMetrics()
{
    // Calculate FPS (simple moving average)
    static float frameTimeAccum = 0.0f;
    static int frameCount = 0;
    
    frameTimeAccum += 16.67f; // Approximate
    frameCount++;
    
    if (frameCount >= 60) // Update once per second
    {
        currentFPS = 1000.0f * frameCount / frameTimeAccum;
        frameTimeAccum = 0.0f;
        frameCount = 0;
        
        // Phase 2-3: No bottom bar
        // bottomBar->updateMetrics(currentFPS, currentCPU, 17.0f);
    }
}

void CanvasComponent::drawGridOverlay(juce::Graphics& g)
{
    if (!gridVisible) return;
    
    const int width = getWidth();
    const int height = getHeight();
    const float topMargin = 80.0f;
    const float bottomMargin = 70.0f;
    const float usableHeight = height - topMargin - bottomMargin;
    
    g.setColour(juce::Colours::white.withAlpha(0.15f));
    
    // Frequency grid lines (horizontal) - musical pitches
    const int numOctaves = 8;
    const int notesPerOctave = 12;
    
    for (int octave = 1; octave <= numOctaves; ++octave)
    {
        for (int note = 0; note < notesPerOctave; ++note)
        {
            float frequency = 55.0f * std::pow(2.0f, octave + note / 12.0f); // A1 = 55Hz base
            if (frequency > 20000.0f) break;
            
            // Convert frequency to Y position (log scale)
            float normalizedFreq = std::log2(frequency / 20.0f) / std::log2(20000.0f / 20.0f);
            float y = topMargin + (1.0f - normalizedFreq) * usableHeight;
            
            if (y >= topMargin && y <= height - bottomMargin)
            {
                float alpha = (note == 0) ? 0.3f : 0.15f; // Octaves brighter
                g.setColour(juce::Colours::white.withAlpha(alpha));
                g.drawHorizontalLine((int)y, 0.0f, (float)width);
            }
        }
    }
    
    // Time grid lines (vertical) - beat markers
    const int numBeats = 16;
    for (int beat = 1; beat < numBeats; ++beat)
    {
        float x = (float)beat / numBeats * width;
        float alpha = (beat % 4 == 0) ? 0.25f : 0.1f; // Downbeats brighter
        g.setColour(juce::Colours::white.withAlpha(alpha));
        g.drawVerticalLine((int)x, topMargin, height - bottomMargin);
    }
}

void CanvasComponent::drawBrushCursor(juce::Graphics& g)
{
    const float x = lastMousePos.x;
    const float y = lastMousePos.y;
    const float radius = currentBrushSize * 0.5f;
    
    // Outer circle
    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.drawEllipse(x - radius, y - radius, radius * 2, radius * 2, 1.0f);
    
    // Inner crosshair
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    const float crossSize = 8.0f;
    g.drawLine(x - crossSize, y, x + crossSize, y, 1.0f);
    g.drawLine(x, y - crossSize, x, y + crossSize, 1.0f);
    
    // Strength indicator (filled portion)
    float strengthRadius = radius * currentBrushStrength;
    g.setColour(getNebulaAccentColor().withAlpha(0.3f));
    g.fillEllipse(x - strengthRadius, y - strengthRadius, 
                  strengthRadius * 2, strengthRadius * 2);
}

juce::Colour CanvasComponent::getNebulaAccentColor() const 
{ 
    return juce::Colour(0xff00bcd4); // Cyan from mockups
}

void CanvasComponent::generateImmediateAudioFeedback(const PaintStroke::Point& point)
{
    // MetaSynth-style: Convert paint stroke directly to spectral mask
    // spectralPos.y = frequency (0-1), spectralPos.x = time position
    
    MaskColumn maskColumn;
    maskColumn.numBins = 257; // For 512-point FFT (NUM_BINS = 257)
    maskColumn.timestampSamples = audioProcessor.getSampleRate() * point.timestamp;
    maskColumn.frameIndex = static_cast<uint32_t>(point.timestamp * 60.0); // ~60fps equivalent
    
    // Clear mask to default (no effect)
    for (size_t i = 0; i < maskColumn.numBins; ++i)
    {
        maskColumn.values[i] = 1.0f; // No masking by default
    }
    
    // Convert screen position to frequency bin
    const float frequencyRatio = juce::jlimit(0.0f, 1.0f, point.spectralPos.y);
    const int centerBin = static_cast<int>(frequencyRatio * (maskColumn.numBins - 1));
    
    // Calculate brush influence based on size and strength
    const int brushRadius = static_cast<int>(currentBrushSize * 0.5f);
    
    for (int bin = juce::jmax(0, centerBin - brushRadius); 
         bin <= juce::jmin(static_cast<int>(maskColumn.numBins - 1), centerBin + brushRadius); 
         ++bin)
    {
        const float distance = std::abs(bin - centerBin);
        const float falloff = juce::jmax(0.0f, 1.0f - (distance / brushRadius));
        
        switch (currentBrushType)
        {
            case 0: // Paint - Add spectral energy
                maskColumn.values[bin] = juce::jlimit(0.0f, 3.0f, 
                    maskColumn.values[bin] + (currentBrushStrength * falloff * point.pressure));
                break;
                
            case 1: // Quantize - Snap to scale (simplified)
                maskColumn.values[bin] = (falloff > 0.5f) ? 2.0f : 1.0f;
                break;
                
            case 2: // Erase - Remove spectral energy
                maskColumn.values[bin] = juce::jmax(0.0f, 
                    maskColumn.values[bin] - (currentBrushStrength * falloff * point.pressure));
                break;
                
            case 3: // Comb - Create harmonic series
                if (bin % 3 == 0) // Simple harmonic pattern
                {
                    maskColumn.values[bin] = 2.0f * currentBrushStrength;
                }
                break;
        }
    }
    
    // Send mask to audio thread immediately (RT-safe)
    if (auto& maskQueue = audioProcessor.getMaskColumnQueue(); maskQueue.hasSpaceAvailable())
    {
        maskQueue.push(maskColumn);
    }
}

void CanvasComponent::parentHierarchyChanged()
{
    // Phase 2-3 MINIMAL: No floating UI components to update
}

void CanvasComponent::visibilityChanged()
{
    // Phase 2-3 MINIMAL: No GPU renderer initialization needed
}

