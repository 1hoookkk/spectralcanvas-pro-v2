#include "SpectralPlayer.h"

void SpectralPlayer::prepare (double sr, int /*blockSize*/, const SpectralModel* m, const SpectralMask* mk)
{
    model = m; mask = mk; sampleRate = sr;
    jassert (model && model->isReady());
    fftOrder = model->fftOrder();
    fftSize  = model->fftSize();
    hop      = model->hop();
    fft      = juce::dsp::FFT (fftOrder);

    win.resize (fftSize);
    for (int n = 0; n < fftSize; ++n)
        win[(size_t) n] = 0.5f * (1.0f - std::cos (2.0f * juce::MathConstants<float>::pi * (float) n / (float) (fftSize - 1)));

    ifftIn .assign ((size_t) fftSize, {});
    ifftOut.assign ((size_t) fftSize, {});

    const int ring = juce::nextPowerOfTwo (fftSize * 8);
    ola .assign ((size_t) ring, 0.0f);
    norm.assign ((size_t) ring, 1e-6f);
    writePos = 0; frameCursor = 0; inHopCursor = 0;
}

void SpectralPlayer::reset()
{
    std::fill (ola.begin(),  ola.end(),  0.0f);
    std::fill (norm.begin(), norm.end(), 1e-6f);
    frameCursor = 0; inHopCursor = 0; writePos = 0;
}

void SpectralPlayer::process (juce::AudioBuffer<float>& out)
{
    out.clear();
    if (! model || ! model->isReady()) return;

    const int num = out.getNumSamples();
    int done = 0;

    while (done < num)
    {
        if (inHopCursor == 0)
        {
            // Build masked spectrum for current frame
            const auto* src = model->complexFrame (frameCursor);
            const float* mk = (mask && mask->isReady()) ? mask->raw() : nullptr;
            const int H = model->numBins();

            for (int k = 0; k <= fftSize/2; ++k)
            {
                float m = mk ? mk[(size_t) frameCursor * (size_t) H + (size_t) k] : 1.0f;
                auto c  = src[(size_t) k];
                ifftIn[(size_t) k] = { c.real() * m, c.imag() * m };
            }
            // Hermitian symmetry
            ifftIn[0].imag (0.0f);
            ifftIn[(size_t) fftSize/2].imag (0.0f);
            for (int k = 1; k < fftSize/2; ++k)
                ifftIn[(size_t) (fftSize - k)] = std::conj (ifftIn[(size_t) k]);

            // iFFT
            fft.perform (ifftIn.data(), ifftOut.data(), true);
            const float scale = 1.0f / (float) fftSize;

            // OLA into ring
            const int R = (int) ola.size();
            for (int n = 0; n < fftSize; ++n)
            {
                const int idx = (writePos + n) & (R - 1);
                const float w = win[(size_t) n];
                const float s = ifftOut[(size_t) n].real() * w * scale;
                ola [(size_t) idx] += s;
                norm[(size_t) idx] += w * w;
            }

            frameCursor = (frameCursor + 1) % model->numFrames();
        }

        const int toCopy = juce::jmin (num - done, hop - inHopCursor);
        const int R = (int) ola.size();

        for (int n = 0; n < toCopy; ++n)
        {
            const int idx = (writePos + n) & (R - 1);
            const float s = ola [(size_t) idx] / norm[(size_t) idx];
            for (int ch = 0; ch < out.getNumChannels(); ++ch)
                out.addSample (ch, done + n, s);
            ola [(size_t) idx] = 0.0f;
            norm[(size_t) idx] = 1e-6f;
        }

        writePos      = (writePos + toCopy) & (R - 1);
        inHopCursor   = (inHopCursor + toCopy);
        if (inHopCursor >= hop) inHopCursor = 0;
        done += toCopy;
    }
}