#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../SpectralCanvasProAudioProcessor.h"
#include "../Core/MessageBus.h"
#include <chrono>
#include <array>
#include <atomic>

/**
 * Phase 2-3 Minimal Canvas Component
 * 
 * STRIPPED DOWN VERSION:
 * - No GPU renderer or floating UI components
 * - Simple spectral visualization and paint strokes
 * - Debug overlay with RT-safe metrics
 * - <5ms paint-to-audio latency via SPSC queues
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
    void visibilityChanged() override;
    
    // Mouse interaction for paint gestures
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;
    
    // Keyboard interaction
    bool keyPressed(const juce::KeyPress& key) override;
    
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
    
    // Modern paint support
    void pushPaintEvent(float y, float intensity);
    
private:
    SpectralCanvasProAudioProcessor& audioProcessor;
    
    // Phase 2-3: No GPU renderer or floating UI components
    // std::unique_ptr<GpuRenderer> gpuRenderer;
    // std::unique_ptr<TopStrip> topStrip;
    // std::unique_ptr<BottomBar> bottomBar;
    
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
    
    // Performance metrics and debug tracking
    float currentFPS = 60.0f;
    std::chrono::high_resolution_clock::time_point frameStartTime;
    std::array<float, 64> latencyBuffer; // Circular buffer for latency percentiles
    std::atomic<int> queueDropCounter{0}; // RT-safe drop counter
    
    // Phase 2-3 Minimal Helper methods
    void paintMinimalBackground(juce::Graphics& g);
    void paintDebugOverlay(juce::Graphics& g);
    void drawSimpleBrushCursor(juce::Graphics& g);
    juce::Point<float> screenToSpectral(juce::Point<float> screenPos) const;
    juce::Point<float> spectralToScreen(juce::Point<float> spectralPos) const;
    void sendMaskColumnToAudio(const PaintStroke::Point& point);
    void processSpectralData();
    void updatePerformanceMetrics();
    void recordLatencyMeasurement();
    float calculateMedianLatency() const;
    float calculateP95Latency() const;
    void createAndSendMaskColumn(juce::Point<float> mousePos);
    void pushMaskFromScreenY(float screenY) noexcept;
    
    // Phase 4 experiment: improved Y-to-bin mapping
#ifdef PHASE4_EXPERIMENT
    static inline int uiToBinLinear(float yNorm, int numBins) noexcept;
    static inline int uiToBinLog(float yNorm, double sampleRate, int fftSize) noexcept;
    void createAndSendMaskColumnPhase4(juce::Point<float> mousePos);
    
    // MaskColumn accumulator for batching
    struct ColumnAccumulator {
        std::array<float, 1024> values;
        bool hasData = false;
        
        void clear() noexcept {
            std::fill(values.begin(), values.end(), 0.0f);
            hasData = false;
        }
        
        void accumulate(int binIndex, float value) noexcept {
            if (binIndex >= 0 && binIndex < static_cast<int>(values.size())) {
                values[binIndex] = std::max(values[binIndex], value);
                hasData = true;
            }
        }
    };
    
    ColumnAccumulator accumulator_;
    std::chrono::high_resolution_clock::time_point lastDispatchTime_;
    static constexpr int batchIntervalMs_ = 2;  // 2ms batching interval
#endif
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CanvasComponent)
};