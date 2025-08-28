#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../SpectralCanvasProAudioProcessor.h"
#include "../Viz/GpuRenderer.h"

class TopStrip;
class BottomBar;

/**
 * Full-bleed nebula canvas component that hosts the spectral visualization
 * and floating UI controls. This is the main interface matching the mockups.
 */
class CanvasComponent : public juce::Component,
                        public juce::Timer,
                        public juce::FileDragAndDropTarget
{
public:
    CanvasComponent(SpectralCanvasProAudioProcessor& processor);
    ~CanvasComponent() override;
    
    // Component interface
    void paint(juce::Graphics& g) override;
    void resized() override;
    void parentHierarchyChanged() override;
    
    // Mouse interaction for paint gestures
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;
    
    // Drag & drop for sample loading
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void fileDragEnter(const juce::StringArray& files, int x, int y) override;
    void fileDragExit(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    // Timer for 60fps updates
    void timerCallback() override;
    
    // Public interface for UI controls
    void setEngineMode(int mode);
    void setBlendAmount(float blend);
    void setBrushType(int type);
    void setBrushSize(float size);
    void setBrushStrength(float strength);
    void setGridVisible(bool visible);
    void setSnapToScale(bool snap);
    
private:
    SpectralCanvasProAudioProcessor& audioProcessor;
    
    // GPU renderer for spectral visualization
    std::unique_ptr<GpuRenderer> gpuRenderer;
    
    // Floating UI components
    std::unique_ptr<TopStrip> topStrip;
    std::unique_ptr<BottomBar> bottomBar;
    
    // Gesture tracking for paint system
    struct PaintStroke
    {
        struct Point
        {
            juce::Point<float> position;    // Screen coordinates
            juce::Point<float> spectralPos; // U,V in spectral space (0-1)
            float pressure;                 // 0-1
            float timestamp;                // Relative to stroke start
        };
        
        std::vector<Point> points;
        int brushType;                      // 0=Paint, 1=Quantize, 2=Erase, 3=Comb
        float brushSize;
        float brushStrength;
        juce::uint32 startTime;
        bool isActive;
    };
    
    PaintStroke currentStroke;
    std::vector<PaintStroke> completedStrokes; // For visual history
    
    // Canvas state
    bool isDragging = false;
    bool isFileDragOver = false;
    juce::Point<float> lastMousePos;
    
    // Visual settings
    bool gridVisible = false;
    bool snapToScale = false;
    int currentBrushType = 0;      // Paint brush default
    float currentBrushSize = 16.0f;
    float currentBrushStrength = 0.7f;
    
    // Performance metrics for BottomBar
    float currentFPS = 60.0f;
    float currentCPU = 0.0f;
    
    // Helper methods
    void initializeGpuRenderer();
    void paintSoftwareFallback(juce::Graphics& g);
    juce::Point<float> screenToSpectral(juce::Point<float> screenPos) const;
    juce::Point<float> spectralToScreen(juce::Point<float> spectralPos) const;
    void sendGestureToProcessor(const PaintStroke& stroke);
    void processSpectralData();
    void updatePerformanceMetrics();
    void drawGridOverlay(juce::Graphics& g);
    void drawBrushCursor(juce::Graphics& g);
    juce::Colour getNebulaAccentColor() const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CanvasComponent)
};