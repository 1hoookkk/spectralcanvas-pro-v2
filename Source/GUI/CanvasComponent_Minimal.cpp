#include "CanvasComponent.h"
#include "../Core/Params.h"

CanvasComponent::CanvasComponent(SpectralCanvasProAudioProcessor& processor)
    : audioProcessor(processor)
{
    setOpaque(true);
    startTimer(16); // 60fps
    
    // Initialize stroke system
    currentStroke.isActive = false;
    completedStrokes.reserve(100);
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

    // Debug overlay
    bool debugEnabled = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::debugOverlayEnabled).getValue();
    if (debugEnabled)
    {
        g.setColour(juce::Colours::lime);
        g.setFont(juce::FontOptions(12.0f));
        g.drawText("Phase 2-3 Minimal UI Active", 8, 18, 300, 14, juce::Justification::left);
        
        bool testActive = audioProcessor.apvts.getParameterAsValue(Params::ParameterIDs::testModeActive).getValue();
        g.drawText(juce::String::formatted("Test Mode: %s", testActive ? "ACTIVE" : "OFF"), 
                   8, 32, 300, 14, juce::Justification::left);
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

    // Start paint stroke
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