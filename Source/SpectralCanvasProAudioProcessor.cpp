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
    
    // Initialize path tracking for this block
    wroteAudioThisBlock.store(false, std::memory_order_relaxed);
    currentPath.store(AudioPath::None, std::memory_order_relaxed);
    
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    
    // Clear any output channels that don't have input data
    for (int ch = totalNumInputChannels; ch < totalNumOutputChannels; ++ch)
        buffer.clear(ch, 0, numSamples);

    // Process parameter updates from UI thread (RT-safe)
    while (auto paramUpdate = parameterQueue.pop())
    {
        // Apply parameter changes with sample-accurate timing
        // TODO: Implement parameter smoothing and application
    }

    // ---- SELECT ONE AUDIO PATH ONLY ----
    bool useTestFeeder = useTestFeeder_.load(std::memory_order_relaxed);

    // ---- TEST FEEDER ----
    if (useTestFeeder) {
        // Test feeder path (default): spectral engine with test tones
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
        
        // RT-safe spectral processing - MUST write into buffer
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
            // Safety fallback: 220Hz beep to ensure we always hear something
            generateFallbackBeep(buffer, numSamples);
        }
        
        currentPath.store(AudioPath::TestFeeder, std::memory_order_relaxed);
        
        // Quick write check (cheap): look at first sample of ch0
        if (totalNumOutputChannels > 0 && numSamples > 0 && buffer.getReadPointer(0)[0] != 0.0f)
            wroteAudioThisBlock.store(true, std::memory_order_relaxed);
        
        // Update sample counter and return
        processedSampleCount_.fetch_add(numSamples, std::memory_order_relaxed);
        return;
    }

#ifdef PHASE4_EXPERIMENT
    // ---- PHASE 4 SYNTH ----
    if (true) { // Always enter this path when testFeeder is false
        // Phase 4 experimental path: oscillator bank with key filter
        // Pop all mask columns into spectral stub
        spectralStub.popAllMaskColumnsInto(maskColumnQueue);
        
        // Apply key filter if enabled
        if (keyFilterEnabled_.load(std::memory_order_relaxed)) {
            keyFilter.apply(spectralStub.getMagnitudesWritePtr(), 257);
        }
        
        // Render oscillator bank - MUST write into buffer
        const float gain = oscGain_.load(std::memory_order_relaxed);
        spectralStub.process(buffer, gain);
        
        currentPath.store(AudioPath::Phase4Synth, std::memory_order_relaxed);
        if (totalNumOutputChannels > 0 && numSamples > 0 && buffer.getReadPointer(0)[0] != 0.0f)
            wroteAudioThisBlock.store(true, std::memory_order_relaxed);
        
        // Update sample counter and return early
        processedSampleCount_.fetch_add(numSamples, std::memory_order_relaxed);
        return;
    }
#endif

    // ---- FALLBACK BEEP (safety) ----
    fallbackBeep(buffer); // tiny 220 Hz so we always hear something
    currentPath.store(AudioPath::Fallback, std::memory_order_relaxed);
    if (totalNumOutputChannels > 0 && numSamples > 0 && buffer.getReadPointer(0)[0] != 0.0f)
        wroteAudioThisBlock.store(true, std::memory_order_relaxed);
    
    processedSampleCount_.fetch_add(numSamples, std::memory_order_relaxed);
    
#if JUCE_DEBUG
    // Health check: detect silent audio in debug builds
    float rmsLevel = buffer.getRMSLevel(0, 0, numSamples);
    if (rmsLevel < 1e-6f) {
        static int silenceCounter = 0;
        if (++silenceCounter % 480 == 0) {  // Log every 10ms at 48kHz
            const char* pathName = useTestFeeder ? "TestFeeder" : "Phase4";
            juce::Logger::writeToLog(juce::String::formatted("Audio path (%s) producing silence", pathName));
        }
    }
#endif
}

// Minimal fallback beep generator (RT-safe)
void SpectralCanvasProAudioProcessor::fallbackBeep(juce::AudioBuffer<float>& buffer) noexcept
{
    static float ph = 0.0f;
    const float inc = float(2.0 * juce::MathConstants<double>::pi * 220.0 / getSampleRate());
    const int ns = buffer.getNumSamples();
    for (int n = 0; n < ns; ++n) {
        ph += inc; 
        if (ph > juce::MathConstants<float>::twoPi) 
            ph -= juce::MathConstants<float>::twoPi;
        const float s = 0.02f * std::sin(ph);
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.getWritePointer(ch)[n] += s;
    }
}

// Safety fallback audio generator (RT-safe)
void SpectralCanvasProAudioProcessor::generateFallbackBeep(juce::AudioBuffer<float>& buffer, int numSamples) noexcept
{
    static float phase = 0.0f;
    const float frequency = 220.0f; // A3 note
    const float phaseIncrement = frequency * 2.0f * juce::MathConstants<float>::pi / static_cast<float>(currentSampleRate);
    const float amplitude = 0.1f; // Moderate volume
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float sampleValue = std::sin(phase) * amplitude;
        
        // Write to all output channels
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.setSample(channel, sample, sampleValue);
        }
        
        phase += phaseIncrement;
        if (phase >= 2.0f * juce::MathConstants<float>::pi)
            phase -= 2.0f * juce::MathConstants<float>::pi;
    }
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