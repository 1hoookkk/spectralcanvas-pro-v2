#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../DSP/SpectralModel.h"
#include "../DSP/SpectralMask.h"

class SpectrogramComponent : public juce::Component
{
public:
    void setModel (const SpectralModel* m) { model = m; rebuildImage(); }
    void setEditableMask (SpectralMask* mk) { mask = mk; repaint(); }
    void setBrushRadius (int r) { brushRadius = juce::jlimit (2, 64, r); }

    void paint (juce::Graphics& g) override;
    void resized() override { rebuildImage(); }

    void mouseDown (const juce::MouseEvent& e) override { applyBrush (e, ! e.mods.isAltDown()); }
    void mouseDrag (const juce::MouseEvent& e) override { applyBrush (e, ! e.mods.isAltDown()); }

private:
    const SpectralModel* model = nullptr;
    SpectralMask* mask = nullptr;
    juce::Image img;
    int brushRadius = 8;

    void rebuildImage();
    void applyBrush (const juce::MouseEvent& e, bool additive);
};