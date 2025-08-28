#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "SpectralCanvasProAudioProcessor.h"
#include "GUI/CanvasComponent.h"

/**
 * Main editor component that hosts the full-bleed nebula canvas interface
 * Transformed from basic placeholder to match the UI mockups
 */
class SpectralCanvasProEditor : public juce::AudioProcessorEditor
{
public:
    SpectralCanvasProEditor(SpectralCanvasProAudioProcessor&);
    ~SpectralCanvasProEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    // Layout helpers
    void layoutControlPanels();
    juce::Rectangle<int> getCanvasArea() const;
    juce::Rectangle<int> getControlStripArea() const;

private:
    SpectralCanvasProAudioProcessor& audioProcessor;
    
    // Main canvas component (full-bleed nebula interface) 
    std::unique_ptr<CanvasComponent> canvasComponent;
    
    // Control panels
    class SourceEnginePanel;
    class BrushControlsPanel;
    class PostFXPanel;
    
    std::unique_ptr<SourceEnginePanel> sourceEnginePanel;
    std::unique_ptr<BrushControlsPanel> brushControlsPanel;
    std::unique_ptr<PostFXPanel> postFXPanel;
    
    // Parameter attachments for real-time control
    std::unique_ptr<juce::SliderParameterAttachment> modeSliderAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> brushSizeAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> brushStrengthAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralCanvasProEditor)
};