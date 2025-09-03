#include "SpectralCanvasProEditor.h"
#include "Core/Params.h"
#include "Core/DiagnosticLogger.h"
#include "Viz/backends/D3D11Renderer.h"

SpectralCanvasProEditor::SpectralCanvasProEditor(SpectralCanvasProAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Phase 2-3 Minimal UI: Top strip + full-bleed canvas
    
    // Canvas component (full-bleed background)
    canvasComponent = std::make_unique<CanvasComponent>(audioProcessor);
    canvasComponent->setParentEditor(this);
    addAndMakeVisible(*canvasComponent);
    
    // Minimal top strip with essential controls only
    topStrip = std::make_unique<MinimalTopStrip>(audioProcessor.apvts);
    addAndMakeVisible(*topStrip);
    
    // Create GPU renderer with safety checks
#if JUCE_WINDOWS
    renderer = std::make_unique<D3D11Renderer>();
    if (renderer) {
        // Safe initialization with fallback handling
        bool initSuccess = renderer->initialize(getTopLevelComponent()->getWindowHandle(), 1200, 800);
        rendererActive = initSuccess && renderer->isInitialized();
        
        if (!rendererActive) {
            // Log failure but continue - CPU fallback will be used
            juce::Logger::writeToLog("GPU renderer initialization failed, using CPU fallback");
            renderer.reset(); // Clean up failed renderer
        }
    }
#endif

    // Initialize sample loading infrastructure
    sampleLoader = std::make_unique<SampleLoaderService>();
    toastManager = std::make_unique<ToastManager>(*this);
    addChildComponent(*toastManager); // Hidden by default, shows toasts when needed
    
    // Phase 5 Performance HUD (initially hidden) - pass renderer for real GPU metrics
    perfHUD = std::make_unique<PerfHUD>(audioProcessor, renderer.get());
    addAndMakeVisible(*perfHUD);
    
    // Listen for HUD toggle parameter changes
    audioProcessor.getValueTreeState().addParameterListener(Params::ParameterIDs::showPerfHud, this);
    
    // Set initial HUD visibility based on parameter
    auto* hudParam = audioProcessor.getValueTreeState().getRawParameterValue(Params::ParameterIDs::showPerfHud);
    if (hudParam != nullptr) {
        perfHUD->setVisible(*hudParam >= 0.5f);
    }
    
    // Add spectral painting components
    addAndMakeVisible(loadButton);
    loadButton.onClick = [this]{ loadSampleButtonClicked(); };
    addAndMakeVisible(spectrogram);
    
    // Enable keyboard focus for 'H' key toggle
    setWantsKeyboardFocus(true);
    
    // Minimal sizing - no complex layouts needed
    setSize(1200, 800);
    setResizable(true, true);
    setResizeLimits(800, 600, 2400, 1600);
}

SpectralCanvasProEditor::~SpectralCanvasProEditor()
{
    // Clean up sample loading infrastructure
    sampleLoader.reset();
    toastManager.reset();
    
    // Ensure renderer shutdown before destruction
    if (renderer) 
        renderer->shutdown();
        
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
    topStrip->setBounds(topArea.removeFromLeft(topArea.getWidth() / 2));
    loadButton.setBounds(topArea.removeFromLeft(140).reduced(2));
    
    // Split remaining area between canvas and spectrogram
    auto leftArea = bounds.removeFromLeft(bounds.getWidth() / 2);
    canvasComponent->setBounds(leftArea);
    spectrogram.setBounds(bounds);
    
    // Position HUD in top-right corner with margin
    if (perfHUD) {
        const int margin = 10;
        perfHUD->setTopRightPosition(getWidth() - margin, 50);
    }
    
    // Size toast manager to cover entire editor
    if (toastManager) {
        toastManager->setBounds(getLocalBounds());
    }
    
    // Hook model/mask if available
    if (audioProcessor.getSpectralModel().isReady())
    {
        spectrogram.setModel(&audioProcessor.getSpectralModel());
        spectrogram.setEditableMask(&audioProcessor.getSpectralMask());
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

void SpectralCanvasProEditor::loadSampleButtonClicked()
{
    juce::Logger::writeToLog("UI: Load Sample button clicked");
    
    if (!sampleLoader || !toastManager)
    {
        juce::Logger::writeToLog("UI ERROR: Sample loading infrastructure not initialized");
        return;
    }
    
    // Use centralized sample loader with toast feedback
    sampleLoader->loadViaChooser(*this, [this](SampleLoaderService::Result result)
    {
        if (result.isSuccess() && result.sample)
        {
            auto& sample = *result.sample;
            
            juce::Logger::writeToLog("UI: Sample load successful: " + sample.filename + 
                                      " (" + juce::String(sample.durationSeconds, 2) + "s)");
            
            // Activate renderer with loaded sample
            bool rendererActivated = audioProcessor.activateSampleRenderer(sample.audio, sample.sampleRate);
            
            if (rendererActivated)
            {
                // Update UI to show loaded sample
                spectrogram.setModel(&audioProcessor.getSpectralModel());
                spectrogram.setEditableMask(&audioProcessor.getSpectralMask());
                spectrogram.repaint();
                
                // Show success toast
                toastManager->showSuccess(
                    "Loaded: " + sample.filename + " (" + 
                    juce::String(sample.durationSeconds, 1) + "s)");
                
                juce::Logger::writeToLog("RENDERER: Sample renderer activated successfully");
            }
            else
            {
                toastManager->showError("Sample loaded but renderer activation failed");
                juce::Logger::writeToLog("RENDERER ERROR: Failed to activate renderer after successful sample load");
            }
        }
        else
        {
            // Handle various error cases with appropriate toasts
            if (result.code != SampleLoaderService::ErrorCode::FileDialogCancelled)
            {
                toastManager->showError(result.errorMessage);
                juce::Logger::writeToLog("LOADER ERROR: Sample load failed: " + result.errorMessage);
            }
        }
    });
}

void SpectralCanvasProEditor::loadAudioFile(const juce::File& file)
{
    juce::Logger::writeToLog("UI: Loading audio file: " + file.getFullPathName());
    
    if (!sampleLoader || !toastManager)
    {
        juce::Logger::writeToLog("UI ERROR: Sample loading infrastructure not initialized");
        return;
    }
    
    // Use centralized sample loader
    sampleLoader->loadFromFile(file, [this](SampleLoaderService::Result result)
    {
        if (result.isSuccess() && result.sample)
        {
            auto& sample = *result.sample;
            
            juce::Logger::writeToLog("UI: Drag-and-drop load successful: " + sample.filename + 
                                      " (" + juce::String(sample.durationSeconds, 2) + "s)");
            
            // Activate renderer with loaded sample
            bool rendererActivated = audioProcessor.activateSampleRenderer(sample.audio, sample.sampleRate);
            
            if (rendererActivated)
            {
                // Update UI to show loaded sample
                spectrogram.setModel(&audioProcessor.getSpectralModel());
                spectrogram.setEditableMask(&audioProcessor.getSpectralMask());
                spectrogram.repaint();
                
                // Show success toast
                toastManager->showSuccess(
                    "Dropped: " + sample.filename + " (" + 
                    juce::String(sample.durationSeconds, 1) + "s)");
                
                juce::Logger::writeToLog("RENDERER: Sample renderer activated via drag-and-drop");
            }
            else
            {
                toastManager->showError("Sample loaded but renderer activation failed");
                juce::Logger::writeToLog("RENDERER ERROR: Failed to activate renderer after drag-and-drop load");
            }
        }
        else
        {
            // Show error toast for failed loads
            toastManager->showError(result.errorMessage);
            juce::Logger::writeToLog("LOADER ERROR: Drag-and-drop load failed: " + result.errorMessage);
        }
    });
}