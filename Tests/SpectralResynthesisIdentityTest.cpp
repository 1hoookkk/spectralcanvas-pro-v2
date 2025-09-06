#include <gtest/gtest.h>
#include "../Source/DSP/SpectralModel.h"
#include "../Source/DSP/SpectralPlayer.h"
#include <juce_audio_basics/juce_audio_basics.h>

namespace
{
    // Helper function to calculate Root Mean Square error
    float calculateRmsError(const float* original, const float* processed, int numSamples)
    {
        float sumSquaredError = 0.0f;
        for (int i = 0; i < numSamples; ++i)
        {
            float error = original[i] - processed[i];
            sumSquaredError += error * error;
        }
        return std::sqrt(sumSquaredError / numSamples);
    }
}

TEST(SpectralResynthesisIdentityTest, SineWaveIdentity)
{
    const double sampleRate = 48000.0;
    const int numSamples = (int)sampleRate * 2; // 2 seconds of audio
    const float frequency = 440.0f;
    const int fftOrder = 10; // 1024
    const int hopSize = 256;
    const int blockSize = 512;

    // 1. Create input signal (sine wave)
    juce::AudioBuffer<float> inputBuffer(1, numSamples);
    auto* inputData = inputBuffer.getWritePointer(0);
    for (int i = 0; i < numSamples; ++i)
    {
        inputData[i] = std::sin(2.0f * juce::MathConstants<float>::pi * frequency * i / sampleRate);
    }

    // 2. Build Spectral Model
    SpectralModel model;
    model.build(inputBuffer, fftOrder, hopSize);
    ASSERT_TRUE(model.isReady());

    // 3. Prepare Spectral Player
    SpectralPlayer player;
    SpectralMask emptyMask; // No mask applied for identity test
    player.prepare(sampleRate, blockSize, &model, &emptyMask);

    // 4. Process the audio through the player
    juce::AudioBuffer<float> outputBuffer(1, numSamples);
    outputBuffer.clear();
    int samplesProcessed = 0;
    while (samplesProcessed < numSamples)
    {
        juce::AudioBuffer<float> tempBlock(1, blockSize);
        player.process(tempBlock);

        int samplesToCopy = std::min(blockSize, numSamples - samplesProcessed);
        outputBuffer.copyFrom(0, samplesProcessed, tempBlock, 0, 0, samplesToCopy);
        samplesProcessed += samplesToCopy;
    }

    // 5. Compare input and output
    // We need to account for the STFT latency. The output will be delayed.
    const int latency = (1 << fftOrder) - hopSize;
    auto* outputData = outputBuffer.getReadPointer(0);

    // To simplify, we compare the middle sections of the buffers to avoid edge artifacts.
    const int startSample = latency * 2;
    const int samplesToCompare = numSamples / 2;
    
    ASSERT_LT(startSample + samplesToCompare, numSamples);

    float rmsError = calculateRmsError(inputData + startSample, outputData + startSample + latency, samplesToCompare);
    
    // A small error is expected due to windowing. A perfect reconstruction would have a very low error.
    // Let's set a threshold. 1e-4 should be a reasonable tolerance for a good STFT implementation.
    EXPECT_LT(rmsError, 1e-4);
}

