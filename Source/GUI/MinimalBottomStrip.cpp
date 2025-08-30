#include "MinimalBottomStrip.h"

MinimalBottomStrip::MinimalBottomStrip(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts)
{
    // Colormap Selector (Phase 3)
    colormapSelector.addItem("Grey", 1);
    colormapSelector.addItem("Viridis", 2);
    colormapSelector.addItem("Magma", 3);
    colormapSelector.setSelectedId(3); // Default to Magma
    setupControl(colormapSelector, colormapLabel, "Colormap");
    
    // Gamma Slider (0.5-1.5 for display gamma correction)
    gammaSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    gammaSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    gammaSlider.setRange(0.5, 1.5, 0.01);
    gammaSlider.setValue(1.0); // Default gamma
    setupControl(gammaSlider, gammaLabel, "Gamma");
    
    // Gain Slider (0-4 for display gain boost)
    gainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    gainSlider.setRange(0.0, 4.0, 0.01);
    gainSlider.setValue(1.0); // Default gain
    setupControl(gainSlider, gainLabel, "Gain");
    
    // Freeze Toggle (stop spectrogram scrolling)
    freezeToggle.setButtonText("Freeze");
    freezeToggle.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    setupControl(freezeToggle, freezeLabel, "Display");
    
    // Parameter attachments
    colormapAttachment = std::make_unique<juce::ComboBoxParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::colormapType), colormapSelector);
    
    gammaAttachment = std::make_unique<juce::SliderParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::visualGamma), gammaSlider);
        
    gainAttachment = std::make_unique<juce::SliderParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::visualGain), gainSlider);
        
    freezeAttachment = std::make_unique<juce::ButtonParameterAttachment>(
        *apvts.getParameter(Params::ParameterIDs::freezeDisplay), freezeToggle);
}

void MinimalBottomStrip::paint(juce::Graphics& g)
{
    // Phase 3 MINIMAL: Simple dark background, no gradients
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    // Simple border line
    g.setColour(juce::Colour(0xff404040));
    g.drawHorizontalLine(0, 0.0f, static_cast<float>(getWidth()));
}

void MinimalBottomStrip::resized()
{
    auto bounds = getLocalBounds().reduced(8, 4);
    int controlWidth = bounds.getWidth() / 4;
    
    // Layout controls horizontally: Colormap | Gamma | Gain | Freeze
    auto colormapArea = bounds.removeFromLeft(controlWidth);
    auto gammaArea = bounds.removeFromLeft(controlWidth);
    auto gainArea = bounds.removeFromLeft(controlWidth);
    auto freezeArea = bounds;
    
    // Each area: label on top, control below
    colormapLabel.setBounds(colormapArea.removeFromTop(16));
    colormapSelector.setBounds(colormapArea.reduced(4));
    
    gammaLabel.setBounds(gammaArea.removeFromTop(16));
    gammaSlider.setBounds(gammaArea.reduced(4));
    
    gainLabel.setBounds(gainArea.removeFromTop(16));
    gainSlider.setBounds(gainArea.reduced(4));
    
    freezeLabel.setBounds(freezeArea.removeFromTop(16));
    freezeToggle.setBounds(freezeArea.reduced(4));
}

void MinimalBottomStrip::setupControl(juce::Component& control, juce::Label& label, const juce::String& text)
{
    addAndMakeVisible(control);
    
    label.setText(text, juce::dontSendNotification);
    label.setFont(juce::FontOptions().withPointHeight(10.0f));
    label.setColour(juce::Label::textColourId, juce::Colour(0xffcccccc));
    label.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(label);
}