#include "SpectralCanvasProEditor.h"
#include "core/Params.h"

// Forward declarations for nested panel classes
class SpectralCanvasProEditor::SourceEnginePanel : public juce::Component
{
public:
    SourceEnginePanel(juce::AudioProcessorValueTreeState& apvts) : apvts_(apvts)
    {
        // Mode slider: Synth ↔ Hybrid ↔ Resynth
        modeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
        modeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        modeSlider.setRange(0.0, 2.0, 1.0);
        modeSlider.setValue(0.0); // Start in Synth mode
        addAndMakeVisible(modeSlider);
        
        modeLabel.setText("SOURCE ENGINE", juce::dontSendNotification);
        modeLabel.setFont(juce::FontOptions().withPointHeight(10.0f).withStyle("Bold"));
        modeLabel.setColour(juce::Label::textColourId, juce::Colour(0x80ffffff));
        addAndMakeVisible(modeLabel);
        
        synthLabel.setText("SYNTH", juce::dontSendNotification);
        synthLabel.setFont(juce::FontOptions(9.0f));
        synthLabel.setColour(juce::Label::textColourId, juce::Colour(0xffffffff));
        addAndMakeVisible(synthLabel);
        
        hybridLabel.setText("HYBRID", juce::dontSendNotification);
        hybridLabel.setFont(juce::FontOptions(8.0f));
        hybridLabel.setColour(juce::Label::textColourId, juce::Colour(0xc0ffffff));
        hybridLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(hybridLabel);
        
        resynthLabel.setText("RESYNTH", juce::dontSendNotification);
        resynthLabel.setFont(juce::FontOptions(9.0f));
        resynthLabel.setColour(juce::Label::textColourId, juce::Colour(0xffffffff));
        addAndMakeVisible(resynthLabel);
        
        // Preset dropdown for starter canvases
        presetCombo.setTextWhenNoChoicesAvailable("Loading...");
        presetCombo.setTextWhenNothingSelected("Starter Canvas");
        presetCombo.addItem("Crystalline Arp", 1);
        presetCombo.addItem("Vocal Halo", 2);  
        presetCombo.addItem("Percussive Carver", 3);
        presetCombo.addItem("Ambient Pad", 4);
        presetCombo.setSelectedId(0); // No selection initially
        addAndMakeVisible(presetCombo);
        
        modeAttachment = std::make_unique<juce::SliderParameterAttachment>(
            *apvts_.getParameter(Params::ParameterIDs::mode), modeSlider);
    }
    
    void paint(juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();
        juce::Colour a = juce::Colour::fromFloatRGBA(0,0,0,0.42f);
        juce::Colour b = juce::Colour::fromFloatRGBA(0,0,0,0.28f);
        g.setGradientFill({ a, r.getTopLeft(), b, r.getBottomLeft(), false });
        g.fillRoundedRectangle(r, 10.0f);
        g.setColour(juce::Colour::fromFloatRGBA(1,1,1,0.12f));
        g.drawRoundedRectangle(r, 10.0f, 1.0f);
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds().reduced(12);
        
        // Preset dropdown at top
        presetCombo.setBounds(bounds.removeFromTop(22));
        bounds.removeFromTop(8);
        
        modeLabel.setBounds(bounds.removeFromTop(15));
        bounds.removeFromTop(5);
        
        auto sliderArea = bounds.removeFromTop(30);
        synthLabel.setBounds(sliderArea.removeFromLeft(50));
        resynthLabel.setBounds(sliderArea.removeFromRight(60));
        
        // Add HYBRID label under the center of the slider
        auto centerArea = sliderArea.removeFromBottom(12);
        hybridLabel.setBounds(centerArea);
        
        modeSlider.setBounds(sliderArea.reduced(5, 0));
    }
    
private:
    juce::AudioProcessorValueTreeState& apvts_;
    juce::Slider modeSlider;
    juce::Label modeLabel, synthLabel, hybridLabel, resynthLabel;
    juce::ComboBox presetCombo;
    std::unique_ptr<juce::SliderParameterAttachment> modeAttachment;
};

class SpectralCanvasProEditor::BrushControlsPanel : public juce::Component
{
public:
    BrushControlsPanel(juce::AudioProcessorValueTreeState& apvts) : apvts_(apvts)
    {
        setupControl(sizeSlider, sizeLabel, "Size", 1.0f, 128.0f, 16.0f);
        setupControl(strengthSlider, strengthLabel, "Strength", 0.0f, 1.0f, 0.7f);
        setupControl(decaySlider, decayLabel, "Decay", 0.0f, 1.0f, 0.5f);
        setupControl(intensitySlider, intensityLabel, "Intensity", 0.0f, 1.0f, 0.88f);
        
        titleLabel.setText("Brush Controls", juce::dontSendNotification);
        titleLabel.setFont(juce::FontOptions().withPointHeight(11.0f).withStyle("Bold"));
        titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xffffffff));
        addAndMakeVisible(titleLabel);
        
        // Parameter attachments
        sizeAttachment = std::make_unique<juce::SliderParameterAttachment>(
            *apvts_.getParameter(Params::ParameterIDs::brushSize), sizeSlider);
        strengthAttachment = std::make_unique<juce::SliderParameterAttachment>(
            *apvts_.getParameter(Params::ParameterIDs::brushStrength), strengthSlider);
    }
    
    void paint(juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();
        juce::Colour a = juce::Colour::fromFloatRGBA(0,0,0,0.42f);
        juce::Colour b = juce::Colour::fromFloatRGBA(0,0,0,0.28f);
        g.setGradientFill({ a, r.getTopLeft(), b, r.getBottomLeft(), false });
        g.fillRoundedRectangle(r, 10.0f);
        g.setColour(juce::Colour::fromFloatRGBA(1,1,1,0.12f));
        g.drawRoundedRectangle(r, 10.0f, 1.0f);
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds().reduced(8);
        titleLabel.setBounds(bounds.removeFromTop(20));
        
        auto controlsArea = bounds;
        int controlWidth = controlsArea.getWidth() / 4;
        
        layoutControl(sizeSlider, sizeLabel, controlsArea.removeFromLeft(controlWidth));
        layoutControl(strengthSlider, strengthLabel, controlsArea.removeFromLeft(controlWidth));
        layoutControl(decaySlider, decayLabel, controlsArea.removeFromLeft(controlWidth));
        layoutControl(intensitySlider, intensityLabel, controlsArea);
    }
    
private:
    juce::AudioProcessorValueTreeState& apvts_;
    juce::Slider sizeSlider, strengthSlider, decaySlider, intensitySlider;
    juce::Label titleLabel, sizeLabel, strengthLabel, decayLabel, intensityLabel;
    std::unique_ptr<juce::SliderParameterAttachment> sizeAttachment;
    std::unique_ptr<juce::SliderParameterAttachment> strengthAttachment;
    
    void setupControl(juce::Slider& slider, juce::Label& label, const juce::String& name, 
                     float min, float max, float defaultVal)
    {
        slider.setSliderStyle(juce::Slider::LinearVertical);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 16);
        slider.setRange(min, max, 0.01f);
        slider.setValue(defaultVal);
        addAndMakeVisible(slider);
        
        label.setText(name, juce::dontSendNotification);
        label.setFont(juce::FontOptions(9.0f));
        label.setColour(juce::Label::textColourId, juce::Colour(0xc0ffffff));
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
    }
    
    void layoutControl(juce::Slider& slider, juce::Label& label, juce::Rectangle<int> area)
    {
        area = area.reduced(4);
        label.setBounds(area.removeFromBottom(12));
        slider.setBounds(area);
    }
};

class SpectralCanvasProEditor::PostFXPanel : public juce::Component
{
public:
    PostFXPanel(juce::AudioProcessorValueTreeState& apvts) : apvts_(apvts)
    {
        titleLabel.setText("POST-FX CHAIN", juce::dontSendNotification);
        titleLabel.setFont(juce::FontOptions().withPointHeight(10.0f).withStyle("Bold"));
        titleLabel.setColour(juce::Label::textColourId, juce::Colour(0x80ffffff));
        addAndMakeVisible(titleLabel);
        
        filterLabel.setText("Z-Plane Filter (Coming Soon)", juce::dontSendNotification);
        filterLabel.setFont(juce::FontOptions(9.0f));
        filterLabel.setColour(juce::Label::textColourId, juce::Colour(0x60ffffff));
        addAndMakeVisible(filterLabel);
    }
    
    void paint(juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();
        juce::Colour a = juce::Colour::fromFloatRGBA(0,0,0,0.42f);
        juce::Colour b = juce::Colour::fromFloatRGBA(0,0,0,0.28f);
        g.setGradientFill({ a, r.getTopLeft(), b, r.getBottomLeft(), false });
        g.fillRoundedRectangle(r, 10.0f);
        g.setColour(juce::Colour::fromFloatRGBA(1,1,1,0.12f));
        g.drawRoundedRectangle(r, 10.0f, 1.0f);
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds().reduced(12);
        titleLabel.setBounds(bounds.removeFromTop(15));
        bounds.removeFromTop(5);
        filterLabel.setBounds(bounds.removeFromTop(15));
    }
    
private:
    juce::AudioProcessorValueTreeState& apvts_;
    juce::Label titleLabel, filterLabel;
};

SpectralCanvasProEditor::SpectralCanvasProEditor(SpectralCanvasProAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Canvas first (z-order underneath)
    canvasComponent = std::make_unique<CanvasComponent>(audioProcessor);
    addAndMakeVisible(*canvasComponent);
    
    // ✅ Pass APVTS, not getParameters()
    auto& apvts = audioProcessor.apvts;
    
    sourceEnginePanel = std::make_unique<SourceEnginePanel>(apvts);
    addAndMakeVisible(*sourceEnginePanel);
    
    brushControlsPanel = std::make_unique<BrushControlsPanel>(apvts);
    addAndMakeVisible(*brushControlsPanel);
    
    postFXPanel = std::make_unique<PostFXPanel>(apvts);
    addAndMakeVisible(*postFXPanel);
    
    setSize(1200, 800);
    setResizable(true, true);
    setResizeLimits(900, 600, 2400, 1600);
}

SpectralCanvasProEditor::~SpectralCanvasProEditor()
{
    sourceEnginePanel.reset();
    brushControlsPanel.reset();
    postFXPanel.reset();
    canvasComponent.reset();
}

void SpectralCanvasProEditor::paint(juce::Graphics& g)
{
    // Canvas component handles all rendering
    // This paint method is only called if canvas fails to render
    g.fillAll(juce::Colour(0xff0a0a0f)); // Dark fallback background
}

void SpectralCanvasProEditor::resized()
{
    layoutControlPanels();
}

void SpectralCanvasProEditor::layoutControlPanels()
{
    auto bounds = getLocalBounds();
    
    // Canvas gets 80% of viewport (full-bleed with overlaid controls)
    auto canvasArea = getCanvasArea();
    canvasComponent->setBounds(canvasArea);
    
    // Control strip at bottom (matching mockup layout)
    auto controlStrip = getControlStripArea();
    
    // Left side: Source Engine (Synth ↔ Resynth slider) + Preset dropdown
    auto leftPanel = controlStrip.removeFromLeft(320);
    sourceEnginePanel->setBounds(leftPanel.reduced(8));
    
    // Right side: Post-FX Chain
    auto rightPanel = controlStrip.removeFromRight(200);
    postFXPanel->setBounds(rightPanel.reduced(8));
    
    // Center: Brush Controls
    brushControlsPanel->setBounds(controlStrip.reduced(8));
}

juce::Rectangle<int> SpectralCanvasProEditor::getCanvasArea() const
{
    // Canvas fills entire area (controls float on top)
    return getLocalBounds();
}

juce::Rectangle<int> SpectralCanvasProEditor::getControlStripArea() const
{
    auto bounds = getLocalBounds();
    // Bottom strip (96px for more canvas space)
    return bounds.removeFromBottom(96).reduced(16, 12);
}