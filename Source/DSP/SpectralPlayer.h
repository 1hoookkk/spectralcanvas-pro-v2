#pragma once
#include <juce_dsp/juce_dsp.h>
#include "SpectralModel.h"
#include "SpectralMask.h"

class SpectralPlayer {
public:
    void prepare (double sr, int blockSize, const SpectralModel* m, const SpectralMask* mk);
    void reset();
    void process (juce::AudioBuffer<float>& out);
    void setRespeed(float newRespeed) noexcept { respeed = juce::jlimit(0.25f, 4.0f, newRespeed); }
private:
    const SpectralModel* model = nullptr;
    const SpectralMask*  mask  = nullptr;
    double sampleRate = 48000.0;
    int fftOrder = 10, fftSize = 1024, hop = 256;
    juce::dsp::FFT fft { 10 };
    std::vector<float> win;
    std::vector<juce::dsp::Complex<float>> ifftIn, ifftOut;
    std::vector<float> ola, norm;
    int writePos = 0, frameCursor = 0, inHopCursor = 0;
    float respeed = 1.0f;
    float framePosition = 0.0f; // Fractional frame position for respeed
};