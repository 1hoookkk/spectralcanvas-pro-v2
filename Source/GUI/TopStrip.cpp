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
    loadedSampleLabel.setFont(juce::FontOptions().withPointHeight(12.0f));
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
    modeLabel.setFont(juce::FontOptions().withPointHeight(12.0f));
    addAndMakeVisible(modeLabel);
    
    blendSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
    blendSlider->setRange(0.0, 1.0, 0.01);
    blendSlider->setValue(0.0); // Full synth by default
    blendSlider->addListener(this);
    blendSlider->setEnabled(false); // Only enabled in Hybrid mode
    addAndMakeVisible(*blendSlider);
    
    blendLabel.setText("Blend", juce::dontSendNotification);
    blendLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    blendLabel.setFont(juce::FontOptions().withPointHeight(12.0f));
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
    brushTypeLabel.setFont(juce::FontOptions().withPointHeight(12.0f));
    addAndMakeVisible(brushTypeLabel);
    
    brushSizeSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
    brushSizeSlider->setRange(1.0, 128.0, 1.0);
    brushSizeSlider->setValue(16.0); // Default size
    brushSizeSlider->addListener(this);
    addAndMakeVisible(*brushSizeSlider);
    
    brushSizeLabel.setText("Size", juce::dontSendNotification);
    brushSizeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    brushSizeLabel.setFont(juce::FontOptions().withPointHeight(12.0f));
    addAndMakeVisible(brushSizeLabel);
    
    brushStrengthSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
    brushStrengthSlider->setRange(0.0, 1.0, 0.01);
    brushStrengthSlider->setValue(0.7); // Default strength
    brushStrengthSlider->addListener(this);
    addAndMakeVisible(*brushStrengthSlider);
    
    brushStrengthLabel.setText("Strength", juce::dontSendNotification);
    brushStrengthLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    brushStrengthLabel.setFont(juce::FontOptions().withPointHeight(12.0f));
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
    auto bounds = getLocalBounds();
    
    // Classic Windows 95 control panel background
    g.setColour(juce::Colour(0xffc0c0c0)); // Win95 button face color
    g.fillRect(bounds);
    
    // Raised 3D border (outer highlight, inner shadow)
    g.setColour(juce::Colours::white); // Highlight
    g.drawLine(0, 0, getWidth(), 0, 2);
    g.drawLine(0, 0, 0, getHeight(), 2);
    
    g.setColour(juce::Colour(0xff808080)); // Shadow
    g.drawLine(getWidth()-1, 1, getWidth()-1, getHeight(), 1);
    g.drawLine(1, getHeight()-1, getWidth(), getHeight()-1, 1);
    
    g.setColour(juce::Colour(0xff404040)); // Dark shadow
    g.drawLine(getWidth()-2, 2, getWidth()-2, getHeight()-1, 1);
    g.drawLine(2, getHeight()-2, getWidth()-1, getHeight()-2, 1);
    
    // Title text - classic MS Sans Serif style
    g.setColour(juce::Colours::black);
    g.setFont(juce::FontOptions().withPointHeight(14.0f).withStyle("Bold"));
    g.drawText("SpectralCanvas Pro v2.0", 
               juce::Rectangle<int>(10, 8, 200, 18), 
               juce::Justification::centredLeft);
    
    // Status indicators - simple text labels in classic style
    g.setFont(juce::FontOptions().withPointHeight(11.0f));
    g.setColour(juce::Colours::darkgreen);
    g.drawText("Audio: ON", getWidth() - 150, 8, 60, 16, juce::Justification::centredLeft);
    g.drawText("GPU: D3D11", getWidth() - 85, 8, 70, 16, juce::Justification::centredLeft);
}

void TopStrip::resized()
{
    const int width = getWidth();
    const int controlHeight = 22; // Classic button height
    const int labelHeight = 16;
    const int topMargin = 30; // Space below title
    const int leftMargin = 12;
    const int sectionSpacing = 25;
    
    int x = leftMargin;
    int y = topMargin;
    
    // File operations section
    loadSampleButton->setBounds(x, y, 85, controlHeight);
    x += 90;
    
    loadedSampleLabel.setBounds(x, y + 3, 140, labelHeight);
    x += 160; // Move to next section
    
    // Mode controls section  
    modeLabel.setBounds(x, y - 18, 50, labelHeight);
    modeSelector->setBounds(x, y, 75, controlHeight);
    x += 85;
    
    blendLabel.setBounds(x, y - 18, 50, labelHeight);
    blendSlider->setBounds(x, y, 70, controlHeight);
    x += 85;
    
    // Brush controls section
    brushTypeLabel.setBounds(x, y - 18, 50, labelHeight);
    brushTypeSelector->setBounds(x, y, 75, controlHeight);
    x += 85;
    
    brushSizeLabel.setBounds(x, y - 18, 40, labelHeight);
    brushSizeSlider->setBounds(x, y, 55, controlHeight);
    x += 65;
    
    brushStrengthLabel.setBounds(x, y - 18, 60, labelHeight);
    brushStrengthSlider->setBounds(x, y, 55, controlHeight);
    x += 75;
    
    // Advanced options (if space available)
    if (x < width - 180)
    {
        snapToScaleButton->setBounds(x, y + 2, 85, 18);
        x += 95;
        
        if (x < width - 90)
        {
            tuningSelector->setBounds(x, y, 65, controlHeight);
            x += 75;
            
            if (x < width - 80)
            {
                fastPaintButton->setBounds(x, y + 2, 70, 18);
            }
        }
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
                juce::Logger::writeToLog("Loading sample: " + selectedFile.getFullPathName());
                
                // Load sample through audio processor (UI thread safe)
                bool loadSuccess = audioProcessor.loadSampleFile(selectedFile);
                
                // Update UI on message thread
                juce::MessageManager::callAsync([this, selectedFile, loadSuccess]()
                {
                    if (loadSuccess)
                    {
                        loadedSampleLabel.setText(selectedFile.getFileNameWithoutExtension(), 
                                                 juce::dontSendNotification);
                        loadedSampleLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
                        
                        // Switch to Resynth mode
                        modeSelector->setSelectedId(2, juce::sendNotificationSync);
                        
                        juce::Logger::writeToLog("Sample loaded successfully: " + 
                                                 juce::String(audioProcessor.getSampleLoader().getNumSpectralFrames()) + 
                                                 " spectral frames");
                    }
                    else
                    {
                        loadedSampleLabel.setText("Load failed!", juce::dontSendNotification);
                        loadedSampleLabel.setColour(juce::Label::textColourId, juce::Colours::red);
                        
                        juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,
                                                               "Sample Load Error",
                                                               "Could not load the selected audio file. Please check the file format and try again.");
                    }
                });
            }
        });
}