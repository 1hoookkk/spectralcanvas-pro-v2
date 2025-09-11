#include "SpectralCanvasLiteEditor.h"
#include "../Core/Params.h"
#include "../Core/MaskColumn.h"

using namespace juce;

SpectralCanvasLiteEditor::SpectralCanvasLiteEditor(SpectralCanvasLiteProcessor& p)
  : AudioProcessorEditor(&p), proc_(p)
{
    setLookAndFeel(&modernLookAndFeel_);
    setSize(500, 380);

    addAndMakeVisible(titleLabel_);
    titleLabel_.setText("Spectral Canvas Lite", dontSendNotification);
    titleLabel_.setFont(Font(24.0f, Font::bold));
    titleLabel_.setJustificationType(Justification::centredTop);

    addAndMakeVisible(canvas_);
    
    auto& apvts = proc_.getAPVTS();

    auto setupLabelAndControl = [&](Component& comp, Label& label, const String& labelText) {
        addAndMakeVisible(comp);
        addAndMakeVisible(label);
        label.setText(labelText, dontSendNotification);
        label.attachToComponent(&comp, false);
        label.setJustificationType(Justification::centred);
    };

    modeAtt_.reset(new ComboBoxAttachment(apvts, Params::kProcessingMode, modeBox_));
    setupLabelAndControl(modeBox_, modeLabel_, "Mode");

    auto setupSlider = [&](Slider& s, std::unique_ptr<SliderAttachment>& att, const String& paramID, const String& labelText) {
        s.setSliderStyle(Slider::RotaryVerticalDrag);
        s.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        att.reset(new SliderAttachment(apvts, paramID, s));
        setupLabelAndControl(s, labelText == "Brush Width" ? brushWidthLabel_ :
                                labelText == "Brush Strength" ? brushStrengthLabel_ :
                                labelText == "Mask Floor" ? maskFloorLabel_ :
                                outputTrimLabel_, labelText);
    };

    setupSlider(brushWidthSlider_,     brushWidthAtt_,     Params::kBrushWidth,     "Brush Width");
    setupSlider(brushStrengthSlider_,  brushStrengthAtt_,  Params::kBrushStrength,  "Brush Strength");
    setupSlider(maskFloorDbSlider_,    maskFloorDbAtt_,    Params::kMaskFloorDb,    "Mask Floor");
    setupSlider(outputTrimDbSlider_,   outputTrimDbAtt_,   Params::kOutputTrimDb,   "Output Trim");

    // Performance HUD setup
    addAndMakeVisible(performanceHUD_);
    performanceHUD_.setAlpha(0.8f);
    
    // Update HUD every 100ms - DISABLED FOR DEBUG
    // startTimer(100);
}

SpectralCanvasLiteEditor::~SpectralCanvasLiteEditor()
{
    setLookAndFeel(nullptr);
}

void SpectralCanvasLiteEditor::paint(Graphics& g)
{
    g.fillAll(Colour(0xff1c1f23)); // Dark background
}

void SpectralCanvasLiteEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);

    titleLabel_.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);

    // Performance HUD in top-right corner
    performanceHUD_.setBounds(bounds.removeFromTop(60).removeFromRight(200));
    bounds.removeFromTop(5);

    auto controlsBounds = bounds.removeFromBottom(100);
    canvas_.setBounds(bounds);

    auto knobWidth = controlsBounds.getWidth() / 4;
    modeBox_.setBounds(controlsBounds.removeFromLeft(knobWidth).reduced(5));
    brushWidthSlider_.setBounds(controlsBounds.removeFromLeft(knobWidth).reduced(5));
    brushStrengthSlider_.setBounds(controlsBounds.removeFromLeft(knobWidth).reduced(5));
    maskFloorDbSlider_.setBounds(controlsBounds.removeFromLeft(knobWidth).reduced(5));
    outputTrimDbSlider_.setBounds(controlsBounds.removeFromLeft(knobWidth).reduced(5));
}

void SpectralCanvasLiteEditor::mouseDown(const juce::MouseEvent& e)
{
    if (canvas_.getBounds().contains(e.getPosition()))
    {
        auto relativeEvent = e.getEventRelativeTo(&canvas_);
        pushMaskEventAt(relativeEvent.getPosition());
    }
}

void SpectralCanvasLiteEditor::mouseDrag(const juce::MouseEvent& e)
{
    if (canvas_.getBounds().contains(e.getPosition()))
    {
        auto relativeEvent = e.getEventRelativeTo(&canvas_);
        pushMaskEventAt(relativeEvent.getPosition());
    }
}

void SpectralCanvasLiteEditor::pushMaskEventAt(const Point<int>& pos)
{
    const float y01 = 1.0f - (float)pos.y / (float)canvas_.getHeight();
    const float widthNorm = (float)brushWidthSlider_.getValue();
    const float strength  = (float)brushStrengthSlider_.getValue();

    const int K = SpectralProcessor::kFftSize / 2;
    const int centerBin = std::clamp((int)std::round(y01 * K), 0, K);
    const int halfSpan  = std::max(1, (int)std::round(widthNorm * 0.5f * (K / 8)));
    const int startBin  = std::max(0, centerBin - halfSpan);
    const int endBin    = std::min(K, centerBin + halfSpan);

    MaskColumn ev;
    ev.y_position = y01;
    ev.intensity  = strength;
    ev.startBin   = startBin;
    ev.endBin     = endBin;

    proc_.pushPaintEvent(ev);
}

void SpectralCanvasLiteEditor::timerCallback()
{
    updatePerformanceStats();
}

void SpectralCanvasLiteEditor::updatePerformanceStats()
{
    auto stats = proc_.getPerfStats();
    float peakGrDb = proc_.getPeakGrDb();
    performanceHUD_.updateStats(stats.eventsEnqueued, stats.eventsDrained, stats.eventsDropped, peakGrDb);
}

// PerformanceHUD Implementation
void SpectralCanvasLiteEditor::PerformanceHUD::paint(Graphics& g)
{
    g.setColour(Colours::black.withAlpha(0.7f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0f);
    
    g.setColour(Colours::white.withAlpha(0.8f));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(1), 4.0f, 1.0f);
    
    g.setColour(Colours::lightgreen);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 10.0f, Font::plain));
    g.drawText(statsText_, 8, 8, getWidth() - 16, getHeight() - 16, Justification::topLeft);
}

void SpectralCanvasLiteEditor::PerformanceHUD::updateStats(uint64_t enqueued, uint64_t drained, uint64_t dropped, float peakGrDb)
{
    statsText_ = String::formatted("Queue: %llu/%llu/%llu\nPeak GR: %.1f dB", 
                                   enqueued, drained, dropped, peakGrDb);
    repaint();
}
