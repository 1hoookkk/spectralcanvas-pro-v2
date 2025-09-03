#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Core/Params.h"

/**
 * Phase 2-3 Minimal top control strip
 * 
 * STRICT MINIMAL DESIGN:
 * - No custom painting or gradients
 * - Standard JUCE components only
 * - Start/Stop toggle, Mask Depth, Brush Strength, Debug toggle
 * - NO fancy theming until Phase 4
 */
class MinimalTopStrip : public juce::Component
{
public:
    MinimalTopStrip(juce::AudioProcessorValueTreeState& apvts);
    ~MinimalTopStrip() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    juce::AudioProcessorValueTreeState& apvts;

    // Phase 2 Required Controls
    juce::ComboBox modeCombo;
    juce::Slider blendSlider;
    juce::ToggleButton testToggleButton;
    juce::Slider maskDepthSlider;
    juce::Slider brushStrengthSlider;
    juce::ToggleButton debugToggleButton;

    // Labels (minimal styling)
    juce::Label modeLabel;
    juce::Label blendLabel;
    juce::Label testLabel;
    juce::Label maskDepthLabel;
    juce::Label brushStrengthLabel; 
    juce::Label debugLabel;

    // Parameter attachments
    std::unique_ptr<juce::ComboBoxParameterAttachment> modeAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> blendAttachment;
    std::unique_ptr<juce::ButtonParameterAttachment> testToggleAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> maskDepthAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> brushStrengthAttachment;
    std::unique_ptr<juce::ButtonParameterAttachment> debugToggleAttachment;

    void setupControl(juce::Component& control, juce::Label& label, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MinimalTopStrip)
};
