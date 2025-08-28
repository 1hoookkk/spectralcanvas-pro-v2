#include "SpectralCanvasProAudioProcessor.h"
#include "SpectralCanvasProEditor.h"
#include <chrono>

SpectralCanvasProAudioProcessor::SpectralCanvasProAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", Params::createParameterLayout())
{
}

SpectralCanvasProAudioProcessor::~SpectralCanvasProAudioProcessor()
{
#ifdef PHASE4_EXPERIMENT
    // Remove parameter listeners
    apvts.removeParameterListener(Params::ParameterIDs::useTestFeeder, this);
    apvts.removeParameterListener(Params::ParameterIDs::keyFilterEnabled, this);
    apvts.removeParameterListener(Params::ParameterIDs::oscGain, this);
    apvts.removeParameterListener(Params::ParameterIDs::scaleType, this);
    apvts.removeParameterListener(Params::ParameterIDs::rootNote, this);
#endif
}

void SpectralCanvasProAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    
    // Initialize RT-safe sample loader
    sampleLoader.initialize(sampleRate);
    
    // Initialize RT-safe test feeder
    maskTestFeeder.initialize(sampleRate, 257); // NUM_BINS for 512-point FFT
    
    // Initialize RT-safe spectral processing components
    spectralEngine = std::make_unique<SpectralEngine>();
    spectralEngine->initialize(sampleRate, samplesPerBlock);
    
    // Connect SampleLoader to SpectralEngine (RT-safe pointer sharing)
    spectralEngine->setSampleLoader(&sampleLoader);
    
    // Initialize Phase 2-3 validation infrastructure  
    processedSampleCount_.store(0, std::memory_order_relaxed);
    // Store epoch in nanoseconds for unified timebase (per spec)
    epochSteadyNanos_.store(std::chrono::steady_clock::now().time_since_epoch().count(), 
                            std::memory_order_relaxed);
    latencyTracker_.reset();
    
    // Clear any existing data in queues
    spectralDataQueue.clear();
    parameterQueue.clear();
    maskColumnQueue.clear();
    
#ifdef PHASE4_EXPERIMENT
    // Initialize Phase 4 experimental components
    const int fftSize = 512;
    const int numBins = fftSize / 2 + 1;  // 257
    const int channels = getTotalNumOutputChannels();
    
    spectralStub.prepare(sampleRate, fftSize, numBins, channels);
    keyFilter.prepare(fftSize, numBins, sampleRate);
    
    // Initialize with default scale (C major)
    keyFilter.rebuildAsync(0, dsp::ScaleType::Major);
    
    // Set up parameter listeners for Phase 4
    
    // Initialize parameter atomics from APVTS values
    useTestFeeder_.store(apvts.getParameterAsValue(Params::ParameterIDs::useTestFeeder).getValue(), std::memory_order_relaxed);
    keyFilterEnabled_.store(apvts.getParameterAsValue(Params::ParameterIDs::keyFilterEnabled).getValue(), std::memory_order_relaxed);
    oscGain_.store(apvts.getParameterAsValue(Params::ParameterIDs::oscGain).getValue(), std::memory_order_relaxed);
    scaleType_.store(static_cast<int>(apvts.getParameterAsValue(Params::ParameterIDs::scaleType).getValue()), std::memory_order_relaxed);
    rootNote_.store(static_cast<int>(apvts.getParameterAsValue(Params::ParameterIDs::rootNote).getValue()), std::memory_order_relaxed);
    
    // Add parameter listeners for real-time updates
    apvts.addParameterListener(Params::ParameterIDs::useTestFeeder, this);
    apvts.addParameterListener(Params::ParameterIDs::keyFilterEnabled, this);
    apvts.addParameterListener(Params::ParameterIDs::oscGain, this);
    apvts.addParameterListener(Params::ParameterIDs::scaleType, this);
    apvts.addParameterListener(Params::ParameterIDs::rootNote, this);
#endif
}

void SpectralCanvasProAudioProcessor::releaseResources()
{
    if (spectralEngine)
        spectralEngine->reset();
        
    sampleLoader.shutdown();
    maskTestFeeder.reset();
}

void SpectralCanvasProAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, 
                                                   juce::MidiBuffer& midiMessages) noexcept
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
    
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    
    // Clear ALL output channels at the start
    buffer.clear();

#ifdef PHASE4_EXPERIMENT
    // Phase 4 experimental path: oscillator bank with key filter
    if (!useTestFeeder_.load(std::memory_order_relaxed)) {
        // Process parameter updates from UI thread (RT-safe)
        while (auto paramUpdate = parameterQueue.pop())
        {
            // Apply parameter changes with sample-accurate timing
            // TODO: Implement parameter smoothing and application
        }
        
        // Pop all mask columns into spectral stub
        spectralStub.popAllMaskColumnsInto(maskColumnQueue);
        
        // Apply key filter if enabled
        if (keyFilterEnabled_.load(std::memory_order_relaxed)) {
            keyFilter.apply(spectralStub.getMagnitudesWritePtr(), 257);
        }
        
        // Render oscillator bank
        const float gain = oscGain_.load(std::memory_order_relaxed);
        spectralStub.process(buffer, gain);
        
        // Update sample counter and return early
        processedSampleCount_.fetch_add(numSamples, std::memory_order_relaxed);
        return;
    }
#endif

    // Test feeder path - fallback for Phase 4 or primary for Phase 2-3
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
        
        // Calculate latency for Phase 2-3 validation (per unified timebase spec)
        if (currentMaskColumn.timestampSamples > 0.0)
        {
            uint64_t currentAudioSamples = processedSampleCount_.load(std::memory_order_relaxed);
            uint64_t uiTimestampSamples = static_cast<uint64_t>(currentMaskColumn.timestampSamples);
            
            if (currentAudioSamples >= uiTimestampSamples)
            {
                uint64_t paintLatencySamples = currentAudioSamples - uiTimestampSamples;
                float latencyMs = (paintLatencySamples * 1000.0f) / static_cast<float>(currentSampleRate);
                
                // Convert to microseconds for RTLatencyTracker
                uint32_t latencyMicros = static_cast<uint32_t>(latencyMs * 1000.0f);
                latencyTracker_.recordLatency(latencyMicros);
            }
        }
        
        if (spectralEngine)
        {
            spectralEngine->updateCurrentMask(&currentMaskColumn);
        }
    }
    
    // Process test mask columns for paint-to-audio validation (RT-safe)
    MaskColumn testMask;
    while (maskTestFeeder.tryPopMask(testMask))
    {
        if (spectralEngine && spectralEngine->isInitialized())
        {
            spectralEngine->applyMaskColumn(testMask);
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
    
    // Update sample counter for latency calculation
    processedSampleCount_.fetch_add(numSamples, std::memory_order_relaxed);
    
#if JUCE_DEBUG
    // Health check: detect silent audio in debug builds
    float rmsLevel = buffer.getRMSLevel(0, 0, numSamples);
    if (rmsLevel < 1e-6f && !useTestFeeder_.load(std::memory_order_relaxed)) {
        // Add a single-sample impulse for debugging (very quiet)
        if (numSamples > 0) {
            buffer.setSample(0, 0, 0.001f);  // Tiny click to verify audio path
        }
        static int silenceCounter = 0;
        if (++silenceCounter % 480 == 0) {  // Log every 10ms at 48kHz
            juce::Logger::writeToLog("Phase 4: Audio path silent (health check triggered)");
        }
    }
#endif
}

#ifdef PHASE4_EXPERIMENT
void SpectralCanvasProAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == Params::ParameterIDs::useTestFeeder)
    {
        useTestFeeder_.store(newValue > 0.5f, std::memory_order_relaxed);
    }
    else if (parameterID == Params::ParameterIDs::keyFilterEnabled)
    {
        keyFilterEnabled_.store(newValue > 0.5f, std::memory_order_relaxed);
    }
    else if (parameterID == Params::ParameterIDs::oscGain)
    {
        oscGain_.store(newValue, std::memory_order_relaxed);
    }
    else if (parameterID == Params::ParameterIDs::scaleType)
    {
        const int scaleType = static_cast<int>(newValue);
        scaleType_.store(scaleType, std::memory_order_relaxed);
        
        // Rebuild key filter LUT with new scale
        const int root = rootNote_.load(std::memory_order_relaxed);
        dsp::ScaleType scale = static_cast<dsp::ScaleType>(scaleType);
        keyFilter.rebuildAsync(root, scale);
    }
    else if (parameterID == Params::ParameterIDs::rootNote)
    {
        const int rootNote = static_cast<int>(newValue);
        rootNote_.store(rootNote, std::memory_order_relaxed);
        
        // Rebuild key filter LUT with new root
        const int scaleType = scaleType_.load(std::memory_order_relaxed);
        dsp::ScaleType scale = static_cast<dsp::ScaleType>(scaleType);
        keyFilter.rebuildAsync(rootNote, scale);
    }
}
#else
void SpectralCanvasProAudioProcessor::parameterChanged(const juce::String&, float)
{
    // No-op when Phase 4 is disabled
}
#endif

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

bool SpectralCanvasProAudioProcessor::loadSampleFile(const juce::File& audioFile)
{
    // This is called from UI thread - safe to allocate
    return sampleLoader.loadSampleFile(audioFile);
}

void SpectralCanvasProAudioProcessor::generateImmediateAudioFeedback()
{
    // Start diagonal sweep test pattern for paint-to-audio validation
    maskTestFeeder.startDiagonalSweep();
}

// Phase 2-3 getter methods for debug overlay
int SpectralCanvasProAudioProcessor::getBlockSize() const
{
    return currentBlockSize;
}

double SpectralCanvasProAudioProcessor::getSampleRate() const
{
    return currentSampleRate;
}

SpectralCanvasProAudioProcessor::PerformanceMetrics SpectralCanvasProAudioProcessor::getPerformanceMetrics() const
{
    PerformanceMetrics metrics;
    
    // Latency statistics from RT-safe tracker
    metrics.medianLatencyMs = latencyTracker_.getMedianLatencyMs();
    metrics.p95LatencyMs = latencyTracker_.getP95LatencyMs();
    
    // Queue statistics
    metrics.queueDepth = maskColumnQueue.getApproxQueueDepth();
    metrics.dropCount = maskColumnQueue.getDropCount();
    
    // Audio processing stats
    metrics.processedSamples = processedSampleCount_.load(std::memory_order_relaxed);
    
    // TODO: XRun detection from host if available
    metrics.xrunCount = 0;
    
    return metrics;
}

// Plugin factory functions
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralCanvasProAudioProcessor();
}