#include "TopStrip.h"
#include "CanvasComponent.h"

TopStrip::TopStrip(SpectralCanvasProAudioProcessor& processor, CanvasComponent& canvas)
    : audioProcessor(processor), canvasComponent(canvas)
{
    setupControls();
}

TopStrip::~TopStrip()
{
}

void TopStrip::setupControls()
{
    // Left section - File operations
    loadSampleButton = std::make_unique<juce::TextButton>("Load Sample");
    loadSampleButton->addListener(this);
    addAndMakeVisible(*loadSampleButton);
    
    loadedSampleLabel.setText("No sample loaded", juce::dontSendNotification);
    loadedSampleLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    loadedSampleLabel.setFont(12.0f);
    addAndMakeVisible(loadedSampleLabel);
    
    // Center section - Mode controls
    modeSelector = std::make_unique<juce::ComboBox>("Mode");
    modeSelector->addItem("Synth", 1);
    modeSelector->addItem("Resynth", 2);
    modeSelector->addItem("Hybrid", 3);
    modeSelector->setSelectedId(1); // Default to Synth
    modeSelector->addListener(this);
    addAndMakeVisible(*modeSelector);
    
    modeLabel.setText("Mode", juce::dontSendNotification);
    modeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    modeLabel.setFont(12.0f);
    addAndMakeVisible(modeLabel);
    
    blendSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
    blendSlider->setRange(0.0, 1.0, 0.01);
    blendSlider->setValue(0.0); // Full synth by default
    blendSlider->addListener(this);
    blendSlider->setEnabled(false); // Only enabled in Hybrid mode
    addAndMakeVisible(*blendSlider);
    
    blendLabel.setText("Blend", juce::dontSendNotification);
    blendLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    blendLabel.setFont(12.0f);
    addAndMakeVisible(blendLabel);
    
    // Right section - Brush controls
    brushTypeSelector = std::make_unique<juce::ComboBox>("Brush Type");
    brushTypeSelector->addItem("Paint", 1);
    brushTypeSelector->addItem("Quantize", 2);
    brushTypeSelector->addItem("Erase", 3);
    brushTypeSelector->addItem("Comb", 4);
    brushTypeSelector->setSelectedId(1); // Paint default
    brushTypeSelector->addListener(this);
    addAndMakeVisible(*brushTypeSelector);
    
    brushTypeLabel.setText("Brush", juce::dontSendNotification);
    brushTypeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    brushTypeLabel.setFont(12.0f);
    addAndMakeVisible(brushTypeLabel);
    
    brushSizeSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
    brushSizeSlider->setRange(1.0, 128.0, 1.0);
    brushSizeSlider->setValue(16.0); // Default size
    brushSizeSlider->addListener(this);
    addAndMakeVisible(*brushSizeSlider);
    
    brushSizeLabel.setText("Size", juce::dontSendNotification);
    brushSizeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    brushSizeLabel.setFont(12.0f);
    addAndMakeVisible(brushSizeLabel);
    
    brushStrengthSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
    brushStrengthSlider->setRange(0.0, 1.0, 0.01);
    brushStrengthSlider->setValue(0.7); // Default strength
    brushStrengthSlider->addListener(this);
    addAndMakeVisible(*brushStrengthSlider);
    
    brushStrengthLabel.setText("Strength", juce::dontSendNotification);
    brushStrengthLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    brushStrengthLabel.setFont(12.0f);
    addAndMakeVisible(brushStrengthLabel);
    
    // Far right - Advanced options
    snapToScaleButton = std::make_unique<juce::ToggleButton>("Snap to Scale");
    snapToScaleButton->addListener(this);
    addAndMakeVisible(*snapToScaleButton);
    
    tuningSelector = std::make_unique<juce::ComboBox>("Tuning");
    tuningSelector->addItem("12-TET", 1);
    tuningSelector->addItem("Just", 2);
    tuningSelector->addItem("Custom", 3);
    tuningSelector->setSelectedId(1);
    tuningSelector->addListener(this);
    addAndMakeVisible(*tuningSelector);
    
    fastPaintButton = std::make_unique<juce::ToggleButton>("Fast Paint");
    fastPaintButton->addListener(this);
    addAndMakeVisible(*fastPaintButton);
    
    // Connect to parameters
    updateControlStates();
}

void TopStrip::paint(juce::Graphics& g)
{
    // Semi-transparent dark background with nebula accent
    g.setColour(juce::Colour(0xaa000000));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);
    
    // Subtle accent border
    g.setColour(getNebulaAccentColor().withAlpha(0.3f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 8.0f, 1.0f);
    
    // Title text
    g.setColour(juce::Colours::white.withAlpha(0.9f));
    g.setFont(juce::FontOptions().withPointHeight(16.0f).withStyle("Bold"));
    g.drawText("SpectralCanvas Pro v2", 
               juce::Rectangle<int>(margin, 4, 200, 20), 
               juce::Justification::centredLeft);
    
    // Service indicators (matching mockup)
    const int indicatorY = 8;
    const int indicatorSize = 8;
    const int rightMargin = getWidth() - margin;
    
    // Service State
    g.setColour(juce::Colours::lightgreen);
    g.fillEllipse(rightMargin - 140, indicatorY, indicatorSize, indicatorSize);
    g.setColour(juce::Colours::white);
    g.setFont(10.0f);
    g.drawText("Service State", rightMargin - 130, indicatorY - 2, 80, 12, juce::Justification::centredLeft);
    
    // Service Health  
    g.setColour(juce::Colours::lightgreen);
    g.fillEllipse(rightMargin - 70, indicatorY, indicatorSize, indicatorSize);
    g.drawText("Service Health", rightMargin - 60, indicatorY - 2, 80, 12, juce::Justification::centredLeft);
}

void TopStrip::resized()
{
    const int width = getWidth();
    const int height = getHeight();
    const int controlHeight = 24;
    const int labelHeight = 14;
    const int sectionSpacing = 40;
    
    int x = margin;
    int y = 28; // Below title
    
    // Left section - File operations (x: margin to ~180)
    loadSampleButton->setBounds(x, y, 100, controlHeight);
    x += 110;
    
    loadedSampleLabel.setBounds(x, y + 2, 150, labelHeight);
    x += sectionSpacing + 120; // Move to next section
    
    // Center section - Mode controls (x: ~300 to ~500)
    modeLabel.setBounds(x, y - 18, 50, labelHeight);
    modeSelector->setBounds(x, y, 80, controlHeight);
    x += 90;
    
    blendLabel.setBounds(x, y - 18, 50, labelHeight);
    blendSlider->setBounds(x, y, 80, controlHeight);
    x += sectionSpacing + 70; // Move to next section
    
    // Right section - Brush controls (x: ~540 to ~end)
    brushTypeLabel.setBounds(x, y - 18, 50, labelHeight);
    brushTypeSelector->setBounds(x, y, 80, controlHeight);
    x += 90;
    
    brushSizeLabel.setBounds(x, y - 18, 50, labelHeight);
    brushSizeSlider->setBounds(x, y, 60, controlHeight);
    x += 70;
    
    brushStrengthLabel.setBounds(x, y - 18, 60, labelHeight);
    brushStrengthSlider->setBounds(x, y, 60, controlHeight);
    x += 80;
    
    // Far right - Advanced options
    if (x < width - 200) // Only show if we have space
    {
        snapToScaleButton->setBounds(x, y - 4, 90, 16);
        x += 100;
        
        tuningSelector->setBounds(x, y, 70, controlHeight);
        x += 80;
        
        fastPaintButton->setBounds(x, y - 4, 80, 16);
    }
}

void TopStrip::buttonClicked(juce::Button* button)
{
    if (button == loadSampleButton.get())
    {
        loadSampleFile();
    }
    else if (button == snapToScaleButton.get())
    {
        canvasComponent.setSnapToScale(snapToScaleButton->getToggleState());
        
        if (auto* param = audioProcessor.getValueTreeState().getParameter("grid_on"))
        {
            param->setValueNotifyingHost(snapToScaleButton->getToggleState() ? 1.0f : 0.0f);
        }
    }
    else if (button == fastPaintButton.get())
    {
        if (auto* param = audioProcessor.getValueTreeState().getParameter("fast_paint_mode"))
        {
            param->setValueNotifyingHost(fastPaintButton->getToggleState() ? 1.0f : 0.0f);
        }
    }
}

void TopStrip::sliderValueChanged(juce::Slider* slider)
{
    if (slider == blendSlider.get())
    {
        float blendValue = (float)blendSlider->getValue();
        canvasComponent.setBlendAmount(blendValue);
        
        if (auto* param = audioProcessor.getValueTreeState().getParameter("blend"))
        {
            param->setValueNotifyingHost(blendValue);
        }
    }
    else if (slider == brushSizeSlider.get())
    {
        float sizeValue = (float)brushSizeSlider->getValue();
        canvasComponent.setBrushSize(sizeValue);
        
        if (auto* param = audioProcessor.getValueTreeState().getParameter("brush_size"))
        {
            param->setValueNotifyingHost(sizeValue / 128.0f);
        }
    }
    else if (slider == brushStrengthSlider.get())
    {
        float strengthValue = (float)brushStrengthSlider->getValue();
        canvasComponent.setBrushStrength(strengthValue);
        
        if (auto* param = audioProcessor.getValueTreeState().getParameter("brush_strength"))
        {
            param->setValueNotifyingHost(strengthValue);
        }
    }
}

void TopStrip::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == modeSelector.get())
    {
        int modeId = modeSelector->getSelectedId();
        int modeValue = modeId - 1; // Convert from 1-based to 0-based
        
        canvasComponent.setEngineMode(modeValue);
        
        // Enable/disable blend control based on mode
        bool isHybridMode = (modeValue == 2);
        blendSlider->setEnabled(isHybridMode);
        blendLabel.setColour(juce::Label::textColourId, 
                            isHybridMode ? juce::Colours::white : juce::Colours::lightgrey);
        
        if (auto* param = audioProcessor.getValueTreeState().getParameter("mode"))
        {
            param->setValueNotifyingHost((float)modeValue / 2.0f);
        }
    }
    else if (comboBoxThatHasChanged == brushTypeSelector.get())
    {
        int brushType = brushTypeSelector->getSelectedId() - 1; // Convert to 0-based
        canvasComponent.setBrushType(brushType);
        
        if (auto* param = audioProcessor.getValueTreeState().getParameter("brush_type"))
        {
            param->setValueNotifyingHost((float)brushType / 3.0f);
        }
    }
    else if (comboBoxThatHasChanged == tuningSelector.get())
    {
        int tuningId = tuningSelector->getSelectedId() - 1;
        if (auto* param = audioProcessor.getValueTreeState().getParameter("tuning_id"))
        {
            param->setValueNotifyingHost((float)tuningId / 10.0f);
        }
    }
}

void TopStrip::updateControlStates()
{
    // Sync controls with parameter values
    auto& params = audioProcessor.getValueTreeState();
    
    if (auto* modeParam = params.getParameter("mode"))
    {
        int mode = (int)(modeParam->getValue() * 2.0f);
        modeSelector->setSelectedId(mode + 1, juce::dontSendNotification);
    }
    
    if (auto* blendParam = params.getParameter("blend"))
    {
        blendSlider->setValue(blendParam->getValue(), juce::dontSendNotification);
    }
    
    if (auto* brushTypeParam = params.getParameter("brush_type"))
    {
        int brushType = (int)(brushTypeParam->getValue() * 3.0f);
        brushTypeSelector->setSelectedId(brushType + 1, juce::dontSendNotification);
    }
    
    if (auto* brushSizeParam = params.getParameter("brush_size"))
    {
        brushSizeSlider->setValue(brushSizeParam->getValue() * 128.0f, juce::dontSendNotification);
    }
    
    if (auto* brushStrengthParam = params.getParameter("brush_strength"))
    {
        brushStrengthSlider->setValue(brushStrengthParam->getValue(), juce::dontSendNotification);
    }
}

void TopStrip::loadSampleFile()
{
    auto chooser = std::make_shared<juce::FileChooser>("Load Audio Sample",
                                                       juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                                                       "*.wav;*.aiff;*.flac;*.mp3");
    
    chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this, chooser](const juce::FileChooser& fc)
        {
            if (auto selectedFile = fc.getResult(); selectedFile.existsAsFile())
            {
                // TODO: Send to AsyncSampleLoader
                juce::Logger::writeToLog("Loading sample: " + selectedFile.getFullPathName());
                
                // Update UI on message thread
                juce::MessageManager::callAsync([this, selectedFile]()
                {
                    loadedSampleLabel.setText(selectedFile.getFileNameWithoutExtension(), 
                                             juce::dontSendNotification);
                    
                    // Switch to Resynth mode
                    modeSelector->setSelectedId(2, juce::sendNotificationSync);
                });
            }
        });
}