#include "SampleManager.h"
#include <juce_audio_formats/juce_audio_formats.h>

bool SampleManager::loadFromFile (const juce::File& file, double targetSR)
{
    if (!file.existsAsFile()) return false;
    
    juce::AudioFormatManager fm; 
    fm.registerBasicFormats();
    std::unique_ptr<juce::AudioFormatReader> r (fm.createReaderFor (file));
    if (!r || r->lengthInSamples <= 0) return false;

    juce::AudioBuffer<float> raw ((int) r->numChannels, (int) r->lengthInSamples);
    r->read (&raw, 0, (int) r->lengthInSamples, 0, true, true);

    // Downmix to mono for MVP
    juce::AudioBuffer<float> mono (1, raw.getNumSamples());
    mono.clear();
    for (int ch = 0; ch < raw.getNumChannels(); ++ch)
        mono.addFrom (0, 0, raw, ch, 0, raw.getNumSamples(), 1.0f / juce::jmax (1, raw.getNumChannels()));

    if (r->sampleRate != targetSR)
    {
        const double ratio  = targetSR / r->sampleRate;
        const int    outLen = (int) std::ceil (mono.getNumSamples() * ratio);
        juce::AudioBuffer<float> resampled (1, outLen);
        juce::LagrangeInterpolator interp;
        int written = interp.process (ratio, mono.getReadPointer (0), resampled.getWritePointer (0), outLen);
        resampled.setSize (1, written, true, true, true);
        data.audio      = std::move (resampled);
        data.sampleRate = targetSR;
    }
    else
    {
        data.audio      = std::move (mono);
        data.sampleRate = r->sampleRate;
    }
    data.loaded = true;
    return true;
}