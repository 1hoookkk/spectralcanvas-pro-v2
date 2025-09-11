#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../Core/SpectralCanvasLiteProcessor.h"
#include "ModernLookAndFeel.h"

class SpectralCanvasLiteEditor : public juce::AudioProcessorEditor, 
                                 private juce::Timer
{
public:
    explicit SpectralCanvasLiteEditor(SpectralCanvasLiteProcessor&);
    ~SpectralCanvasLiteEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void timerCallback() override;

    void pushMaskEventAt(const juce::Point<int>& pos);
    void updatePerformanceStats();

    SpectralCanvasLiteProcessor& proc_;
    ModernLookAndFeel modernLookAndFeel_;

    // A dedicated component for the spectral display and painting
    class SpectralCanvasComponent : public juce::Component
    {
    public:
        void paint(juce::Graphics& g) override {
            g.fillAll(juce::Colours::black);
            g.setColour(juce::Colours::darkgrey);
            g.drawRect(getLocalBounds(), 1);
            g.setColour(juce::Colours::lightgrey);
            g.drawFittedText("Canvas", getLocalBounds(), juce::Justification::centred, 1);
        }
    };
    
    SpectralCanvasComponent canvas_;

    juce::Label titleLabel_, modeLabel_, brushWidthLabel_, brushStrengthLabel_, maskFloorLabel_, outputTrimLabel_;

    juce::Slider brushWidthSlider_, brushStrengthSlider_, maskFloorDbSlider_, outputTrimDbSlider_;
    juce::ComboBox modeBox_;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<SliderAttachment> brushWidthAtt_, brushStrengthAtt_, maskFloorDbAtt_, outputTrimDbAtt_;
    std::unique_ptr<ComboBoxAttachment> modeAtt_;

    // Performance HUD
    class PerformanceHUD : public juce::Component
    {
    public:
        PerformanceHUD() = default;
        void paint(juce::Graphics& g) override;
        void updateStats(uint64_t enqueued, uint64_t drained, uint64_t dropped, float peakGrDb);
        
    private:
        juce::String statsText_;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerformanceHUD)
    };
    
    PerformanceHUD performanceHUD_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralCanvasLiteEditor)
};
