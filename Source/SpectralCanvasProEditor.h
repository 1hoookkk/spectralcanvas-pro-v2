#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "SpectralCanvasProAudioProcessor.h"
#include "GUI/CanvasComponent.h"
#include "GUI/MinimalTopStrip.h"
#include "GUI/PerfHUD.h"
#include "GUI/SpectrogramComponent.h"
#include "GUI/ToastManager.h"
#include "Core/SampleLoaderService.h"
#include "Viz/GpuRenderer.h"

/**
 * Phase 2-3 Minimal UI Editor
 * 
 * STRIPPED DOWN VERSION:
 * - MinimalTopStrip for essential controls only
 * - Full-bleed CanvasComponent for painting
 * - NO fancy panels until Phase 4
 * - RT-safe with <5ms paint-to-audio latency
 */
class SpectralCanvasProEditor : public juce::AudioProcessorEditor,
                              public juce::AudioProcessorValueTreeState::Listener
{
public:
    SpectralCanvasProEditor(SpectralCanvasProAudioProcessor&);
    ~SpectralCanvasProEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress& key) override;
    
    // Parameter listener for HUD toggle
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    // Sample loading callbacks
    void loadSampleButtonClicked();
    void loadAudioFile(const juce::File& file);
    
    // Access to loading infrastructure for other components
    SampleLoaderService* getSampleLoader() const { return sampleLoader.get(); }
    ToastManager* getToastManager() const { return toastManager.get(); }

private:
    SpectralCanvasProAudioProcessor& audioProcessor;
    
    // Phase 2-3 Minimal UI Components
    std::unique_ptr<CanvasComponent> canvasComponent;
    std::unique_ptr<MinimalTopStrip> topStrip;
    
    // Phase 5 Performance HUD
    std::unique_ptr<PerfHUD> perfHUD;
    
    // Spectral painting components
    juce::TextButton loadButton { "Load Sample" };
    SpectrogramComponent spectrogram;
    
    // GPU renderer
    std::unique_ptr<GpuRenderer> renderer;
    bool rendererActive = false; // Track initialization status for safety
    
    // Sample loading infrastructure
    std::unique_ptr<SampleLoaderService> sampleLoader;
    std::unique_ptr<ToastManager> toastManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralCanvasProEditor)
};