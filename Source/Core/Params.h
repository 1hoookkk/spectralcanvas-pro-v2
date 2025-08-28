#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <array>

namespace Params
{
// Parameter IDs for the plugin
namespace ParameterIDs
{
    // Dual-Mode Engine Parameters
    const juce::String mode = "mode";                     // 0=Synth, 1=Resynth, 2=Hybrid
    const juce::String blend = "blend";                   // 0..1 (0=synth only, 1=resynth only)
    const juce::String respeed = "respeed";               // 0.25..4.0 (resynth playback speed)
    
    // Brush System Parameters
    const juce::String brushType = "brush_type";          // 0=Paint, 1=Quantize, 2=Erase, 3=Comb
    const juce::String brushSize = "brush_size";          // 1..128 bins
    const juce::String brushStrength = "brush_strength";  // 0..1
    const juce::String quantizeStrength = "quantize_strength"; // 0..1
    
    // Grid & Tuning Parameters
    const juce::String gridOn = "grid_on";
    const juce::String tuningId = "tuning_id";            // 0=12TET, custom N-EDO...
    
    // Z-Plane Morph Parameters
    const juce::String zPlaneMorphX = "zplane_morph_x";   // 0..1
    const juce::String zPlaneMorphY = "zplane_morph_y";   // 0..1
    
    // Spectral Processing Parameters
    const juce::String spectralGain = "spectral_gain";
    const juce::String spectralMix = "spectral_mix";
    const juce::String fftSize = "fft_size";
    const juce::String hopSize = "hop_size";
    
    // Z-Plane Filter Parameters  
    const juce::String filterType = "filter_type";       // 0.0=LP, 0.5=BP, 1.0=HP
    const juce::String filterCutoff = "filter_cutoff";
    const juce::String filterResonance = "filter_resonance";
    const juce::String filterMorphAmount = "filter_morph_amount";
    
    // Tube Saturation Parameters
    const juce::String saturationDrive = "saturation_drive";
    const juce::String saturationMix = "saturation_mix";
    const juce::String saturationOversampling = "saturation_oversampling";
    
    // Visual Parameters
    const juce::String colormapType = "colormap_type";   // 0=Viridis, 1=Magma, 2=Purple_Cyan
    const juce::String particleCount = "particle_count";
    const juce::String animationSpeed = "animation_speed";
    
    // Performance Parameters
    const juce::String fastPaintMode = "fast_paint_mode"; // Low-latency mode for interaction
    const juce::String gpuAcceleration = "gpu_acceleration";
    
    // Master Parameters
    const juce::String gain = "gain";                     // -24..+24 dB master gain
}

// Parameter ranges and defaults
namespace ParameterRanges
{
    // Dual-Mode Engine
    const juce::NormalisableRange<float> modeRange(0.0f, 2.0f, 1.0f);
    const float modeDefault = 0.0f; // Synth
    
    const juce::NormalisableRange<float> blendRange(0.0f, 1.0f, 0.01f);
    const float blendDefault = 0.0f; // Full synth
    
    const juce::NormalisableRange<float> respeedRange(0.25f, 4.0f, 0.01f);
    const float respeedDefault = 1.0f;
    
    // Brush System
    const juce::NormalisableRange<float> brushTypeRange(0.0f, 3.0f, 1.0f);
    const float brushTypeDefault = 0.0f; // Paint
    
    const juce::NormalisableRange<float> brushSizeRange(1.0f, 128.0f, 1.0f);
    const float brushSizeDefault = 16.0f;
    
    const juce::NormalisableRange<float> brushStrengthRange(0.0f, 1.0f, 0.01f);
    const float brushStrengthDefault = 0.7f;
    
    const juce::NormalisableRange<float> quantizeStrengthRange(0.0f, 1.0f, 0.01f);
    const float quantizeStrengthDefault = 0.5f;
    
    // Grid & Tuning
    const juce::NormalisableRange<float> tuningIdRange(0.0f, 10.0f, 1.0f);
    const float tuningIdDefault = 0.0f; // 12TET
    
    // Z-Plane Morph
    const juce::NormalisableRange<float> zPlaneMorphRange(0.0f, 1.0f, 0.01f);
    const float zPlaneMorphXDefault = 0.5f;
    const float zPlaneMorphYDefault = 0.5f;
    
    // Spectral Processing
    const juce::NormalisableRange<float> spectralGainRange(0.0f, 2.0f, 0.01f);
    const float spectralGainDefault = 1.0f;
    
    const juce::NormalisableRange<float> spectralMixRange(0.0f, 1.0f, 0.01f);
    const float spectralMixDefault = 1.0f;
    
    const juce::NormalisableRange<float> fftSizeRange(256.0f, 2048.0f, 1.0f);
    const float fftSizeDefault = 512.0f;
    
    // Z-Plane Filter
    const juce::NormalisableRange<float> filterTypeRange(0.0f, 1.0f, 0.01f);
    const float filterTypeDefault = 0.0f; // Lowpass
    
    const juce::NormalisableRange<float> filterCutoffRange(20.0f, 20000.0f, 1.0f);
    const float filterCutoffDefault = 1000.0f;
    
    const juce::NormalisableRange<float> filterResonanceRange(0.1f, 10.0f, 0.01f);
    const float filterResonanceDefault = 0.7f;
    
    // Tube Saturation
    const juce::NormalisableRange<float> saturationDriveRange(0.0f, 10.0f, 0.01f);
    const float saturationDriveDefault = 1.0f;
    
    const juce::NormalisableRange<float> saturationMixRange(0.0f, 1.0f, 0.01f);
    const float saturationMixDefault = 0.3f;
    
    // Visual
    const juce::NormalisableRange<float> colormapTypeRange(0.0f, 2.0f, 1.0f);
    const float colormapTypeDefault = 2.0f; // Purple_Cyan (nebula default)
    
    const juce::NormalisableRange<float> particleCountRange(100.0f, 5000.0f, 10.0f);
    const float particleCountDefault = 1000.0f;
    
    // Master
    const juce::NormalisableRange<float> gainRange(-24.0f, 24.0f, 0.01f);
    const float gainDefault = 0.0f;
}

// Create parameter layout for ValueTreeState
inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    // Dual-Mode Engine Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::mode, "Mode",
        ParameterRanges::modeRange, ParameterRanges::modeDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::blend, "Blend",
        ParameterRanges::blendRange, ParameterRanges::blendDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::respeed, "Resynth Speed",
        ParameterRanges::respeedRange, ParameterRanges::respeedDefault));
    
    // Brush System Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::brushType, "Brush Type",
        ParameterRanges::brushTypeRange, ParameterRanges::brushTypeDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::brushSize, "Brush Size",
        ParameterRanges::brushSizeRange, ParameterRanges::brushSizeDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::brushStrength, "Brush Strength",
        ParameterRanges::brushStrengthRange, ParameterRanges::brushStrengthDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::quantizeStrength, "Quantize Strength",
        ParameterRanges::quantizeStrengthRange, ParameterRanges::quantizeStrengthDefault));
    
    // Grid & Tuning Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterBool>(
        ParameterIDs::gridOn, "Grid On", false));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::tuningId, "Tuning",
        ParameterRanges::tuningIdRange, ParameterRanges::tuningIdDefault));
    
    // Z-Plane Morph Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::zPlaneMorphX, "Z-Plane Morph X",
        ParameterRanges::zPlaneMorphRange, ParameterRanges::zPlaneMorphXDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::zPlaneMorphY, "Z-Plane Morph Y",
        ParameterRanges::zPlaneMorphRange, ParameterRanges::zPlaneMorphYDefault));
    
    // Spectral Processing Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::spectralGain, "Spectral Gain",
        ParameterRanges::spectralGainRange, ParameterRanges::spectralGainDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::spectralMix, "Spectral Mix",
        ParameterRanges::spectralMixRange, ParameterRanges::spectralMixDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::fftSize, "FFT Size",
        ParameterRanges::fftSizeRange, ParameterRanges::fftSizeDefault));
    
    // Z-Plane Filter Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::filterType, "Filter Type",
        ParameterRanges::filterTypeRange, ParameterRanges::filterTypeDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::filterCutoff, "Filter Cutoff",
        ParameterRanges::filterCutoffRange, ParameterRanges::filterCutoffDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::filterResonance, "Filter Resonance",
        ParameterRanges::filterResonanceRange, ParameterRanges::filterResonanceDefault));
    
    // Tube Saturation Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::saturationDrive, "Saturation Drive",
        ParameterRanges::saturationDriveRange, ParameterRanges::saturationDriveDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::saturationMix, "Saturation Mix",
        ParameterRanges::saturationMixRange, ParameterRanges::saturationMixDefault));
    
    // Visual Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::colormapType, "Colormap Type",
        ParameterRanges::colormapTypeRange, ParameterRanges::colormapTypeDefault));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::particleCount, "Particle Count",
        ParameterRanges::particleCountRange, ParameterRanges::particleCountDefault));
    
    // Performance Parameters
    parameters.push_back(std::make_unique<juce::AudioParameterBool>(
        ParameterIDs::fastPaintMode, "Fast Paint Mode", false));
    
    parameters.push_back(std::make_unique<juce::AudioParameterBool>(
        ParameterIDs::gpuAcceleration, "GPU Acceleration", true));
    
    // Master Gain
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::gain, "Master Gain",
        ParameterRanges::gainRange, ParameterRanges::gainDefault));
    
    return { parameters.begin(), parameters.end() };
}

} // namespace Params