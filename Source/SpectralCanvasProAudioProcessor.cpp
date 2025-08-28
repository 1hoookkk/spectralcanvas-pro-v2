#include "SpectralCanvasProAudioProcessor.h"
#include "SpectralCanvasProEditor.h"

SpectralCanvasProAudioProcessor::SpectralCanvasProAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", Params::createParameterLayout())
{
}

SpectralCanvasProAudioProcessor::~SpectralCanvasProAudioProcessor()
{
}

void SpectralCanvasProAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    
    // Initialize RT-safe spectral processing components
    spectralEngine = std::make_unique<SpectralEngine>();
    spectralEngine->initialize(sampleRate, samplesPerBlock);
    
    // Clear any existing data in queues
    spectralDataQueue.clear();
    parameterQueue.clear();
    maskColumnQueue.clear();
}

void SpectralCanvasProAudioProcessor::releaseResources()
{
    if (spectralEngine)
        spectralEngine->reset();
}

void SpectralCanvasProAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, 
                                                   juce::MidiBuffer& midiMessages) noexcept
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
    
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    
    // Clear any output channels that don't contain input data
    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
        buffer.clear(channel, 0, numSamples);
    
    // Process parameter updates from UI thread (RT-safe)
    while (auto paramUpdate = parameterQueue.pop())
    {
        // Apply parameter changes with sample-accurate timing
        // TODO: Implement parameter smoothing and application
    }
    
    // Process mask column updates from GPU thread (RT-safe)
    static MaskColumn currentMaskColumn; // Static to keep mask data alive
    while (auto maskColumn = maskColumnQueue.pop())
    {
        currentMaskColumn = *maskColumn; // Copy mask data
        if (spectralEngine)
        {
            spectralEngine->updateCurrentMask(&currentMaskColumn);
        }
    }
    
    // RT-safe spectral processing (process mono for now)
    if (spectralEngine && spectralEngine->isInitialized())
    {
        // Process first channel through spectral engine
        const float* inputData = buffer.getReadPointer(0);
        float* outputData = buffer.getWritePointer(0);
        
        spectralEngine->processBlock(inputData, outputData, numSamples);
        
        // Copy to other channels if stereo
        for (int channel = 1; channel < totalNumOutputChannels; ++channel)
        {
            buffer.copyFrom(channel, 0, buffer, 0, 0, numSamples);
        }
        
        // Extract spectral data for visualization
        if (spectralDataQueue.hasSpaceAvailable())
        {
            SpectralFrame frame;
            if (spectralEngine->extractSpectralFrame(frame))
            {
                spectralDataQueue.push(frame);
            }
        }
    }
    else
    {
        // Pass through unchanged if spectral engine not ready
    }
}

juce::AudioProcessorEditor* SpectralCanvasProAudioProcessor::createEditor()
{
    return new SpectralCanvasProEditor(*this);
}

void SpectralCanvasProAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SpectralCanvasProAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr && xmlState->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

// Plugin factory functions
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralCanvasProAudioProcessor();
}