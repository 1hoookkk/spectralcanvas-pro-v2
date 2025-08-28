#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../SpectralCanvasProAudioProcessor.h"

class CanvasComponent;

/**
 * Floating control strip at bottom of canvas with transport and metrics
 * Matches the design from the UI mockups
 */
class BottomBar : public juce::Component,
                  public juce::Button::Listener,
                  public juce::Slider::Listener
{
public:
    BottomBar(SpectralCanvasProAudioProcessor& processor, CanvasComponent& canvas);
    ~BottomBar() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Control callbacks
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    
    // Update performance metrics from canvas
    void updateMetrics(float fps, float cpuPercent, float latencyMs);
    void updateFFTSize(int fftSize);
    void updateServiceHealth(bool audioOk, bool gpuOk);
    
    // Parent hierarchy updates
    void updateParentReferences() {}
    
private:
    SpectralCanvasProAudioProcessor& audioProcessor;
    CanvasComponent& canvasComponent;
    
    // Left section - Transport controls
    std::unique_ptr<juce::TextButton> playButton;
    std::unique_ptr<juce::TextButton> stopButton;
    std::unique_ptr<juce::ToggleButton> loopButton;
    
    // Center section - Playback controls
    std::unique_ptr<juce::Slider> speedSlider;         // 0.25x - 4x resynth speed
    std::unique_ptr<juce::Slider> gainSlider;          // -24 to +24 dB
    juce::Label speedLabel;
    juce::Label gainLabel;
    
    // Right section - Performance metrics  
    juce::Label latencyLabel;      // "Latency: 17 ms"
    juce::Label fpsLabel;          // "60 fps"
    juce::Label cpuLabel;          // "CPU: 12%"
    juce::Label fftSizeLabel;      // "FFT Size: 4096"
    
    // Service health indicators
    juce::Label serviceHealthLabel; // "Service Health"
    juce::Component audioHealthIndicator;
    juce::Component gpuHealthIndicator;
    
    // State
    bool isPlaying = false;
    bool isLooping = false;
    float currentFPS = 60.0f;
    float currentCPU = 0.0f;
    float currentLatency = 17.0f;
    int currentFFTSize = 512;
    bool audioServiceOk = true;
    bool gpuServiceOk = true;
    
    // Visual styling
    const int barHeight = 50;
    const int margin = 8;
    const int controlSpacing = 12;
    const int indicatorSize = 12;
    
    // Helper methods
    void setupControls();
    void updateTransportStates();
    void drawHealthIndicator(juce::Graphics& g, juce::Rectangle<int> area, bool isHealthy);
    juce::Colour getNebulaAccentColor() const { return juce::Colour(0xff00bcd4); } // Cyan
    juce::Colour getHealthColor(bool healthy) const 
    { 
        return healthy ? juce::Colours::lightgreen : juce::Colours::orangered; 
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomBar)
};