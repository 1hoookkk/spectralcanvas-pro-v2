#include "SpectrogramComponent.h"

void SpectrogramComponent::rebuildImage()
{
    if (! model || ! model->isReady()) { img = {}; repaint(); return; }
    const int W = model->numFrames();
    const int H = model->numBins();
    img = juce::Image (juce::Image::RGB, juce::jmax (1, W), juce::jmax (1, H), false);

    const float floorDb = -80.0f;
    for (int x = 0; x < W; ++x)
        for (int y = 0; y < H; ++y)
        {
            float mag = model->magnitude (x, y);
            float db  = 20.0f * std::log10 (mag + 1e-9f);
            float t   = juce::jlimit (0.0f, 1.0f, (db - floorDb) / -floorDb);
            juce::uint8 v = (juce::uint8) juce::jlimit (0, 255, (int) std::round (t * 255.0f));
            img.setPixelAt (x, H - 1 - y, juce::Colour (v, v, v));
        }
    repaint();
}

void SpectrogramComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    if (img.isValid())
        g.drawImageTransformed (img, juce::AffineTransform::scale ((float) getWidth() / img.getWidth(),
                                                                   (float) getHeight() / img.getHeight()));
    // Mask overlay (red where masked out)
    if (mask && model && model->isReady())
    {
        const int W = getWidth(), H = getHeight();
        const int F = model->numFrames();
        const int B = model->numBins();
        juce::Image overlay (juce::Image::ARGB, W, H, true);
        for (int x = 0; x < W; ++x)
        {
            int f = juce::jlimit (0, F-1, (int) std::floor ((x / (float) W) * F));
            for (int y = 0; y < H; ++y)
            {
                int b = juce::jlimit (0, B-1, (int) std::floor ((1.0f - y / (float) H) * B));
                float m = mask->get (f, b);
                if (m < 0.999f)
                    overlay.setPixelAt (x, y, juce::Colour (255, 0, 0).withAlpha ((float) (1.0f - m) * 0.5f));
            }
        }
        g.drawImageAt (overlay, 0, 0);
    }
}

void SpectrogramComponent::applyBrush (const juce::MouseEvent& e, bool additive)
{
    if (! (mask && model && model->isReady())) return;
    const int W = getWidth(), H = getHeight();
    const int F = model->numFrames(), B = model->numBins();

    const int cx = juce::jlimit (0, W-1, (int) e.position.x);
    const int cy = juce::jlimit (0, H-1, (int) e.position.y);
    const int f0 = juce::jlimit (0, F-1, (int) std::floor ((cx / (float) W) * F));
    const int b0 = juce::jlimit (0, B-1, (int) std::floor ((1.0f - cy / (float) H) * B));

    const int r = brushRadius;
    for (int df = -r; df <= r; ++df)
        for (int db = -r; db <= r; ++db)
        {
            if (df*df + db*db > r*r) continue;
            int f = juce::jlimit (0, F-1, f0 + df);
            int b = juce::jlimit (0, B-1, b0 + db);
            float cur = mask->get (f, b);
            float v   = additive ? juce::jmin (1.0f, cur + 0.9f) : 0.0f;
            mask->set (f, b, v);
        }
    repaint();
}