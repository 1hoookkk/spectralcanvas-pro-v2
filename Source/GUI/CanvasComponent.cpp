#include "CanvasComponent.h"
#include "TopStrip.h"
#include "BottomBar.h"
#include "../Viz/backends/D3D11Renderer.h"

CanvasComponent::CanvasComponent(SpectralCanvasProAudioProcessor& processor)
    : audioProcessor(processor)
{
    // Don't initialize GPU renderer in constructor - defer until parentHierarchyChanged
    
    // Create floating UI components
    topStrip = std::make_unique<TopStrip>(audioProcessor, *this);
    bottomBar = std::make_unique<BottomBar>(audioProcessor, *this);
    
    addAndMakeVisible(*topStrip);
    addAndMakeVisible(*bottomBar);
    
    // Start 60fps timer for smooth nebula animation
    startTimer(16); // ~60fps (16.67ms)
    
    // Initialize stroke system
    currentStroke.isActive = false;
    completedStrokes.reserve(100); // Preallocate for performance
}

CanvasComponent::~CanvasComponent()
{
    stopTimer();
    gpuRenderer.reset();
}

void CanvasComponent::paint(juce::Graphics& g)
{
    // Try GPU renderer first, fall back to software if not available
    if (gpuRenderer && gpuRenderer->isInitialized())
    {
        gpuRenderer->beginFrame();
        gpuRenderer->renderSpectralVisualization();
        gpuRenderer->renderGestureTrails();
        gpuRenderer->renderParticleSystem();
        gpuRenderer->endFrame();
        gpuRenderer->present();
    }
    else
    {
        // Software fallback - living nebula-style background
        paintSoftwareFallback(g);
    }
    
    // Overlay grid if enabled
    if (gridVisible)
    {
        drawGridOverlay(g);
    }
    
    // Draw brush cursor
    if (isMouseOverOrDragging())
    {
        drawBrushCursor(g);
    }
    
    // File drag feedback
    if (isFileDragOver)
    {
        g.setColour(getNebulaAccentColor().withAlpha(0.3f));
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);
        
        g.setColour(getNebulaAccentColor());
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 8.0f, 2.0f);
        
        g.setFont(24.0f);
        g.drawText("Drop audio file to resynthesize", getLocalBounds(), juce::Justification::centred);
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
    const int width = getWidth();
    const int height = getHeight();
    
    // Resize GPU renderer
    if (gpuRenderer)
    {
        gpuRenderer->resizeBuffers(width, height);
    }
    
    // Position floating UI components
    const int topStripHeight = 60;
    const int bottomBarHeight = 50;
    const int margin = 12;
    
    // TopStrip floats at top with margin
    topStrip->setBounds(margin, margin, width - 2 * margin, topStripHeight);
    
    // BottomBar floats at bottom with margin  
    bottomBar->setBounds(margin, height - bottomBarHeight - margin, 
                        width - 2 * margin, bottomBarHeight);
}

void CanvasComponent::parentHierarchyChanged()
{
    // Initialize GPU renderer once we're attached to a window
    if (!gpuRenderer && getTopLevelComponent() != nullptr)
    {
        // Defer initialization slightly to ensure window is fully ready
        juce::MessageManager::callAsync([this]()
        {
            if (!gpuRenderer)
            {
                initializeGpuRenderer();
            }
        });
    }
}

void CanvasComponent::mouseDown(const juce::MouseEvent& e)
{
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
void CanvasComponent::initializeGpuRenderer()
{
#ifdef _WIN32
    try 
    {
        gpuRenderer = std::make_unique<D3D11Renderer>();
        
        // Initialize with window handle when component is visible
        if (auto* peer = getPeer())
        {
            if (auto* nativeHandle = peer->getNativeHandle())
            {
                bool success = gpuRenderer->initialize(nativeHandle, getWidth(), getHeight());
                if (!success)
                {
                    // GPU initialization failed, reset to use software fallback
                    gpuRenderer.reset();
                }
            }
        }
        else
        {
            // No window peer available yet, reset and try later
            gpuRenderer.reset();
        }
    }
    catch (...)
    {
        // GPU initialization threw exception, use software fallback
        gpuRenderer.reset();
    }
#endif
}

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
        
        // Update bottom bar
        bottomBar->updateMetrics(currentFPS, currentCPU, 17.0f); // TODO: Real latency
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