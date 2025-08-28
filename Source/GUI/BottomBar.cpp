#include "BottomBar.h"
#include "CanvasComponent.h"

BottomBar::BottomBar(SpectralCanvasProAudioProcessor& processor, CanvasComponent& canvas)
    : audioProcessor(processor), canvasComponent(canvas)
{
    setupControls();
}

BottomBar::~BottomBar()
{
}

void BottomBar::setupControls()
{
    // Left section - Transport controls
    playButton = std::make_unique<juce::TextButton>("▶");
    playButton->addListener(this);
    playButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d2d2d));
    playButton->setColour(juce::TextButton::textColourOnId, getNebulaAccentColor());
    addAndMakeVisible(*playButton);
    
    stopButton = std::make_unique<juce::TextButton>("⏹");
    stopButton->addListener(this);
    stopButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d2d2d));
    addAndMakeVisible(*stopButton);
    
    loopButton = std::make_unique<juce::ToggleButton>("🔄");
    loopButton->addListener(this);
    addAndMakeVisible(*loopButton);
    
    // Center section - Playback controls
    speedSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight);
    speedSlider->setRange(0.25, 4.0, 0.01);
    speedSlider->setValue(1.0);
    speedSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    speedSlider->addListener(this);
    addAndMakeVisible(*speedSlider);
    
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    speedLabel.setFont(12.0f);
    addAndMakeVisible(speedLabel);
    
    gainSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight);
    gainSlider->setRange(-24.0, 24.0, 0.1);
    gainSlider->setValue(0.0);
    gainSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    gainSlider->addListener(this);
    addAndMakeVisible(*gainSlider);
    
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    gainLabel.setFont(12.0f);
    addAndMakeVisible(gainLabel);
    
    // Right section - Performance metrics (matching mockup)
    latencyLabel.setText("Latency: 17 ms", juce::dontSendNotification);
    latencyLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    latencyLabel.setFont(11.0f);
    addAndMakeVisible(latencyLabel);
    
    fpsLabel.setText("60 fps", juce::dontSendNotification);
    fpsLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    fpsLabel.setFont(11.0f);
    addAndMakeVisible(fpsLabel);
    
    cpuLabel.setText("CPU: 12%", juce::dontSendNotification);
    cpuLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    cpuLabel.setFont(11.0f);
    addAndMakeVisible(cpuLabel);
    
    fftSizeLabel.setText("FFT Size: 4096", juce::dontSendNotification);
    fftSizeLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    fftSizeLabel.setFont(11.0f);
    addAndMakeVisible(fftSizeLabel);
    
    // Service health indicators
    serviceHealthLabel.setText("Service Health", juce::dontSendNotification);
    serviceHealthLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    serviceHealthLabel.setFont(10.0f);
    addAndMakeVisible(serviceHealthLabel);
    
    addAndMakeVisible(audioHealthIndicator);
    addAndMakeVisible(gpuHealthIndicator);
}

void BottomBar::paint(juce::Graphics& g)
{
    // Semi-transparent dark background with nebula accent
    g.setColour(juce::Colour(0xaa000000));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 6.0f);
    
    // Subtle accent border
    g.setColour(getNebulaAccentColor().withAlpha(0.3f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 6.0f, 1.0f);
    
    // Health indicators
    const int rightX = getWidth() - margin - 100;
    const int indicatorY = 20;
    
    // Audio service indicator
    drawHealthIndicator(g, juce::Rectangle<int>(rightX, indicatorY, indicatorSize, indicatorSize), audioServiceOk);
    g.setColour(juce::Colours::white);
    g.setFont(10.0f);
    g.drawText("Audio", rightX + indicatorSize + 4, indicatorY - 2, 30, 12, juce::Justification::centredLeft);
    
    // GPU service indicator  
    drawHealthIndicator(g, juce::Rectangle<int>(rightX + 50, indicatorY, indicatorSize, indicatorSize), gpuServiceOk);
    g.drawText("GPU", rightX + 50 + indicatorSize + 4, indicatorY - 2, 30, 12, juce::Justification::centredLeft);
}

void BottomBar::resized()
{
    const int width = getWidth();
    const int height = getHeight();
    const int controlHeight = 20;
    const int labelHeight = 12;
    
    int x = margin;
    int y = (height - controlHeight) / 2;
    
    // Left section - Transport controls
    playButton->setBounds(x, y - 2, 30, 24);
    x += 35;
    
    stopButton->setBounds(x, y - 2, 30, 24);  
    x += 35;
    
    loopButton->setBounds(x, y - 2, 30, 24);
    x += 50; // More space before next section
    
    // Center section - Playback controls
    speedLabel.setBounds(x, y - 15, 40, labelHeight);
    speedSlider->setBounds(x, y, 100, controlHeight);
    x += 120;
    
    gainLabel.setBounds(x, y - 15, 40, labelHeight);
    gainSlider->setBounds(x, y, 100, controlHeight);
    x += 140; // Space for metrics
    
    // Right section - Performance metrics
    const int metricsX = width - 250;
    const int metricsSpacing = 60;
    
    latencyLabel.setBounds(metricsX, y - 8, 80, labelHeight);
    fpsLabel.setBounds(metricsX, y + 4, 40, labelHeight);
    
    cpuLabel.setBounds(metricsX + metricsSpacing, y - 8, 60, labelHeight);
    fftSizeLabel.setBounds(metricsX + metricsSpacing, y + 4, 80, labelHeight);
    
    // Service health area is handled in paint()
    serviceHealthLabel.setBounds(width - 100, y - 15, 80, labelHeight);
}

void BottomBar::buttonClicked(juce::Button* button)
{
    if (button == playButton.get())
    {
        isPlaying = !isPlaying;
        playButton->setButtonText(isPlaying ? "⏸" : "▶");
        playButton->setColour(juce::TextButton::textColourOnId, 
                             isPlaying ? juce::Colours::orange : getNebulaAccentColor());
        
        // TODO: Start/stop playback in audio engine
        juce::Logger::writeToLog(isPlaying ? "Starting playback" : "Stopping playback");
    }
    else if (button == stopButton.get())
    {
        isPlaying = false;
        playButton->setButtonText("▶");
        playButton->setColour(juce::TextButton::textColourOnId, getNebulaAccentColor());
        
        // TODO: Stop and reset playback position
        juce::Logger::writeToLog("Stop pressed");
    }
    else if (button == loopButton.get())
    {
        isLooping = loopButton->getToggleState();
        // TODO: Set loop mode in audio engine
        juce::Logger::writeToLog(isLooping ? "Loop enabled" : "Loop disabled");
    }
}

void BottomBar::sliderValueChanged(juce::Slider* slider)
{
    if (slider == speedSlider.get())
    {
        float speed = (float)speedSlider->getValue();
        if (auto* param = audioProcessor.getValueTreeState().getParameter("respeed"))
        {
            param->setValueNotifyingHost((speed - 0.25f) / 3.75f); // Normalize to 0-1
        }
    }
    else if (slider == gainSlider.get())
    {
        float gainDb = (float)gainSlider->getValue();
        // TODO: Apply gain to audio processor
        juce::Logger::writeToLog("Gain: " + juce::String(gainDb, 1) + " dB");
    }
}

void BottomBar::updateMetrics(float fps, float cpuPercent, float latencyMs)
{
    currentFPS = fps;
    currentCPU = cpuPercent;
    currentLatency = latencyMs;
    
    fpsLabel.setText(juce::String((int)fps) + " fps", juce::dontSendNotification);
    cpuLabel.setText("CPU: " + juce::String((int)cpuPercent) + "%", juce::dontSendNotification);
    latencyLabel.setText("Latency: " + juce::String(latencyMs, 0) + " ms", juce::dontSendNotification);
}

void BottomBar::updateFFTSize(int fftSize)
{
    currentFFTSize = fftSize;
    fftSizeLabel.setText("FFT Size: " + juce::String(fftSize), juce::dontSendNotification);
}

void BottomBar::updateServiceHealth(bool audioOk, bool gpuOk)
{
    audioServiceOk = audioOk;
    gpuServiceOk = gpuOk;
    repaint(); // Redraw health indicators
}

void BottomBar::updateTransportStates()
{
    // Sync transport controls with audio processor state
    // TODO: Get actual playback state from processor
}

void BottomBar::drawHealthIndicator(juce::Graphics& g, juce::Rectangle<int> area, bool isHealthy)
{
    g.setColour(getHealthColor(isHealthy));
    g.fillEllipse(area.toFloat());
    
    // Add subtle glow for healthy indicators
    if (isHealthy)
    {
        g.setColour(getHealthColor(isHealthy).withAlpha(0.3f));
        g.fillEllipse(area.expanded(2).toFloat());
    }
}