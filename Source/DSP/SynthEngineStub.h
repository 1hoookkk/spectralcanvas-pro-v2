#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

// Placeholder: in Synth mode, you can generate audio here later.
// For now it outputs silence (pass-through silence).
class SynthEngineStub
{
public:
    void prepare(double /*sr*/, int /*blockSize*/) {}
    void process(juce::AudioBuffer<float>& buffer)
    {
        buffer.clear();
    }
    void reset() {}
};
