#pragma once
#include <vector>
#include <cmath>
#include <juce_core/juce_core.h>

namespace tools
{
    struct BrushParams
    {
        float radiusBins = 8.0f;    // vertical radius in bins
        float strength   = 0.8f;    // 0..1 attenuation amount
    };
    
    // Simple attenuation brush producing a per-bin gain array (1.0 = no change)
    struct GainBrush
    {
        // centerBin in [0, numBins-1]
        static void stroke(std::vector<float>& out, int centerBin, int numBins, const BrushParams& p)
        {
            if ((int)out.size() != numBins) out.assign((size_t)numBins, 1.0f);
            const float r = juce::jmax(1.0f, p.radiusBins);
            for (int b = 0; b < numBins; ++b)
            {
                const float d = std::abs((float)b - (float)centerBin);
                const float t = juce::jlimit(0.0f, 1.0f, 1.0f - d / r);
                const float atten = 1.0f - (p.strength * t); // linear falloff
                out[(size_t)b] = juce::jlimit(0.0f, 1.0f, atten);
            }
        }
    };
}