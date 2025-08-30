#include "SpectralCanvasProEditor.h"
#include "Core/Params.h"

SpectralCanvasProEditor::SpectralCanvasProEditor(SpectralCanvasProAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Phase 2-3 Minimal UI: Top strip + full-bleed canvas
    
    // Canvas component (full-bleed background)
    canvasComponent = std::make_unique<CanvasComponent>(audioProcessor);
    addAndMakeVisible(*canvasComponent);
    
    // Minimal top strip with essential controls only
    topStrip = std::make_unique<MinimalTopStrip>(audioProcessor.apvts);
    addAndMakeVisible(*topStrip);
    
    // Phase 5 Performance HUD (initially hidden)
    perfHUD = std::make_unique<PerfHUD>(audioProcessor);
    addAndMakeVisible(*perfHUD);
    
    // Listen for HUD toggle parameter changes
    audioProcessor.getValueTreeState().addParameterListener(Params::ParameterIDs::showPerfHud, this);
    
    // Set initial HUD visibility based on parameter
    auto* hudParam = audioProcessor.getValueTreeState().getRawParameterValue(Params::ParameterIDs::showPerfHud);
    if (hudParam != nullptr) {
        perfHUD->setVisible(*hudParam >= 0.5f);
    }
    
    // Enable keyboard focus for 'H' key toggle
    setWantsKeyboardFocus(true);
    
    // Minimal sizing - no complex layouts needed
    setSize(1200, 800);
    setResizable(true, true);
    setResizeLimits(800, 600, 2400, 1600);
}

SpectralCanvasProEditor::~SpectralCanvasProEditor()
{
    // Remove parameter listener
    audioProcessor.getValueTreeState().removeParameterListener(Params::ParameterIDs::showPerfHud, this);
    
    perfHUD.reset();
    topStrip.reset();
    canvasComponent.reset();
}

void SpectralCanvasProEditor::paint(juce::Graphics& g)
{
    // Minimal dark background fallback
    g.fillAll(juce::Colour(0xff0a0a0f));
}

void SpectralCanvasProEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Top strip gets fixed height (40px minimal)
    auto topArea = bounds.removeFromTop(40);
    topStrip->setBounds(topArea);
    
    // Canvas gets all remaining area
    canvasComponent->setBounds(bounds);
    
    // Position HUD in top-right corner with margin
    if (perfHUD) {
        const int margin = 10;
        perfHUD->setTopRightPosition(getWidth() - margin, topArea.getHeight() + margin);
    }
}

bool SpectralCanvasProEditor::keyPressed(const juce::KeyPress& key)
{
    // Handle 'H' key for HUD toggle
    if (key.getKeyCode() == 'H' || key.getKeyCode() == 'h') {
        if (perfHUD) {
            // Toggle the parameter value (0.0 -> 1.0, 1.0 -> 0.0)
            auto* hudParam = audioProcessor.getValueTreeState().getParameter(Params::ParameterIDs::showPerfHud);
            if (hudParam != nullptr) {
                float currentValue = hudParam->getValue();
                hudParam->setValueNotifyingHost(currentValue < 0.5f ? 1.0f : 0.0f);
            }
        }
        return true;
    }
    
    // Pass other keys to canvas component for painting shortcuts
    if (canvasComponent) {
        return canvasComponent->keyPressed(key);
    }
    
    return false;
}

void SpectralCanvasProEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == Params::ParameterIDs::showPerfHud && perfHUD) {
        // Update HUD visibility on main message thread
        juce::MessageManager::callAsync([this, newValue]() {
            if (perfHUD) {
                perfHUD->setVisible(newValue >= 0.5f);
            }
        });
    }
}