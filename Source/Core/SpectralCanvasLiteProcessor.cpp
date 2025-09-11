#include "SpectralCanvasLiteProcessor.h"
#include "../UI/SpectralCanvasLiteEditor.h"
#include "../Utils/TracyProfiler.h"

using namespace juce;

SpectralCanvasLiteProcessor::SpectralCanvasLiteProcessor()
  : AudioProcessor(BusesProperties().withInput ("Input",  AudioChannelSet::stereo(), true)
                                      .withOutput("Output", AudioChannelSet::stereo(), true)),
    apvts_(*this, nullptr, "PARAMS", Params::createParameterLayout())
{
}

bool SpectralCanvasLiteProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& in  = layouts.getChannelSet(true,  0);
    const auto& out = layouts.getChannelSet(false, 0);
    return in == out && (in == AudioChannelSet::mono() || in == AudioChannelSet::stereo());
}

void SpectralCanvasLiteProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    FloatVectorOperations::disableDenormalisedNumberSupport();

    spectral_.prepare(sampleRate, samplesPerBlock);
    synth_.prepare(sampleRate, samplesPerBlock);

    setLatencySamples(spectral_.getLatencySamples());
}

void SpectralCanvasLiteProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midi)
{
    SPECTRAL_PROFILE_ZONE("hop_total");
    
    ignoreUnused(midi);
    ScopedNoDenormals noDenormals;

    const auto mode = static_cast<ProcessingMode>((int)apvts_.getRawParameterValue(Params::kProcessingMode)->load());
    
    {
        SPECTRAL_PROFILE_ZONE("brush_drain");
        int drained = spectral_.drainEvents(queue_, 16);
        if (drained > 0) {
            eventsDrained_.fetch_add(drained, std::memory_order_relaxed);
        }
    }

    const float maskFloorDb  = apvts_.getRawParameterValue(Params::kMaskFloorDb)->load();
    const float outputTrimDb = apvts_.getRawParameterValue(Params::kOutputTrimDb)->load();

    if (mode == ProcessingMode::Effect)
    {
        spectral_.process(buffer, maskFloorDb, outputTrimDb);
        return;
    }

    if (mode == ProcessingMode::Synth)
    {
        buffer.clear();
        synth_.process(buffer);
        return;
    }

    if (mode == ProcessingMode::Hybrid)
    {
        spectral_.process(buffer, maskFloorDb, outputTrimDb);
        return;
    }
}

AudioProcessorEditor* SpectralCanvasLiteProcessor::createEditor()
{
    return new SpectralCanvasLiteEditor(*this);
}

void SpectralCanvasLiteProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = apvts_.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary(*xml, destData);
}

void SpectralCanvasLiteProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts_.state.getType()))
            apvts_.replaceState(ValueTree::fromXml(*xml));
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralCanvasLiteProcessor();
}
