#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace Params
{
    static constexpr const char* kProcessingMode   = "processingMode";
    static constexpr const char* kBrushWidth       = "brushWidth";
    static constexpr const char* kBrushStrength    = "brushStrength";
    static constexpr const char* kMaskFloorDb      = "maskFloorDb";
    static constexpr const char* kOutputTrimDb     = "outputTrimDb";

    inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        using namespace juce;

        std::vector<std::unique_ptr<RangedAudioParameter>> params;

        // Mode: Effect, Synth, Hybrid (default Effect)
        params.push_back(std::make_unique<AudioParameterChoice>(
            kProcessingMode, "Mode",
            StringArray{ "Effect", "Synth", "Hybrid" }, 0));

        params.push_back(std::make_unique<AudioParameterFloat>(
            kBrushWidth, "Brush Width",
            NormalisableRange<float>(0.1f, 1.0f, 0.0f, 1.0f),
            0.5f));

        params.push_back(std::make_unique<AudioParameterFloat>(
            kBrushStrength, "Brush Strength",
            NormalisableRange<float>(0.0f, 1.0f, 0.0f, 1.0f),
            0.8f));

        params.push_back(std::make_unique<AudioParameterFloat>(
            kMaskFloorDb, "Mask Floor (dB)",
            NormalisableRange<float>(-60.0f, -12.0f, 0.0f, 1.0f),
            -24.0f));

        params.push_back(std::make_unique<AudioParameterFloat>(
            kOutputTrimDb, "Output Trim (dB)",
            NormalisableRange<float>(-12.0f, 0.0f, 0.0f, 1.0f),
            -6.0f));

        return { params.begin(), params.end() };
    }
}
