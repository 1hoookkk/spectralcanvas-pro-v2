#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Core/Params.h"

/**
 * Phase 3 Minimal bottom control strip  
 * 
 * OPTIONAL CONTROLS (Phase 3 validation):
 * - Colormap selector (Grey/Viridis/Magma)
 * - Gamma (0.5-1.5) & Gain (0-4) sliders for visualization
 * - Freeze/Scroll toggle for spectrogram
 * - NO fancy theming until Phase 4
 */
class MinimalBottomStrip : public juce::Component
{
public:
    MinimalBottomStrip(juce::AudioProcessorValueTreeState& apvts);
    ~MinimalBottomStrip() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    juce::AudioProcessorValueTreeState& apvts;
    
    // Phase 3 Visual Controls
    juce::ComboBox colormapSelector;
    juce::Slider gammaSlider;
    juce::Slider gainSlider; 
    juce::ToggleButton freezeToggle;
    
    // Labels (minimal styling)
    juce::Label colormapLabel;
    juce::Label gammaLabel;
    juce::Label gainLabel;
    juce::Label freezeLabel;
    
    // Parameter attachments
    std::unique_ptr<juce::ComboBoxParameterAttachment> colormapAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> gammaAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> gainAttachment;
    std::unique_ptr<juce::ButtonParameterAttachment> freezeAttachment;
    
    void setupControl(juce::Component& control, juce::Label& label, const juce::String& text);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MinimalBottomStrip)
};