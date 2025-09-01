#include "SpectralModel.h"

void SpectralModel::build (const juce::AudioBuffer<float>& mono, int fftOrder, int hop)
{
    fftOrder_ = fftOrder;
    hop_      = hop;
    const int N = 1 << fftOrder_;
    if (mono.getNumSamples() < N) { clear(); return; }
    const int F = 1 + (mono.getNumSamples() - N) / hop_;
    if (F <= 0) { clear(); return; }

    window.resize (N);
    for (int n = 0; n < N; ++n)
        window[n] = 0.5f * (1.0f - std::cos (2.0f * juce::MathConstants<float>::pi * (float) n / (float) (N - 1)));

    frames.assign ((size_t) F, std::vector<juce::dsp::Complex<float>> ((size_t) (N/2 + 1)));
    mags.assign   ((size_t) F, std::vector<float> ((size_t) (N/2 + 1)));

    juce::dsp::FFT fft (fftOrder_);
    std::vector<juce::dsp::Complex<float>> in (N), out (N);
    auto* x = mono.getReadPointer (0);

    for (int f = 0; f < F; ++f)
    {
        const int start = f * hop_;
        for (int n = 0; n < N; ++n) in[(size_t) n] = { x[start + n] * window[(size_t) n], 0.0f };
        fft.perform (in.data(), out.data(), false);
        for (int k = 0; k <= N/2; ++k)
        {
            frames[(size_t) f][(size_t) k] = out[(size_t) k];
            mags[(size_t) f][(size_t) k]   = std::abs (out[(size_t) k]);
        }
    }
}