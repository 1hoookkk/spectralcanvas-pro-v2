#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>

struct SampleData {
    juce::AudioBuffer<float> audio;  // mono for MVP
    double sampleRate = 48000.0;
    bool   loaded     = false;
};

class SampleManager {
public:
    bool loadFromFile (const juce::File& file, double targetSR);
    const SampleData& get() const noexcept { return data; }
    void clear() { data = {}; }
private:
    SampleData data;
};