#include "MinimalTopStrip.h"

MinimalTopStrip::MinimalTopStrip(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts)
{
    // Mode Combo (0=Synth, 1=Resynth, 2=Hybrid)
    modeCombo.addItem("Synth", 1);
    modeCombo.addItem("Resynth", 2);
    modeCombo.addItem("Hybrid", 3);
    modeCombo.setJustificationType(juce::Justification::centredLeft);
    setupControl(modeCombo, modeLabel, "Mode");

    // Blend Slider (0..1), only meaningful in Hybrid mode
    blendSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    blendSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    blendSlider.setRange(0.0, 1.0, 0.01);
    blendSlider.setValue(0.0); // 0=synth only, 1=resynth only
    setupControl(blendSlider, blendLabel, "Blend");

    // Test Mode Toggle
    testToggleButton.setButtonText("START");
    testToggleButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    testToggleButton.onStateChange = [this]() {
        // Update button text based on state
        testToggleButton.setButtonText(testToggleButton.getToggleState() ? "STOP" : "START");
    };
    setupControl(testToggleButton, testLabel, "Test Mode");
    
    // Mask Depth Slider (0..1, linear)
    maskDepthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    maskDepthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    maskDepthSlider.setRange(0.0, 1.0, 0.01);
    maskDepthSlider.setValue(0.7); // Default from Params.h
    setupControl(maskDepthSlider, maskDepthLabel, "Mask Depth");
    
    // Brush Strength Slider (0..1, linear) 
    brushStrengthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    brushStrengthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    brushStrengthSlider.setRange(0.0, 1.0, 0.01);
    brushStrengthSlider.setValue(0.7); // Default from Params.h
    setupControl(brushStrengthSlider, brushStrengthLabel, "Brush Strength");
    
    // Debug Toggle
    debugToggleButton.setButtonText("Debug");
    debugToggleButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    setupControl(debugToggleButton, debugLabel, "Debug Overlay");

    // Loop Sample Toggle
    loopSampleToggle.setButtonText("Loop");
    loopSampleToggle.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    setupControl(loopSampleToggle, loopLabel, "Playback");

    // Parameter attachments
    modeAttachment = std::make_unique<juce::ComboBoxParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::mode), modeCombo);

    blendAttachment = std::make_unique<juce::SliderParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::blend), blendSlider);

    testToggleAttachment = std::make_unique<juce::ButtonParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::useTestFeeder), testToggleButton);
    
    maskDepthAttachment = std::make_unique<juce::SliderParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::maskDepth), maskDepthSlider);
        
    brushStrengthAttachment = std::make_unique<juce::SliderParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::brushStrength), brushStrengthSlider);
        
    debugToggleAttachment = std::make_unique<juce::ButtonParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::debugOverlayEnabled), debugToggleButton);

    loopToggleAttachment = std::make_unique<juce::ButtonParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::loopSample), loopSampleToggle);
}

void MinimalTopStrip::paint(juce::Graphics& g)
{
    // Minimal dark background - NO gradients or rounded rectangles per Phase 2-3 spec
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    // Simple border line
    g.setColour(juce::Colour(0xff404040));
    g.drawHorizontalLine(getHeight() - 1, 0.0f, static_cast<float>(getWidth()));
}

void MinimalTopStrip::resized()
{
    auto bounds = getLocalBounds().reduced(8, 4);
    // 7 groups: Mode | Blend | Test | MaskDepth | BrushStrength | Debug | Loop
    int controlWidth = bounds.getWidth() / 7;

    // Layout controls horizontally
    auto modeArea = bounds.removeFromLeft(controlWidth);
    auto blendArea = bounds.removeFromLeft(controlWidth);
    auto testArea = bounds.removeFromLeft(controlWidth);
    auto maskArea = bounds.removeFromLeft(controlWidth);
    auto strengthArea = bounds.removeFromLeft(controlWidth);
    auto debugArea = bounds.removeFromLeft(controlWidth);
    auto loopArea = bounds; // last slot uses remainder

    // Each area: label on top, control below
    modeLabel.setBounds(modeArea.removeFromTop(16));
    modeCombo.setBounds(modeArea.reduced(4));

    blendLabel.setBounds(blendArea.removeFromTop(16));
    blendSlider.setBounds(blendArea.reduced(4));

    testLabel.setBounds(testArea.removeFromTop(16));
    testToggleButton.setBounds(testArea.reduced(4));
    
    maskDepthLabel.setBounds(maskArea.removeFromTop(16));
    maskDepthSlider.setBounds(maskArea.reduced(4));
    
    brushStrengthLabel.setBounds(strengthArea.removeFromTop(16));
    brushStrengthSlider.setBounds(strengthArea.reduced(4));
    
    debugLabel.setBounds(debugArea.removeFromTop(16));
    debugToggleButton.setBounds(debugArea.reduced(4));

    loopLabel.setBounds(loopArea.removeFromTop(16));
    loopSampleToggle.setBounds(loopArea.reduced(4));
}

void MinimalTopStrip::setupControl(juce::Component& control, juce::Label& label, const juce::String& text)
{
    addAndMakeVisible(control);
    
    label.setText(text, juce::dontSendNotification);
    label.setFont(juce::FontOptions().withPointHeight(10.0f));
    label.setColour(juce::Label::textColourId, juce::Colour(0xffcccccc));
    label.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(label);
}
