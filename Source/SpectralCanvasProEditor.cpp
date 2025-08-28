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
    
    // Minimal sizing - no complex layouts needed
    setSize(1200, 800);
    setResizable(true, true);
    setResizeLimits(800, 600, 2400, 1600);
}

SpectralCanvasProEditor::~SpectralCanvasProEditor()
{
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
}