#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class ModernLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ModernLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::white);
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff2d3135));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff2d3135));
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        
        setColour(juce::Label::textColourId, juce::Colours::lightgrey);
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff2d3135));
        setColour(juce::ComboBox::outlineColourId, juce::Colours::black);
        setColour(juce::ComboBox::arrowColourId, juce::Colours::lightgrey);
        setColour(juce::ComboBox::textColourId, juce::Colours::white);
        
        setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xff2d3135));
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::deepskyblue);
        setColour(juce::PopupMenu::textColourId, juce::Colours::white);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin(8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        // Background
        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        g.fillEllipse(bounds);

        // Outline
        g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
        g.drawEllipse(bounds, 1.0f);

        // Pointer
        juce::Path p;
        auto center = bounds.getCentre();
        p.addPieSegment(center.x, center.y, arcRadius * 0.8f, arcRadius, rotaryStartAngle, toAngle, 0.0f);
        g.setColour(juce::Colours::deepskyblue);
        g.fillPath(p);

        // Center dot
        g.setColour(juce::Colours::black);
        g.fillEllipse(center.x - lineW, center.y - lineW, lineW * 2.0f, lineW * 2.0f);
    }
};
