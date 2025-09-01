#include "SpectralPaintNode.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace nodes {

void SpectralPaintNode::prepare (const juce::dsp::ProcessSpec& spec) {
    sampleRate_ = spec.sampleRate;
    fftOrder_   = 8;
    fftSize_    = size_t(1) << fftOrder_; // 256
    numBins_    = int(fftSize_ / 2);      // 128
    hop_        = int(fftSize_ / 2);      // 50% overlap for easy PR

    fft_    = std::make_unique<juce::dsp::FFT>(fftOrder_);
    window_ = std::make_unique<juce::dsp::WindowingFunction<float>>(fftSize_, juce::dsp::WindowingFunction<float>::hann);

    timeBuffer_.assign(fftSize_, 0.0f);
    freqBuffer_.assign(fftSize_, 0.0f);
    overlapBuffer_.assign(fftSize_, 0.0f);
    normBuffer_.assign(fftSize_, 1e-9f); // if you switch to sqrt-Hann PR
    sqrtWin_.assign(fftSize_, 0.0f);

    // basic Hann (not sqrt) since we use 50% overlap
    // window_->getWindowingTable() is private; call multiply on the fly

    writeIndex_ = 0;
    bed_.assign(fftSize_, 0.0f);
    bedReady_ = false;
}

inline int SpectralPaintNode::reIdx(int bin) const noexcept {
    return (bin == 0 ? 0 : (bin == numBins_ ? 1 : 2*bin));
}
inline int SpectralPaintNode::imIdx(int bin) const noexcept {
    return (bin == 0 || bin == numBins_ ? -1 : 2*bin + 1);
}

int SpectralPaintNode::mapYToBin(float y) const noexcept {
    constexpr float fMin=80.f, fMax=8000.f;
    const float nyq = float(sampleRate_) * 0.5f;
    y = juce::jlimit(0.0f, 1.0f, y);
    // invert if you want lows at top: y = 1.0f - y;
    const float f   = fMin * std::pow(fMax/fMin, y);
    int bin = int(std::lround((f / nyq) * float(numBins_)));
    return juce::jlimit(0, numBins_, bin);
}

void SpectralPaintNode::applyRadialBoost(float* F, int center, float amt, int radius) noexcept {
    const int lo = std::max(0, center - radius), hi = std::min(numBins_, center + radius);
    for (int b = lo; b <= hi; ++b) {
        const float t = 1.f - float(std::abs(b - center)) / float(std::max(1, radius));
        const float g = 1.f + amt * t * t;
        const int r = reIdx(b), i = imIdx(b);
        F[r] *= g; if (i >= 0) F[i] *= g;
    }
}

void SpectralPaintNode::buildPinkBed(float dBFS) {
    const float lin = std::pow(10.0f, dBFS / 20.0f);
    std::fill(bed_.begin(), bed_.end(), 0.0f);
    for (int b = 1; b < numBins_; ++b) {
        const float shade = 1.0f / std::sqrt(float(b));
        const float mag   = lin * shade;
        const float phi   = bedRng_.nextFloat() * juce::MathConstants<float>::twoPi;
        const float re    = mag * std::cos(phi);
        const float im    = mag * std::sin(phi);
        const int r = reIdx(b), i = imIdx(b);
        bed_[r] = re; if (i >= 0) bed_[i] = im;
    }
    bedReady_ = true;
}

void SpectralPaintNode::addPinkBed(float* F) noexcept {
    if (!bedReady_) buildPinkBed(-60.0f); // quieter
    for (size_t i = 0; i < fftSize_; ++i) F[i] += bed_[i];
}

void SpectralPaintNode::process (const juce::dsp::ProcessContextReplacing<float>& ctx) {
    auto block = ctx.getOutputBlock();
    auto* L = block.getChannelPointer(0);
    const int n = int(block.getNumSamples());

    for (int i = 0; i < n; ++i) {
        timeBuffer_[writeIndex_] = L[i];
        if (++writeIndex_ == int(fftSize_)) {
            // window + FFT
            window_->multiplyWithWindowingTable(timeBuffer_.data(), fftSize_);
            std::copy(timeBuffer_.begin(), timeBuffer_.end(), freqBuffer_.begin());
            fft_->performRealOnlyForwardTransform(freqBuffer_.data());

            // apply paint
            rt::MaskColumnLite ev;
            while (maskQueue_.try_pop(ev)) {
                const int bin = mapYToBin(ev.y);
                const float amt = juce::jlimit(0.0f, 1.0f, ev.intensity);
                applyRadialBoost(freqBuffer_.data(), bin, amt, /*radius*/ 10);
            }

            // optional bed
            if (enablePinkBed_) addPinkBed(freqBuffer_.data());

            // iFFT
            fft_->performRealOnlyInverseTransform(freqBuffer_.data());

            // 50% overlap-add + simple scale (Hann@50% is stable)
            const float norm = 1.0f / float(fftSize_);
            for (size_t s = 0; s < fftSize_; ++s) {
                const float samp = freqBuffer_[s] * norm + overlapBuffer_[s];
                if (int(s) < n) L[s] = samp;
            }
            // shift OLA by hop
            for (size_t s = 0; s < fftSize_ - size_t(hop_); ++s)
                overlapBuffer_[s] = freqBuffer_[s + hop_] * norm;
            for (size_t s = fftSize_ - hop_; s < fftSize_; ++s)
                overlapBuffer_[s] = 0.0f;

            // slide time window by hop
            std::move(timeBuffer_.begin() + hop_, timeBuffer_.end(), timeBuffer_.begin());
            std::fill(timeBuffer_.end() - hop_, timeBuffer_.end(), 0.0f);
            writeIndex_ = int(fftSize_) - hop_;
        }
    }

    // copy L to other channels
    for (size_t ch = 1; ch < block.getNumChannels(); ++ch)
        std::memcpy(block.getChannelPointer(ch), L, sizeof(float) * size_t(n));
}

} // namespace nodes