#pragma once
#include <juce_dsp/juce_dsp.h>

class SpectralModel {
public:
    void build (const juce::AudioBuffer<float>& mono, int fftOrder, int hop);
    bool isReady() const noexcept { return ! frames.empty(); }
    void clear() { frames.clear(); mags.clear(); }

    int fftSize()   const noexcept { return 1 << fftOrder_; }
    int fftOrder()  const noexcept { return fftOrder_; }
    int hop()       const noexcept { return hop_; }
    int numFrames() const noexcept { return (int) frames.size(); }
    int numBins()   const noexcept { return fftSize() / 2 + 1; }

    const juce::dsp::Complex<float>* complexFrame (int f) const noexcept { return frames[(size_t) f].data(); }
    float magnitude (int f, int k) const noexcept { return mags[(size_t) f][(size_t) k]; }

private:
    int fftOrder_ = 10; // 1024
    int hop_      = 256;
    std::vector<std::vector<juce::dsp::Complex<float>>> frames; // [F][K]
    std::vector<std::vector<float>> mags;                        // [F][K]
    std::vector<float> window;
};