#pragma once
#include <juce_core/juce_core.h>
#include <atomic>
#include <vector>

class SpectralMask {
public:
    void init (int frames, int bins)
    {
        W = frames; H = bins;
        data.assign ((size_t) W * (size_t) H, 1.0f); // unity = pass-through
    }
    bool isReady() const noexcept { return W > 0 && H > 0; }
    int  width()  const noexcept { return W; }
    int  height() const noexcept { return H; }
    const float* raw() const noexcept { return data.data(); }
    float  get (int f, int k) const noexcept { return data[(size_t) f * (size_t) H + (size_t) k]; }
    void   set (int f, int k, float v)       { data[(size_t) f * (size_t) H + (size_t) k] = juce::jlimit (0.0f, 1.0f, v); }
private:
    int W = 0, H = 0;
    std::vector<float> data;
};