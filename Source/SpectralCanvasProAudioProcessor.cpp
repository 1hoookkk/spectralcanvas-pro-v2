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
    
    // set initial audio path from current params (message thread)
    setAudioPathFromParams();
    lastPath_ = currentPath_.load(std::memory_order_relaxed);
    
    // Initialize RT-safe sample loader
    sampleLoader.initialize(sampleRate);
    
    // Initialize RT-safe test feeder
    maskTestFeeder.initialize(sampleRate, 257); // NUM_BINS for 512-point FFT
    
    // Initialize RT-safe spectral processing components
    spectralEngine = std::make_unique<SpectralEngine>();
    spectralEngine->initialize(sampleRate, samplesPerBlock);
    
    // Connect SampleLoader to SpectralEngine (RT-safe pointer sharing)
    spectralEngine->setSampleLoader(&sampleLoader);
    
    // Initialize modern JUCE DSP-based spectral painting processor
    spectralPaintProcessor = std::make_unique<SpectralPaintProcessor>();
    
    // Prepare with JUCE DSP spec
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<uint32_t>(samplesPerBlock);
    spec.numChannels = static_cast<uint32_t>(std::max(getTotalNumInputChannels(), getTotalNumOutputChannels()));
    
    spectralPaintProcessor->prepare(spec);
    
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
    
    // RT-SAFE: processBlock setup - no logging in RT path
    // Queue diagnostics moved to UI thread for HUD display
    
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
    useModernPaint_.store(apvts.getParameterAsValue(Params::ParameterIDs::useModernPaint).getValue(), std::memory_order_relaxed);
    
    // Add parameter listeners for real-time updates
    apvts.addParameterListener(Params::ParameterIDs::useTestFeeder, this);
    apvts.addParameterListener(Params::ParameterIDs::keyFilterEnabled, this);
    apvts.addParameterListener(Params::ParameterIDs::oscGain, this);
    apvts.addParameterListener(Params::ParameterIDs::scaleType, this);
    apvts.addParameterListener(Params::ParameterIDs::rootNote, this);
    apvts.addParameterListener(Params::ParameterIDs::useModernPaint, this);
#endif
    
    // Initialize RT-safe parameter smoothers (50ms ramp time for smooth automation)
    const float rampTimeSec = 0.05f;  // 50ms ramp
    oscGainSmoother_.reset(sampleRate, rampTimeSec);
    brushSizeSmoother_.reset(sampleRate, rampTimeSec);
    brushStrengthSmoother_.reset(sampleRate, rampTimeSec);
    
    // Set initial values from current parameters
    oscGainSmoother_.setCurrentAndTargetValue(oscGain_.load(std::memory_order_relaxed));
    brushSizeSmoother_.setCurrentAndTargetValue(16.0f);  // Default brush size
    brushStrengthSmoother_.setCurrentAndTargetValue(0.7f);  // Default brush strength
    
    // Initialize smoothed params (50ms ramp time)
    oscGainSmoother_.reset(sampleRate, smoothingTimeSec_);
    brushSizeSmoother_.reset(sampleRate, smoothingTimeSec_);
    brushStrengthSmoother_.reset(sampleRate, smoothingTimeSec_);
    
    // Seed current + targets from APVTS
    oscGainSmoother_.setCurrentAndTargetValue(getParamFast(Params::ParameterIDs::oscGain));
    brushSizeSmoother_.setCurrentAndTargetValue(getParamFast(Params::ParameterIDs::brushSize));
    brushStrengthSmoother_.setCurrentAndTargetValue(getParamFast(Params::ParameterIDs::brushStrength));
    
#ifdef PHASE4_EXPERIMENT
    // Pre-calculate timestamp for RT safety (Phase4 only)
    using namespace std::chrono;
    auto now = steady_clock::now();
    rtTimestampUs_.store(static_cast<uint64_t>(duration_cast<microseconds>(now.time_since_epoch()).count()), std::memory_order_relaxed);
#endif
}

void SpectralCanvasProAudioProcessor::releaseResources()
{
    if (spectralEngine)
        spectralEngine->reset();
        
    if (spectralPaintProcessor)
        spectralPaintProcessor->reset();
        
    sampleLoader.shutdown();
    maskTestFeeder.reset();
}

// Called from parameter listener / message thread whenever GUI toggles mode
void SpectralCanvasProAudioProcessor::setAudioPathFromParams()
{
#ifdef PHASE4_EXPERIMENT
    const bool useTestFeeder = useTestFeeder_.load(std::memory_order_relaxed);
    const bool useModernPaint = useModernPaint_.load(std::memory_order_relaxed);
    
    AudioPath path;
    if (useTestFeeder)
        path = AudioPath::TestFeeder;
    else if (useModernPaint)
        path = AudioPath::ModernPaint;
    else
        path = AudioPath::Phase4Synth;
        
    currentPath_.store(path, std::memory_order_release);
#else
    currentPath_.store(AudioPath::TestFeeder, std::memory_order_release);
#endif
}

#ifdef PHASE4_EXPERIMENT
void SpectralCanvasProAudioProcessor::rtResetPhase4_() noexcept
{
    spectralStub.reset(); // zero phases/magnitudes; RT-safe
}

void SpectralCanvasProAudioProcessor::rtResetTestFeeder_() noexcept
{
    // Reset test feeder state if needed
    // Static phase will be reset naturally due to scope
}

void SpectralCanvasProAudioProcessor::rtResetModernPaint_() noexcept
{
    if (spectralPaintProcessor)
        spectralPaintProcessor->reset();
}

int SpectralCanvasProAudioProcessor::getActiveBinCount() const noexcept
{
    return spectralStub.getActiveBinCount();
}

int SpectralCanvasProAudioProcessor::getNumBins() const noexcept
{
    return spectralStub.getNumBins();
}
#endif

void SpectralCanvasProAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, 
                                                   juce::MidiBuffer& midiMessages) noexcept
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
    
    const int numSamples = buffer.getNumSamples();
    buffer.clear(); // clear once at the top
    
    // RT-safe perf counters: no clocks on audio thread
    lastBlockSize_.store(numSamples, std::memory_order_relaxed);
    totalBlocksProcessed_.fetch_add(1, std::memory_order_relaxed);
    totalSamplesProcessed_.fetch_add(static_cast<uint64_t>(numSamples), std::memory_order_relaxed);

    // RT path transition handling
    const auto path = currentPath_.load(std::memory_order_acquire);
    if (path != lastPath_)
    {
#ifdef PHASE4_EXPERIMENT
        if (path == AudioPath::Phase4Synth)   rtResetPhase4_();
        else if (path == AudioPath::TestFeeder) rtResetTestFeeder_();
        else if (path == AudioPath::ModernPaint) rtResetModernPaint_();
#endif
        lastPath_ = path;
    }

    // Initialize write detection for this block
    wroteAudioFlag_.store(false, std::memory_order_relaxed);
    
#if PHASE4_DEBUG_TAP
    // Record queue address on audio thread for integrity checking
    debugTap_.queueAddrOnAudio.store(reinterpret_cast<uintptr_t>(&maskColumnQueue), std::memory_order_relaxed);
#endif

    // Update parameter smoothers with current target values (RT-safe)
    oscGainSmoother_.setTargetValue(oscGain_.load(std::memory_order_relaxed));
    
    // Use smoothed params on the audio thread
    const float oscGain = juce::jlimit(1.0e-6f, 1.0f, oscGainSmoother_.getNextValue());
    const float brushSize = brushSizeSmoother_.getNextValue();
    const float brushStrength = brushStrengthSmoother_.getNextValue();
    
    // Snapshot only the params needed for the chosen path (no APVTS lookups here)
    const bool keyFilterOn = keyFilterEnabled_.load(std::memory_order_relaxed);

    switch (path)
    {
        case AudioPath::Silent:
#ifdef PHASE4_EXPERIMENT
            // Drain queued paint columns so "Drops" stays at 0 in silent mode
            spectralStub.popAllMaskColumnsInto(maskColumnQueue); // discard all data
#endif
            // Nothing is rendered; output remains cleared.
            return;

        case AudioPath::TestFeeder:
        {
#ifdef PHASE4_EXPERIMENT
            // Don't drain queue - preserve paint data for potential Phase4 switch
            // TestFeeder generates its own audio independently
#endif
            // *** Render ONLY the test feeder here ***
            static float testPhase = 0.0f;
            const float freq = 440.0f;
            const float incr = juce::MathConstants<float>::twoPi * freq / static_cast<float>(getSampleRate());
            
            for (int n = 0; n < numSamples; ++n)
            {
                testPhase += incr;
                if (testPhase >= juce::MathConstants<float>::twoPi) 
                    testPhase -= juce::MathConstants<float>::twoPi;
                
                const float sample = 0.2f * std::sin(testPhase);
                for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
                    buffer.getWritePointer(ch)[n] = sample;
            }
            
            // TestFeeder always writes (direct tone)
            wroteAudioFlag_.store(true, std::memory_order_relaxed);
            return;
        }

        case AudioPath::Phase4Synth:
        {
#ifdef PHASE4_EXPERIMENT
            // Increment diagnostic counter to prove this branch executes
            phase4Blocks_.fetch_add(1, std::memory_order_relaxed);
            
            // 1) Bounded pop from mask queue (RT-safe, no allocations)
            int masksPopped = 0;
            while (masksPopped < MAX_MASKS_PER_BLOCK)
            {
                auto maskOpt = maskColumnQueue.pop();
                if (!maskOpt.has_value()) break;
                
                maskRing_[maskRingIdx_] = std::move(*maskOpt);
                maskRingIdx_ = (maskRingIdx_ + 1) % MAX_MASKS_PER_BLOCK;
                masksPopped++;
            }
            
            // Masks are now applied directly in popAllMaskColumnsInto above
            if (masksPopped > 0)
            {
                masksApplied_.fetch_add(masksPopped, std::memory_order_relaxed);
                lastMaskTimestamp_.store(getCurrentTimeUs(), std::memory_order_relaxed);
            }
            
            // 2) Gate (only apply when explicitly enabled)
            if (keyFilterOn) {
                keyFilter.apply(spectralStub.getMagnitudesWritePtr(), 257);
            } else {
                // Ensure KeyFilter is disabled when not wanted
                keyFilter.enabled.store(false, std::memory_order_relaxed);
            }
            
            // 3) Render
            spectralStub.process(buffer, oscGain);
            
            // DIAGNOSTIC: Force-audible chirp to prove Phase4 branch executes
            #ifndef PHASE4_DIAG_CHIRP
            #define PHASE4_DIAG_CHIRP 1
            #endif
            
            #if PHASE4_DIAG_CHIRP
            // If buffer is still silent, emit a -36 dB sine for 120 ms every second
            static int   chirpSamples = 0;
            static float chirpPhase   = 0.0f;
            const int sr  = (int)getSampleRate();
            const int N   = numSamples;
            float currentRms = buffer.getNumChannels() > 0 ? buffer.getRMSLevel(0, 0, N) : 0.0f;
            bool needChirp = (currentRms < 1.0e-6f);
            
            if (needChirp) {
                const int period = sr;                      // 1 second
                const int dur    = (int)(0.12 * sr);       // 120 ms audible blip
                chirpSamples = (chirpSamples + N) % period;
                if (chirpSamples < dur) {
                    const float gain = 0.0158f;            // ~ -36 dB
                    const float inc  = juce::MathConstants<float>::twoPi * 440.0f / (float)sr;
                    for (int n = 0; n < N; ++n) {
                        chirpPhase += inc; 
                        if (chirpPhase >= juce::MathConstants<float>::twoPi) 
                            chirpPhase -= juce::MathConstants<float>::twoPi;
                        const float s = std::sin(chirpPhase) * gain;
                        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
                            buffer.getWritePointer(ch)[n] += s;
                    }
                }
            }
            #endif
            
            // Mark "wrote" only if buffer has anything > ~ -120 dBFS
            const float rms = buffer.getNumChannels() > 0
                ? buffer.getRMSLevel(0, 0, numSamples) : 0.0f;
            if (rms > 1.0e-6f)
                wroteAudioFlag_.store(true, std::memory_order_relaxed);
#endif
            return;
        }
        
        case AudioPath::ModernPaint:
        {
            // Modern JUCE DSP-based spectral painting
            if (spectralPaintProcessor && spectralPaintProcessor->isInitialized())
            {
                // Create JUCE DSP context for processing
                juce::dsp::AudioBlock<float> audioBlock(buffer);
                juce::dsp::ProcessContextReplacing<float> context(audioBlock);
                
                // Process through spectral paint node
                spectralPaintProcessor->processBlock(context);
                
                // Check if audio was generated
                const float rms = buffer.getNumChannels() > 0
                    ? buffer.getRMSLevel(0, 0, numSamples) : 0.0f;
                if (rms > 1.0e-6f)
                    wroteAudioFlag_.store(true, std::memory_order_relaxed);
            }
            return;
        }
    }
    
#if JUCE_DEBUG
    // RT-SAFE: Silent audio detection using atomic counters only
    float rmsLevel = buffer.getRMSLevel(0, 0, numSamples);
    if (rmsLevel < 1e-6f) {
        silenceBlockCount_.fetch_add(1, std::memory_order_relaxed);
    } else {
        silenceBlockCount_.store(0, std::memory_order_relaxed);
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
        setAudioPathFromParams(); // Update path atomically
    }
    else if (parameterID == Params::ParameterIDs::useModernPaint)
    {
        useModernPaint_.store(newValue > 0.5f, std::memory_order_relaxed);
        setAudioPathFromParams(); // Update path atomically
    }
    else if (parameterID == Params::ParameterIDs::keyFilterEnabled)
    {
        keyFilterEnabled_.store(newValue > 0.5f, std::memory_order_relaxed);
    }
    else if (parameterID == Params::ParameterIDs::oscGain)
    {
        oscGain_.store(newValue, std::memory_order_relaxed);
        // Update smoother target (message thread is safe for this)
        oscGainSmoother_.setTargetValue(newValue);
    }
    else if (parameterID == Params::ParameterIDs::brushSize)
    {
        // Update smoother target for brush size
        brushSizeSmoother_.setTargetValue(newValue);
    }
    else if (parameterID == Params::ParameterIDs::brushStrength)
    {
        // Update smoother target for brush strength
        brushStrengthSmoother_.setTargetValue(newValue);
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

#ifdef PHASE4_EXPERIMENT
uint64_t SpectralCanvasProAudioProcessor::getCurrentTimeUs() noexcept
{
    // Simple fallback - use basic counter for now
    // This avoids RT violations while maintaining functionality
    static std::atomic<uint64_t> counter{0};
    return counter.fetch_add(1, std::memory_order_relaxed);
}
#else
uint64_t SpectralCanvasProAudioProcessor::getCurrentTimeUs() noexcept
{
    // Fallback when Phase4 is disabled - return 0 
    return 0;
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

bool SpectralCanvasProAudioProcessor::pushMaskColumn(const MaskColumn& mask)
{
    // DEBUG: Log that pushMaskColumn is being called
    // RT-SAFE: Track push attempts with atomic counter
    pushMaskAttempts_.fetch_add(1, std::memory_order_relaxed);
    
    // TESTING: Temporarily bypass path check to force data through
    /*
    // Only push when Phase4 is active - double-check path state
    if (getCurrentPath() != AudioPath::Phase4Synth) {
        // RT-SAFE: Count rejections for HUD display
        pushMaskRejects_.fetch_add(1, std::memory_order_relaxed);
        return false; // Silently ignore when not in Phase4 mode
    }
    */
    
    // RT-SAFE: Proceeding to queue - no logging
    
#if PHASE4_DEBUG_TAP
    // Create enhanced column with debug sequence
    static std::atomic<uint64_t> gSeq{1};
    MaskColumnEx colEx;
    
    // Copy base MaskColumn data
    colEx.frameIndex = mask.frameIndex;
    colEx.numBins = mask.numBins;
    colEx.timestampSamples = mask.timestampSamples;
    colEx.uiTimestampMicros = mask.uiTimestampMicros;
    colEx.sequenceNumber = mask.sequenceNumber;
    std::memcpy(colEx.values, mask.values, sizeof(colEx.values));
    
    // Add debug sequence
    colEx.debugSeq = gSeq.fetch_add(1, std::memory_order_relaxed);
    
    // Attempt to push enhanced column to queue
    const bool success = maskColumnQueue.push(colEx);
    
    // Update debug tap
    debugTap_.queueAddrOnPush.store(reinterpret_cast<uintptr_t>(&maskColumnQueue), std::memory_order_relaxed);
    debugTap_.lastSeqPushed.store(colEx.debugSeq, std::memory_order_relaxed);
    
    if (success) {
        debugTap_.pushes.fetch_add(1, std::memory_order_relaxed);
        maskPushCount_.fetch_add(1, std::memory_order_relaxed);
    } else {
        debugTap_.pushFails.fetch_add(1, std::memory_order_relaxed);
        maskDropCount_.fetch_add(1, std::memory_order_relaxed);
    }
#else
    // Simple path without debug tap
    const bool success = maskColumnQueue.push(mask);
    
    if (success) {
        maskPushCount_.fetch_add(1, std::memory_order_relaxed);
    } else {
        maskDropCount_.fetch_add(1, std::memory_order_relaxed);
    }
#endif
    
    return success;
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

bool SpectralCanvasProAudioProcessor::pushPaintEvent(float y, float intensity, uint32_t timestampMs) noexcept
{
    if (!spectralPaintProcessor)
        return false;
        
    // Generate timestamp if not provided
    if (timestampMs == 0)
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = now.time_since_epoch();
        timestampMs = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
    }
    
    // Forward to spectral paint processor
    return spectralPaintProcessor->pushPaintEvent(y, intensity, timestampMs);
}

// Plugin factory functions
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralCanvasProAudioProcessor();
}