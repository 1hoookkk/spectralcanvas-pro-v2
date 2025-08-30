#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../SpectralCanvasProAudioProcessor.h"

class CanvasComponent;

/**
 * Floating control strip at top of canvas with Mode/Brush/Blend controls
 * Matches the design from the UI mockups
 */
class TopStrip : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::ComboBox::Listener
{
public:
    TopStrip(SpectralCanvasProAudioProcessor& processor, CanvasComponent& canvas);
    ~TopStrip() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Control callbacks
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    
    // Parent hierarchy updates
    void updateParentReferences() {}
    
private:
    SpectralCanvasProAudioProcessor& audioProcessor;
    CanvasComponent& canvasComponent;
    
    // Left section - File operations
    std::unique_ptr<juce::TextButton> loadSampleButton;
    juce::Label loadedSampleLabel;
    
    // Center section - Mode controls
    std::unique_ptr<juce::ComboBox> modeSelector;      // Synth | Resynth | Hybrid
    std::unique_ptr<juce::Slider> blendSlider;         // 0..1 crossfade
    juce::Label modeLabel;
    juce::Label blendLabel;
    
    // Right section - Brush controls  
    std::unique_ptr<juce::ComboBox> brushTypeSelector; // Paint | Quantize | Erase | Comb
    std::unique_ptr<juce::Slider> brushSizeSlider;     // 1..128 bins
    std::unique_ptr<juce::Slider> brushStrengthSlider; // 0..1
    juce::Label brushTypeLabel;
    juce::Label brushSizeLabel;
    juce::Label brushStrengthLabel;
    
    // Far right - Advanced options
    std::unique_ptr<juce::ToggleButton> snapToScaleButton;
    std::unique_ptr<juce::ComboBox> tuningSelector;    // 12-TET, Custom...
    std::unique_ptr<juce::ToggleButton> fastPaintButton;
    
    // Visual styling
    const int stripHeight = 60;
    const int margin = 8;
    const int controlSpacing = 12;
    
    // Helper methods
    void setupControls();
    void updateControlStates();
    void loadSampleFile();
    juce::Colour getNebulaAccentColor() const { return juce::Colour(0xff00bcd4); } // Cyan
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopStrip)
};