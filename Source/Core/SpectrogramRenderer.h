#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_dsp/juce_dsp.h>

// Lightweight offline STFT → Image renderer for UI background
class SpectrogramRenderer
{
public:
    struct Palette {
        // Map normalized frequency [0,1] and magnitude [0,1] to colour
        static juce::Colour map(float normFreq, float normMag)
        {
            // Emotional palette: lows=magenta/purple, mids=orange/green, highs=cyan/yellow/white
            const float h = juce::jlimit(0.0f, 1.0f, normFreq);
            const float v = juce::jlimit(0.0f, 1.0f, std::pow(normMag, 0.6f)); // perceptual
            // Piecewise hue mapping
            float hue;
            if (h < 0.33f)      hue = juce::jmap(h, 0.0f, 0.33f, 0.85f, 0.95f); // purple→magenta
            else if (h < 0.66f) hue = juce::jmap(h, 0.33f, 0.66f, 0.08f, 0.33f); // red/orange→green
            else                hue = juce::jmap(h, 0.66f, 1.0f, 0.50f, 0.16f);  // cyan→yellow
            const float sat = juce::jlimit(0.2f, 0.95f, 0.65f + 0.25f * (1.0f - v));
            return juce::Colour::fromHSV(hue, sat, v, 1.0f);
        }
    };

    // Renders a spectrogram image (width x height) for a mono or stereo buffer (first channel used)
    static juce::Image render(const juce::AudioBuffer<float>& buffer,
                              double sampleRate,
                              int width,
                              int height)
    {
        const int fftOrder = 11; // 2048
        const int fftSize  = 1 << fftOrder;
        juce::dsp::FFT fft(fftOrder);
        juce::AudioBuffer<float> mono(1, buffer.getNumSamples());
        mono.clear();
        const int chs = buffer.getNumChannels();
        for (int i = 0; i < mono.getNumSamples(); ++i)
        {
            float sum = 0.0f;
            for (int ch = 0; ch < chs; ++ch) sum += buffer.getSample(ch, i);
            mono.setSample(0, i, sum / juce::jmax(1, chs));
        }

        juce::Image img(juce::Image::RGB, width, height, false);
        juce::Graphics g(img);
        g.fillAll(juce::Colours::black);

        std::vector<float> window(fftSize);
        for (int i = 0; i < fftSize; ++i)
            window[i] = 0.5f - 0.5f * std::cos(2.0f * juce::MathConstants<float>::pi * i / (fftSize - 1));

        std::vector<float> fftData(2 * fftSize);
        const int hop = fftSize / 4; // 75% overlap
        const int numFrames = juce::jmax(1, (mono.getNumSamples() - fftSize) / hop);

        for (int x = 0; x < width; ++x)
        {
            // Map column to frame index
            const float t = (float)x / (float)(width - 1);
            const int frame = juce::jlimit(0, numFrames - 1, (int)std::round(t * (numFrames - 1)));
            const int start = frame * hop;
            // Copy windowed frame
            std::fill(fftData.begin(), fftData.end(), 0.0f);
            for (int i = 0; i < fftSize; ++i)
                fftData[i] = mono.getSample(0, start + i) * window[i];
            // Perform FFT
            fft.performRealOnlyForwardTransform(fftData.data());
            // Magnitude and log scaling
            for (int y = 0; y < height; ++y)
            {
                const float nf = (float)y / (float)(height - 1);
                const int bin = juce::jlimit(0, fftSize / 2 - 1, (int)std::round(nf * (fftSize / 2 - 1)));
                const float re = fftData[bin];
                const float im = fftData[bin + fftSize / 2];
                const float mag = std::sqrt(re * re + im * im) + 1e-9f;
                const float db = juce::Decibels::gainToDecibels(mag);
                const float normMag = juce::jlimit(0.0f, 1.0f, (db + 80.0f) / 80.0f);
                img.setPixelAt(x, height - 1 - y, Palette::map(nf, normMag));
            }
        }
        return img;
    }
};