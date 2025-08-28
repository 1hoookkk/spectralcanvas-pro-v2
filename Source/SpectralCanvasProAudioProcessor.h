#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "Core/Params.h"
#include "Core/MessageBus.h"
#include "DSP/SpectralEngine.h"

class SpectralCanvasProAudioProcessor : public juce::AudioProcessor
{
public:
    SpectralCanvasProAudioProcessor();
    ~SpectralCanvasProAudioProcessor() override;

    // AudioProcessor interface
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) noexcept override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override { juce::ignoreUnused(index); }
    const juce::String getProgramName(int index) override { juce::ignoreUnused(index); return {}; }
    void changeProgramName(int index, const juce::String& newName) override { juce::ignoreUnused(index, newName); }
    
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameter management
    juce::AudioProcessorValueTreeState& getValueTreeState() { return apvts; }
    
    // Inter-thread communication
    SpectralDataQueue& getSpectralDataQueue() { return spectralDataQueue; }
    ParameterQueue& getParameterQueue() { return parameterQueue; }
    MaskColumnQueue& getMaskColumnQueue() { return maskColumnQueue; }

    // Public APVTS access for editor (matches plugin designer's convention)
    juce::AudioProcessorValueTreeState apvts;

private:
    
    // Inter-thread communication (lock-free)
    SpectralDataQueue spectralDataQueue;
    ParameterQueue parameterQueue;
    MaskColumnQueue maskColumnQueue;
    
    // Audio processing state
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
    
    // RT-safe spectral processing
    std::unique_ptr<SpectralEngine> spectralEngine;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralCanvasProAudioProcessor)
};