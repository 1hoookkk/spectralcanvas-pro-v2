#include "SpectralProcessor.h"
#include "../Utils/TracyProfiler.h"
#include <algorithm>

using namespace juce;

void SpectralProcessor::prepare(double sampleRate, int maxBlockSize)
{
    sampleRate_ = sampleRate;

    fft_.reset(new dsp::FFT(kFftOrder));

    analysisWin_.resize(kFftSize);
    synthWin_.resize(kFftSize);
    
    // Generate sqrt-Hann window manually for COLA at 75% overlap
    for (int n = 0; n < kFftSize; ++n)
    {
        float hannValue = 0.5f * (1.0f - std::cos(2.0f * MathConstants<float>::pi * n / (kFftSize - 1)));
        float w = std::sqrt(hannValue);
        analysisWin_[n] = w;
        synthWin_[n] = w;
    }

    inputFifo_.assign(kFftSize, 0.0f);
    fftBuffer_.assign(kFftSize, 0.0f);
    olaBuffer_.assign(kFftSize + (kOverlapFactor - 1) * kHopSize, 0.0f);
    
    const int numBins = kFftSize / 2 + 1;
    maskTarget_.assign(numBins, 1.0f);
    maskSmoothed_.assign(numBins, 1.0f);
    
    reset();
}

void SpectralProcessor::reset()
{
    std::fill(inputFifo_.begin(), inputFifo_.end(), 0.0f);
    fifoWritePos_ = 0;
    std::fill(olaBuffer_.begin(), olaBuffer_.end(), 0.0f);
    olaWritePos_ = 0;
    olaReadPos_ = 0;
    std::fill(maskTarget_.begin(), maskTarget_.end(), 1.0f);
    std::fill(maskSmoothed_.begin(), maskSmoothed_.end(), 1.0f);
    hopCounter_ = 0;
    peakGrDb_ = 0.0f;
}

void SpectralProcessor::process(AudioBuffer<float>& buffer,
                                float maskFloorDb,
                                float outputTrimDb)
{
    const int numSamples = buffer.getNumSamples();
    const float trim = Decibels::decibelsToGain(outputTrimDb);
    const float maskFloorLin = Decibels::decibelsToGain(maskFloorDb);

    for (int n = 0; n < numSamples; ++n)
    {
        // 1. Write input sample to our circular buffer (FIFO)
        const float in = buffer.getNumChannels() > 0 ? buffer.getSample(0, n) : 0.0f;
        inputFifo_[fifoWritePos_] = in;
        fifoWritePos_ = (fifoWritePos_ + 1) % kFftSize;

        // 2. Read the processed output from the OLA buffer
        const float out = olaBuffer_[olaReadPos_];
        olaBuffer_[olaReadPos_] = 0.0f; // Clear after reading
        olaReadPos_ = (olaReadPos_ + 1) % kFftSize;

        // 3. Write output to all channels
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.setSample(ch, n, out * trim);

        // 4. If we've collected enough samples for a hop, process a frame
        if (++hopCounter_ >= kHopSize)
        {
            hopCounter_ = 0;
            processFrame(maskFloorLin);
        }
    }
}

void SpectralProcessor::processFrame(float maskFloorLin)
{
    SPECTRAL_PROFILE_ZONE("frame_dsp");

    // 1. Copy the last kFftSize samples from the circular input buffer into a linear buffer for the FFT
    const int startPos = (fifoWritePos_ - kFftSize + kFftSize) % kFftSize;
    const int firstChunkSize = kFftSize - startPos;

    FloatVectorOperations::copy(fftBuffer_.data(), inputFifo_.data() + startPos, firstChunkSize);
    if (firstChunkSize < kFftSize)
    {
        FloatVectorOperations::copy(fftBuffer_.data() + firstChunkSize, inputFifo_.data(), kFftSize - firstChunkSize);
    }
    
    // 2. Apply analysis window and perform FFT
    FloatVectorOperations::multiply(fftBuffer_.data(), analysisWin_.data(), kFftSize);
    fft_->performRealOnlyForwardTransform(fftBuffer_.data());

    // 3. Apply the spectral mask
    decayMaskTowardUnity();
    smoothMaskAttackRelease();
    {
        SPECTRAL_PROFILE_ZONE("applyMask");
        applyMaskToSpectrum(maskFloorLin);
    }
    
    // 4. Perform iFFT and apply synthesis window
    fft_->performRealOnlyInverseTransform(fftBuffer_.data());
    FloatVectorOperations::multiply(fftBuffer_.data(), synthWin_.data(), kFftSize);
    
    // 5. Add the result into the circular Overlap-Add buffer
    // OLA buffer length is kFftSize + (kOverlapFactor - 1) * kHopSize
    const int olaLen = static_cast<int>(olaBuffer_.size());
    const float invN = 1.0f / (float)kFftSize;
    for (int i = 0; i < kFftSize; ++i)
    {
        const int idx = (olaWritePos_ + i) % olaLen;
        olaBuffer_[idx] += fftBuffer_[i] * invN;
    }
    // Advance write pointer by hop for next frame
    olaWritePos_ = (olaWritePos_ + kHopSize) % olaLen;
}

void SpectralProcessor::decayMaskTowardUnity() noexcept
{
    const float hopSec = (float)kHopSize / (float)sampleRate_;
    const float T60s   = 0.6f;
    decayCoeff_ = std::exp(-hopSec * std::log(1000.0f) / T60s);
    for (auto& v : maskTarget_)
        v = 1.0f + (v - 1.0f) * decayCoeff_;
}

void SpectralProcessor::smoothMaskAttackRelease() noexcept
{
    const float hopSec = (float)kHopSize / (float)sampleRate_;
    atkCoeff_ = std::exp(-hopSec / (3.0f * 0.001f));
    relCoeff_ = std::exp(-hopSec / (80.0f * 0.001f));

    for (size_t k = 0; k < maskTarget_.size(); ++k)
    {
        float t = maskTarget_[k];
        float y = maskSmoothed_[k];
        const float a = (t < y) ? atkCoeff_ : relCoeff_;
        maskSmoothed_[k] = a * y + (1.0f - a) * t;
    }
}

void SpectralProcessor::applyMaskToSpectrum(float maskFloorLin) noexcept
{
    const int numBins = kFftSize / 2 + 1;
    float minLin = 1.0f;

    for (int k = 0; k < numBins; ++k)
    {
        float re, im;
        getPackedBin(fftBuffer_.data(), k, kFftSize, re, im);
        float g = jlimit(maskFloorLin, 1.0f, maskSmoothed_[k]);
        minLin = std::min(minLin, g);
        setPackedBin(fftBuffer_.data(), k, kFftSize, re * g, im * g);
    }
    peakGrDb_ = Decibels::gainToDecibels(minLin);
}

void SpectralProcessor::applyPaintEvent(const MaskColumn& col) noexcept
{
    const int numBins = kFftSize / 2 + 1;
    const int start = std::clamp(std::min(col.startBin, col.endBin), 0, numBins - 1);
    const int end   = std::clamp(std::max(col.startBin, col.endBin), 0, numBins - 1);
    
    const float center = 0.5f * (start + end);
    const float width  = std::max(1.0f, 0.5f * (end - start + 1));
    const float sigma2 = width * width;

    const float brushCutLin = Decibels::decibelsToGain(-12.0f * col.intensity);

    for (int k = start; k <= end; ++k)
    {
        const float d  = (k - center);
        const float gw = std::exp(-(d * d) / (2.0f * sigma2));
        const float g  = 1.0f - (1.0f - brushCutLin) * gw;
        maskTarget_[k] = std::min(maskTarget_[k], g);
    }
}
