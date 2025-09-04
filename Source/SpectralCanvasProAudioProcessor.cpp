#include "SpectralCanvasProAudioProcessor.h"
#include "SpectralCanvasProEditor.h"
#include "Core/DiagnosticLogger.h"
#include "Core/ColumnOps.h"
#include <chrono>

SpectralCanvasProAudioProcessor::SpectralCanvasProAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", Params::createParameterLayout())
{
    DBG("Processor ctor");
    // Set STFT latency immediately on construction for pluginval compatibility
    // This accounts for overlap-add reconstruction delay in STFT processing
    const int stftLatency = AtlasConfig::FFT_SIZE - AtlasConfig::HOP_SIZE; // 512-128 = 384
    updateReportedLatency(stftLatency);
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
    apvts.removeParameterListener(Params::ParameterIDs::useModernPaint, this);
    apvts.removeParameterListener(Params::ParameterIDs::mode, this);
    apvts.removeParameterListener(Params::ParameterIDs::blend, this);
    apvts.removeParameterListener(Params::ParameterIDs::respeed, this);
#endif
}

void SpectralCanvasProAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    DBG("prepareToPlay(sr=" << sampleRate << ", spb=" << samplesPerBlock << ")");
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    
    // set initial audio path from current params (message thread)
    setAudioPathFromParams();
    lastPath_ = currentPath_.load(std::memory_order_relaxed);
    
    // Initialize RT-safe sample loader
    sampleLoader.initialize(sampleRate);
    
    // Initialize RT-safe test feeder
    maskTestFeeder.initialize(sampleRate, AtlasConfig::NUM_BINS);
    
    // Initialize RT-safe spectral processing components
    spectralEngine = std::make_unique<SpectralEngine>();
    spectralEngine->initialize(sampleRate, samplesPerBlock);
    
    // Connect SampleLoader to SpectralEngine (RT-safe pointer sharing)
    spectralEngine->setSampleLoader(&sampleLoader);
    
    // Initialize modern JUCE DSP-based spectral painting processor
    spectralPaintProcessor = std::make_unique<SpectralPaintProcessor>();
    
    // Initialize tiled atlas system
    initializeTiledAtlas();
    
    // Initialize Phase 4 (HEAR): RT-safe STFT masking
    hop_.prepare(sampleRate);
    hop_.setQueue(&maskDeltaQueue);
    hop_.setAtlas(tiledAtlas_.get(), tiledAtlas_ ? tiledAtlas_->allocateFreePage() : AtlasPageHandle{});
    hop_.setAtlasUpdateQueue(&atlasUpdateQueue);
    rt_.prepare(sampleRate, AtlasConfig::FFT_SIZE, AtlasConfig::HOP_SIZE);
    
    // Set STFT latency based on single-source atlas configuration
    // This accounts for overlap-add reconstruction delay in STFT processing
    const int stftLatency = AtlasConfig::FFT_SIZE - AtlasConfig::HOP_SIZE;
    updateReportedLatency(stftLatency);
    jassert(getLatencySamples() == stftLatency);
    
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
    sampleQueue.clear();
    
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
    // Initialize dual-mode params
    mode_.store(static_cast<int>(apvts.getParameterAsValue(Params::ParameterIDs::mode).getValue()), std::memory_order_relaxed);
    blend_.store(apvts.getParameterAsValue(Params::ParameterIDs::blend).getValue(), std::memory_order_relaxed);
    respeed_.store(apvts.getParameterAsValue(Params::ParameterIDs::respeed).getValue(), std::memory_order_relaxed);
    
    // Add parameter listeners for real-time updates
    apvts.addParameterListener(Params::ParameterIDs::useTestFeeder, this);
    apvts.addParameterListener(Params::ParameterIDs::keyFilterEnabled, this);
    apvts.addParameterListener(Params::ParameterIDs::oscGain, this);
    apvts.addParameterListener(Params::ParameterIDs::scaleType, this);
    apvts.addParameterListener(Params::ParameterIDs::rootNote, this);
    apvts.addParameterListener(Params::ParameterIDs::useModernPaint, this);
    apvts.addParameterListener(Params::ParameterIDs::mode, this);
    apvts.addParameterListener(Params::ParameterIDs::blend, this);
    apvts.addParameterListener(Params::ParameterIDs::respeed, this);
#endif
    
    // Initialize RT-safe parameter smoothers (50ms ramp time for smooth automation)
    oscGainSmoother_.reset(sampleRate, smoothingTimeSec_);
    brushSizeSmoother_.reset(sampleRate, smoothingTimeSec_);
    brushStrengthSmoother_.reset(sampleRate, smoothingTimeSec_);
    
    // Set initial values from current APVTS parameters
    oscGainSmoother_.setCurrentAndTargetValue(getParamFast(Params::ParameterIDs::oscGain));
    brushSizeSmoother_.setCurrentAndTargetValue(getParamFast(Params::ParameterIDs::brushSize));
    brushStrengthSmoother_.setCurrentAndTargetValue(getParamFast(Params::ParameterIDs::brushStrength));
    
#ifdef PHASE4_EXPERIMENT
    // Pre-calculate timestamp for RT safety (Phase4 only)
    using namespace std::chrono;
    auto now = steady_clock::now();
    rtTimestampUs_.store(static_cast<uint64_t>(duration_cast<microseconds>(now.time_since_epoch()).count()), std::memory_order_relaxed);
#endif
    
    // Initialize spectral pipeline if model is ready
    if (spectralModel.isReady())
    {
        spectralPlayer.prepare (sampleRate, samplesPerBlock, &spectralModel, &spectralMask);
        spectralReady = true;
    }
    else
    {
        spectralReady = false;
    }

    // Preallocate hybrid buffer for mix mode (avoid RT allocations)
    const int numOutChans = std::max(getTotalNumInputChannels(), getTotalNumOutputChannels());
    hybridBuffer_.setSize(numOutChans, samplesPerBlock);
    
    // Initialize RT-safe latency delay line for non-STFT paths (FFT_SIZE - HOP_SIZE)
    const int latencyDelaySamples = AtlasConfig::FFT_SIZE - AtlasConfig::HOP_SIZE; // 384 samples
    latencyLine_.setSize(numOutChans, latencyDelaySamples);
    latencyLine_.clear();
    latencyWritePos_ = 0;
}

void SpectralCanvasProAudioProcessor::releaseResources()
{
    DBG("releaseResources()");
    if (spectralEngine)
        spectralEngine->reset();
        
    if (spectralPaintProcessor)
        spectralPaintProcessor->reset();
        
    sampleLoader.shutdown();
    maskTestFeeder.reset();
    
    // Shutdown tiled atlas system
    shutdownTiledAtlas();
    
    // Keep latency intact - host needs consistent latency reporting
    // Do not reset latency to 0 here (causes pluginval failures)
}

// Called from parameter listener / message thread whenever GUI toggles mode
void SpectralCanvasProAudioProcessor::setAudioPathFromParams()
{
#ifdef PHASE4_EXPERIMENT
    const int mode = mode_.load(std::memory_order_relaxed); // 0=Synth,1=Resynth,2=Hybrid
    const bool useTestFeeder = useTestFeeder_.load(std::memory_order_relaxed);
    const bool useModernPaint = useModernPaint_.load(std::memory_order_relaxed);

    AudioPath path = AudioPath::Phase4Synth;
    if (mode == 1) {
        // Resynth only (if ready)
        path = spectralReady ? AudioPath::SpectralResynthesis : AudioPath::Phase4Synth;
    } else {
        // Synth-based paths
        if (useTestFeeder) path = AudioPath::TestFeeder;
        else if (useModernPaint) path = AudioPath::ModernPaint;
        else path = AudioPath::Phase4Synth;
    }

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
    
    // Early exit for zero-length buffers to prevent crashes
    if (numSamples <= 0 || buffer.getNumChannels() <= 0) {
        return;
    }
    
    // RT-safe perf counters: no clocks on audio thread
    lastBlockSize_.store(numSamples, std::memory_order_relaxed);
    totalBlocksProcessed_.fetch_add(1, std::memory_order_relaxed);
    totalSamplesProcessed_.fetch_add(static_cast<uint64_t>(numSamples), std::memory_order_relaxed);
    
    // RT-safe XRun detection: check for unusual buffer conditions
    if (numSamples == 0 || (numSamples < 16 && currentBlockSize > 64)) {
        // Abnormally small buffer size may indicate XRun condition
        xrunCount_.fetch_add(1, std::memory_order_relaxed);
    }

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

    // Check if spectral pipeline should handle audio
    // Use double-checked locking to avoid race conditions during model access
    const bool spectralPathReady = spectralReady && spectralModel.isReady();
    if (spectralPathReady && spectralModel.isReady())
    {
        currentPath_.store(AudioPath::SpectralResynthesis, std::memory_order_relaxed);
        spectralPlayer.process(buffer);
        wroteAudioFlag_.store(true, std::memory_order_relaxed);
        return;
    }
    
    // Initialize write detection for this block
    wroteAudioFlag_.store(false, std::memory_order_relaxed);

#if PHASE4_DEBUG_TAP
    // Record queue address on audio thread for integrity checking
    debugTap_.queueAddrOnAudio.store(reinterpret_cast<uintptr_t>(&maskColumnQueue), std::memory_order_relaxed);
#endif

    // Update parameter smoothers with current target values (RT-safe)
    oscGainSmoother_.setTargetValue(oscGain_.load(std::memory_order_relaxed));
    brushSizeSmoother_.setTargetValue(brushSize_.load(std::memory_order_relaxed));
    brushStrengthSmoother_.setTargetValue(brushStrength_.load(std::memory_order_relaxed));
    
    // Use smoothed params on the audio thread
    const float oscGain = juce::jlimit(1.0e-6f, 1.0f, oscGainSmoother_.getNextValue());
    const float brushSize = brushSizeSmoother_.getNextValue();
    const float brushStrength = brushStrengthSmoother_.getNextValue();
    
    // These are prepared for future paint integration
    juce::ignoreUnused(brushSize, brushStrength);
    
    // Phase 4 (HEAR): RT-safe STFT masking takes priority
    if (buffer.getNumChannels() <= 0) {
        return; // Early exit for invalid channel count
    }
    
    // Mono processing (use channel 0 as input)
    auto* inMono = buffer.getReadPointer(0);
    
    // Zero all outputs first
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.clear(ch, 0, numSamples);
    
    // Drain mask deltas and apply to staging page
    hop_.drainAndApply(16);
    
    // Process block with current mask column
    rt_.processBlock(inMono, buffer.getWritePointer(0), numSamples,
                    hop_.activeMaskCol(hop_.currentColInTile()));
    
    // Copy mono output to all channels
    for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, numSamples);
    
    // Advance hop position
    hop_.advance(numSamples);
    
    wroteAudioFlag_.store(true, std::memory_order_relaxed);
    return;
    
    // Snapshot only the params needed for the chosen path (no APVTS lookups here)
    const bool keyFilterOn = keyFilterEnabled_.load(std::memory_order_relaxed);
    const int mode = mode_.load(std::memory_order_relaxed); // 0=Synth,1=Resynth,2=Hybrid
    const bool resReady = spectralReady && spectralModel.isReady();
    const float blend = juce::jlimit(0.0f, 1.0f, blend_.load(std::memory_order_relaxed));

    // Hybrid mode: mix resynthesis and Phase4 synth
    if (mode == 2 && resReady)
    {
        // 1) Render resynthesis into main buffer (double-check model is still ready)
        if (spectralModel.isReady()) {
            spectralPlayer.process(buffer);
        }

        // 2) Render Phase4 synth into hybridBuffer_
        hybridBuffer_.clear();

        // Drain sample queue for currentSample activation
        SampleMessage sampleMsg;
        while (sampleQueue.pop(sampleMsg))
        {
            if (sampleMsg.isValid())
            {
                auto& memSys = RealtimeMemorySystem::getInstance();
                auto view = memSys.samples().lookup(sampleMsg.handle);
                if (view)
                {
                    currentSample = *view;
                    currentSampleHandle = sampleMsg.handle;
                    samplePosition = 0;
                }
            }
        }

        // Use standard conversion path for mask processing
        const uint64_t currentSamplePos = totalSamplesProcessed_.load(std::memory_order_acquire);
        convertMaskColumnsToDeltas(currentSamplePos);
        if (keyFilterOn) {
            keyFilter.apply(spectralStub.getMagnitudesWritePtr(), AtlasConfig::NUM_BINS);
        } else {
            keyFilter.enabled.store(false, std::memory_order_relaxed);
        }

        // Optional sample mix into hybrid buffer
        if (currentSample && currentSample->channels != nullptr && currentSample->numFrames > 0)
        {
            const uint32_t frames = currentSample->numFrames;
            const uint32_t chans  = currentSample->numChannels;
            const float sGain = 0.15f;
            for (int n = 0; n < numSamples; ++n)
            {
                float s = 0.0f;
                for (uint32_t ch = 0; ch < chans; ++ch)
                {
                    const float* src = currentSample->channels[ch];
                    const uint64_t idx = (samplePosition + (uint64_t) n) % frames;
                    s += src[idx];
                }
                s = (chans > 0 ? s / (float) chans : s) * sGain;
                for (int outCh = 0; outCh < hybridBuffer_.getNumChannels(); ++outCh)
                    hybridBuffer_.getWritePointer(outCh)[n] += s;
            }
            samplePosition = (samplePosition + (uint64_t) numSamples) % (uint64_t) frames;
        }

        // Render spectral stub into hybrid buffer
        spectralStub.process(hybridBuffer_, oscGain);

        // 3) Mix: blend means 0=synth only, 1=resynth only
        const float synthGain = 1.0f - blend;
        if (synthGain > 0.0f)
        {
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            {
                const float* src = hybridBuffer_.getReadPointer(ch);
                float* dst = buffer.getWritePointer(ch);
                for (int n = 0; n < numSamples; ++n)
                {
                    dst[n] = dst[n] * blend + src[n] * synthGain;
                }
            }
        }

        // Mark wrote if non-silent
        const float rms = buffer.getNumChannels() > 0 ? buffer.getRMSLevel(0, 0, numSamples) : 0.0f;
        if (rms > 1.0e-6f) wroteAudioFlag_.store(true, std::memory_order_relaxed);
        return;
    }

    switch (path)
    {
        case AudioPath::Silent:
            // Convert any pending mask columns to deltas but don't apply them
            // This prevents queue buildup without processing through spectralStub
            {
                const uint64_t currentSamplePos = totalSamplesProcessed_.load(std::memory_order_acquire);
                convertMaskColumnsToDeltas(currentSamplePos);
            }
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
            applyLatencyDelayIfNeeded(buffer); // align with reported latency
            return;
        }

        case AudioPath::Phase4Synth:
        {
#ifdef PHASE4_EXPERIMENT
            // Increment diagnostic counter to prove this branch executes
            phase4Blocks_.fetch_add(1, std::memory_order_relaxed);
            
            // 1) Drain sample queue (RT-safe, keep last)
            SampleMessage sampleMsg;
            while (sampleQueue.pop(sampleMsg))
            {
                if (sampleMsg.isValid())
                {
                    auto& memSys = RealtimeMemorySystem::getInstance();
                    auto view = memSys.samples().lookup(sampleMsg.handle);
                    if (view)
                    {
                        currentSample = *view;
                        currentSampleHandle = sampleMsg.handle;
                        samplePosition = 0; // Reset playback
                    }
                }
            }
            
            // 2) Use standard conversion path instead of direct spectralStub drain
            {
                const uint64_t currentSamplePos = totalSamplesProcessed_.load(std::memory_order_acquire);
                convertMaskColumnsToDeltas(currentSamplePos);
            }
            
            // 2) Gate (only apply when explicitly enabled)
            if (keyFilterOn) {
                keyFilter.apply(spectralStub.getMagnitudesWritePtr(), AtlasConfig::NUM_BINS);
            } else {
                // Ensure KeyFilter is disabled when not wanted
                keyFilter.enabled.store(false, std::memory_order_relaxed);
            }
            
            // 3) Render
            // Optional: simple sample playback if a sample is active
            if (currentSample && currentSample->channels != nullptr && currentSample->numFrames > 0)
            {
                const uint32_t frames = currentSample->numFrames;
                const uint32_t chans  = currentSample->numChannels;
                const float gain = 0.15f;
                for (int n = 0; n < numSamples; ++n)
                {
                    // Mix down source channels
                    float s = 0.0f;
                    for (uint32_t ch = 0; ch < chans; ++ch)
                    {
                        const float* src = currentSample->channels[ch];
                        const uint64_t idx = (samplePosition + (uint64_t) n) % frames;
                        s += src[idx];
                    }
                    s = (chans > 0 ? s / (float) chans : s) * gain;
                    for (int outCh = 0; outCh < buffer.getNumChannels(); ++outCh)
                        buffer.getWritePointer(outCh)[n] += s;
                }
                samplePosition = (samplePosition + (uint64_t) numSamples) % (uint64_t) frames;
            }

            // Continue Phase4 synthesis
            spectralStub.process(buffer, oscGain);
            
            // Mark "wrote" only if buffer has anything > ~ -120 dBFS
            const float rms = buffer.getNumChannels() > 0
                ? buffer.getRMSLevel(0, 0, numSamples) : 0.0f;
            if (rms > 1.0e-6f)
                wroteAudioFlag_.store(true, std::memory_order_relaxed);
#endif
            applyLatencyDelayIfNeeded(buffer); // align with reported latency
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
                if (rms > 1.0e-6f) {
                    wroteAudioFlag_.store(true, std::memory_order_relaxed);
                }
            }
            applyLatencyDelayIfNeeded(buffer); // align with reported latency
            return;
        }
    }
    
    // Phase 4: Apply pending mask deltas (bounded) and advance hop
    hop_.drainAndApply(16);
    hop_.advance((uint32_t) numSamples);
    // deltaDrains available via getDeltaDrainsPerBlock() method

    // Process tiled atlas messages (RT-safe)
    processTiledAtlasMessages();
    
#ifdef PHASE4_EXPERIMENT
    // Process KeyFilter rebuild requests (RT-safe)
    KeyFilterRebuildRequest rebuildRequest;
    while (keyFilterRebuildQueue.pop(rebuildRequest)) {
        keyFilter.rebuildAsync(rebuildRequest.rootNote, rebuildRequest.scale);
    }
#endif
    
#if JUCE_DEBUG
    // RT-SAFE: Silent audio detection using atomic counters only
    float rmsLevel = buffer.getRMSLevel(0, 0, numSamples);
    if (rmsLevel < 1e-6f) {
        silenceBlockCount_.fetch_add(1, std::memory_order_relaxed);
    } else {
        silenceBlockCount_.store(0, std::memory_order_relaxed);
    }
#endif

    // Publish snapshot for thread-safe GUI access (call at end of audio processing)
    publishCanvasSnapshot();
}

// RT-safe fallback beep generator (220Hz A3 tone)
void SpectralCanvasProAudioProcessor::fallbackBeep(juce::AudioBuffer<float>& buffer) noexcept
{
    static float phase = 0.0f;
    const float phaseIncrement = 220.0f * juce::MathConstants<float>::twoPi / static_cast<float>(getSampleRate());
    const float amplitude = 0.05f; // Balanced volume
    const int numSamples = buffer.getNumSamples();
    
    for (int n = 0; n < numSamples; ++n) {
        const float sample = std::sin(phase) * amplitude;
        
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.getWritePointer(ch)[n] += sample;
            
        phase += phaseIncrement;
        if (phase >= juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;
    }
}

// Compatibility wrapper for legacy interface
void SpectralCanvasProAudioProcessor::generateFallbackBeep(juce::AudioBuffer<float>& buffer, int numSamples) noexcept
{
    juce::ignoreUnused(numSamples); // buffer.getNumSamples() is authoritative
    fallbackBeep(buffer);
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
        // Smoother target will be set in processBlock - RT-safe
    }
    else if (parameterID == Params::ParameterIDs::brushSize)
    {
        brushSize_.store(newValue, std::memory_order_relaxed);
        // Smoother target will be set in processBlock - RT-safe
    }
    else if (parameterID == Params::ParameterIDs::brushStrength)
    {
        brushStrength_.store(newValue, std::memory_order_relaxed);
        // Smoother target will be set in processBlock - RT-safe
    }
    else if (parameterID == Params::ParameterIDs::scaleType)
    {
        const int scaleType = static_cast<int>(newValue);
        scaleType_.store(scaleType, std::memory_order_relaxed);
        
        // Queue rebuild request for audio thread processing
        const int root = rootNote_.load(std::memory_order_relaxed);
        KeyFilterRebuildRequest request{root, static_cast<dsp::ScaleType>(scaleType)};
        keyFilterRebuildQueue.push(request); // Non-blocking
    }
    else if (parameterID == Params::ParameterIDs::rootNote)
    {
        const int rootNote = static_cast<int>(newValue);
        rootNote_.store(rootNote, std::memory_order_relaxed);
        
        // Queue rebuild request for audio thread processing
        const int scaleType = scaleType_.load(std::memory_order_relaxed);
        KeyFilterRebuildRequest request{rootNote, static_cast<dsp::ScaleType>(scaleType)};
        keyFilterRebuildQueue.push(request); // Non-blocking
    }
    else if (parameterID == Params::ParameterIDs::mode)
    {
        mode_.store(static_cast<int>(newValue), std::memory_order_relaxed);
        setAudioPathFromParams();
    }
    else if (parameterID == Params::ParameterIDs::blend)
    {
        blend_.store(newValue, std::memory_order_relaxed);
    }
    else if (parameterID == Params::ParameterIDs::respeed)
    {
        respeed_.store(newValue, std::memory_order_relaxed);
        // Update spectral player respeed if spectral pipeline is ready
        if (spectralReady) {
            spectralPlayer.setRespeed(newValue);
        }
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
    // Load using new spectral pipeline
    if (!sampleManager.loadFromFile(audioFile, getSampleRate())) return false;
    spectralModel.build(sampleManager.get().audio, /*fftOrder*/ 10, /*hop*/ 256);
    spectralMask.init(spectralModel.numFrames(), spectralModel.numBins());
    if (currentSampleRate > 0 && currentBlockSize > 0) {
        spectralPlayer.prepare(getSampleRate(), getBlockSize(), &spectralModel, &spectralMask);
        spectralPlayer.reset();
    }
    spectralReady = spectralModel.isReady();
    
    // NEW: Also load into RT-safe handle system for direct playback
    auto& memSys = RealtimeMemorySystem::getInstance();
    auto handle = memSys.samples().allocate();
    if (handle)
    {
        const auto& sampleData = sampleManager.get();
        if (sampleData.loaded && sampleData.audio.getNumSamples() > 0)
        {
            // Keep audio data alive on UI thread (member variable)
            static std::vector<std::unique_ptr<float[]>> ownedChannels;
            static std::vector<const float*> channelPtrs;
            
            ownedChannels.clear();
            channelPtrs.clear();
            
            // Copy to UI-owned memory
            const int numChannels = sampleData.audio.getNumChannels();
            const int numSamples = sampleData.audio.getNumSamples();
            
            for (int ch = 0; ch < numChannels; ++ch)
            {
                auto channelData = std::make_unique<float[]>(numSamples);
                std::copy(sampleData.audio.getReadPointer(ch),
                         sampleData.audio.getReadPointer(ch) + numSamples,
                         channelData.get());
                channelPtrs.push_back(channelData.get());
                ownedChannels.push_back(std::move(channelData));
            }
            
            SampleView view(channelPtrs.data(), numChannels, numSamples, sampleData.sampleRate);
            
            if (memSys.samples().publish(*handle, view))
            {
                SampleMessage msg;
                msg.handle = *handle;
                msg.tscOrUsec = juce::Time::getMillisecondCounter() * 1000;
                sampleQueue.push(msg);
            }
            else
            {
                memSys.samples().free(*handle);
            }
        }
        else
        {
            memSys.samples().free(*handle);
        }
    }
    
    return spectralReady;
}

bool SpectralCanvasProAudioProcessor::hasActiveRenderer() const noexcept
{
    return hasActiveRenderer_.load(std::memory_order_acquire);
}

bool SpectralCanvasProAudioProcessor::activateSampleRenderer(const juce::AudioBuffer<float>& sampleData, double sourceSampleRate)
{
    juce::Logger::writeToLog("RENDERER: Activating sample renderer: " + 
                              juce::String(sampleData.getNumChannels()) + " channels, " +
                              juce::String(sampleData.getNumSamples()) + " samples, " +
                              juce::String(sourceSampleRate / 1000.0, 1) + "kHz");
    
    // Ensure we have valid sample data
    if (sampleData.getNumSamples() <= 0 || sampleData.getNumChannels() <= 0)
    {
        juce::Logger::writeToLog("RENDERER ERROR: Cannot activate renderer with invalid sample data");
        return false;
    }
    
    // Build spectral model from sample
    spectralModel.build(sampleData, /*fftOrder*/ 10, /*hop*/ 256);
    if (!spectralModel.isReady())
    {
        juce::Logger::writeToLog("RENDERER ERROR: Failed to build spectral model from sample");
        return false;
    }
    
    // Initialize spectral mask
    spectralMask.init(spectralModel.numFrames(), spectralModel.numBins());
    
    // Prepare spectral player if audio system is ready
    if (currentSampleRate > 0 && currentBlockSize > 0)
    {
        spectralPlayer.prepare(getSampleRate(), getBlockSize(), &spectralModel, &spectralMask);
        spectralPlayer.reset();
        spectralReady = true;
        
        juce::Logger::writeToLog("RENDERER: Spectral player prepared: " +
                                  juce::String(spectralModel.numFrames()) + " frames, " +
                                  juce::String(spectralModel.numBins()) + " bins");
    }
    
    // Mark renderer as active
    hasActiveRenderer_.store(true, std::memory_order_release);

    juce::Logger::writeToLog("RENDERER: Renderer activation successful");

    // Also publish this sample via handle-based RT system for Phase4 path
    {
        auto& memSys = RealtimeMemorySystem::getInstance();
        auto handle = memSys.samples().allocate();
        if (handle)
        {
            // Keep audio data alive on UI/message thread by copying into owned buffers
            static std::vector<std::unique_ptr<float[]>> ownedChannels;
            static std::vector<const float*> channelPtrs;

            ownedChannels.clear();
            channelPtrs.clear();

            const int numChannels = sampleData.getNumChannels();
            const int numSamples  = sampleData.getNumSamples();
            for (int ch = 0; ch < numChannels; ++ch)
            {
                auto channelData = std::make_unique<float[]>(numSamples);
                std::copy(sampleData.getReadPointer(ch),
                          sampleData.getReadPointer(ch) + numSamples,
                          channelData.get());
                channelPtrs.push_back(channelData.get());
                ownedChannels.push_back(std::move(channelData));
            }

            SampleView view(channelPtrs.data(), (uint32_t) numChannels, (uint32_t) numSamples, sourceSampleRate);
            if (memSys.samples().publish(*handle, view))
            {
                SampleMessage msg;
                msg.handle = *handle;
                msg.tscOrUsec = juce::Time::getMillisecondCounter() * 1000ULL;
                sampleQueue.push(msg);
                juce::Logger::writeToLog("RENDERER: Published sample handle to RT system");
            }
            else
            {
                memSys.samples().free(*handle);
            }
        }
    }
    return true;
}

void SpectralCanvasProAudioProcessor::deactivateRenderer()
{
    juce::Logger::writeToLog("RENDERER: Deactivating renderer");
    
    hasActiveRenderer_.store(false, std::memory_order_release);
    spectralReady = false;
    
    // Reset spectral components
    spectralPlayer.reset();
    spectralModel.clear();
    
    // Reset spectral mask by reinitializing with zero size
    spectralMask.init(0, 0);
    
    juce::Logger::writeToLog("RENDERER: Renderer deactivated");
}

void SpectralCanvasProAudioProcessor::generateImmediateAudioFeedback()
{
    // Start diagonal sweep test pattern for paint-to-audio validation
    maskTestFeeder.startDiagonalSweep();
}

bool SpectralCanvasProAudioProcessor::pushMaskColumn(const MaskColumn& mask)
{
    // RT-SAFE: Track push attempts with atomic counter
    pushMaskAttempts_.fetch_add(1, std::memory_order_relaxed);
    
    // Only push when Phase4 is active - double-check path state
    if (getCurrentPath() != AudioPath::Phase4Synth) {
        // RT-SAFE: Count rejections for HUD display
        pushMaskRejects_.fetch_add(1, std::memory_order_relaxed);
        return false; // Silently ignore when not in Phase4 mode
    }
    
    
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
    // Enforce NUM_BINS-sized copy at compile time
    atlas::copyColumn(colEx.values, mask.values);
    
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
    
    // XRun detection from RT-safe buffer monitoring
    metrics.xrunCount = xrunCount_.load(std::memory_order_relaxed);
    
    return metrics;
}

void SpectralCanvasProAudioProcessor::updateReportedLatency(int samples) noexcept
{
    juce::ignoreUnused(samples);
    // Ensure latency matches STFT configuration (FFT_SIZE - HOP_SIZE)
    const int stftLatency = AtlasConfig::FFT_SIZE - AtlasConfig::HOP_SIZE; // 384 samples
    AudioProcessor::setLatencySamples(stftLatency);
    latencySamples_.store(stftLatency, std::memory_order_release);
}

std::shared_ptr<const SpectralCanvasProAudioProcessor::CanvasSnapshot> SpectralCanvasProAudioProcessor::getCanvasSnapshot() const
{
    return std::atomic_load_explicit(&canvasSnapshot_, std::memory_order_acquire);
}

void SpectralCanvasProAudioProcessor::publishCanvasSnapshot() const
{
    auto snapshot = std::make_shared<CanvasSnapshot>();
    
    // Populate with current processor state
    snapshot->timestampMs = juce::Time::getMillisecondCounterHiRes();
    snapshot->metrics = getPerformanceMetrics();
    snapshot->currentPath = getCurrentPath();
    snapshot->wroteAudioFlag = getWroteAudioFlag();
    snapshot->sampleRate = getSampleRate();
    snapshot->blockSize = getBlockSize();
    
    #ifdef PHASE4_EXPERIMENT
    snapshot->activeBins = getActiveBinCount();
    snapshot->totalBins = getNumBins();
    snapshot->maskPushCount = getMaskPushCount();
    snapshot->maskDropCount = getMaskDropCount();
    snapshot->maxMagnitude = getMaxMagnitude();
    snapshot->phase4Blocks = getPhase4Blocks();
    #endif
    
    // Atomically publish the snapshot
    std::atomic_store_explicit(&canvasSnapshot_, snapshot, std::memory_order_release);
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

// Tiled atlas system implementation methods

void SpectralCanvasProAudioProcessor::initializeTiledAtlas() noexcept {
    // Initialize tiled atlas
    tiledAtlas_ = std::make_shared<TiledAtlas>();
    if (!tiledAtlas_->initialize()) {
        tiledAtlas_.reset();
        return;
    }
    
    // Initialize offline analyzer
    offlineAnalyzer_ = std::make_unique<OfflineStftAnalyzer>();
    if (!offlineAnalyzer_->initialize(currentSampleRate, maskDeltaQueue)) {
        offlineAnalyzer_.reset();
        return;
    }
    
    // Phase 4 hop scheduler is initialized in prepareToPlay()
}

void SpectralCanvasProAudioProcessor::shutdownTiledAtlas() noexcept {
    if (offlineAnalyzer_) {
        offlineAnalyzer_->shutdown();
        offlineAnalyzer_.reset();
    }
    
    // Phase 4 hop scheduler is managed directly as hop_ member
    
    if (tiledAtlas_) {
        tiledAtlas_->shutdown();
        tiledAtlas_.reset();
    }
}

void SpectralCanvasProAudioProcessor::processTiledAtlasMessages() noexcept {
    // Phase 4: Using hop_ member instead of hopScheduler_
    
    // Get current audio position
    const uint64_t currentSamplePos = totalSamplesProcessed_.load(std::memory_order_acquire);
    
    // Convert MaskColumn → MaskColumnDelta and feed to HopScheduler
    convertMaskColumnsToDeltas(currentSamplePos);
    
    // Phase 4 mask delta processing is handled directly in processBlock()
    
    // Process any atlas page management messages
    AtlasPageMessage pageMsg;
    while (pageManagementQueue.pop(pageMsg)) {
        if (!tiledAtlas_) continue;
        
        switch (pageMsg.type) {
            case AtlasPageMessage::Activate:
                if (pageMsg.handle.isValid()) {
                    tiledAtlas_->activatePage(pageMsg.handle);
                }
                break;
                
            case AtlasPageMessage::Release:
                if (pageMsg.handle.isValid()) {
                    tiledAtlas_->releasePage(pageMsg.handle);
                }
                break;
                
            default:
                break; // Other message types handled by background thread
        }
    }
}

void SpectralCanvasProAudioProcessor::convertMaskColumnsToDeltas(uint64_t currentSamplePos) noexcept {
    MaskColumn maskCol;
    static constexpr size_t MAX_CONVERSIONS_PER_BLOCK = 16; // RT-safety: bounded processing
    size_t conversionsThisBlock = 0;
    
    // Drain painting system output with bounded processing
    while (maskColumnQueue.pop(maskCol) && 
           maskDeltaQueue.hasSpaceAvailable() && 
           conversionsThisBlock < MAX_CONVERSIONS_PER_BLOCK) {
        
        // CRITICAL: Runtime bounds checking to prevent heap corruption
        if (maskCol.numBins != AtlasConfig::NUM_BINS) {
            badBinSkips_.fetch_add(1, std::memory_order_relaxed);
            jassertfalse; // Debug: catch now rather than crash later
            continue;     // Drop the malformed delta safely
        }
        
        // Create MaskColumnDelta
        MaskColumnDelta delta;
        
        // Map UI column to atlas position
        // FINAL SPEC: column = floor((playheadSamples - startOffsetSamples) / HOP)
        // For now, use maskCol.x directly as column index
        const uint32_t column = static_cast<uint32_t>(maskCol.x);
        const uint32_t tileId = column / AtlasConfig::TILE_WIDTH;
        const uint32_t colInTile = column % AtlasConfig::TILE_WIDTH;
        
        // CRITICAL: Additional bounds checking for column position  
        if (colInTile >= AtlasConfig::TILE_WIDTH) {
            badColSkips_.fetch_add(1, std::memory_order_relaxed);
            continue; // Skip out-of-bounds column to prevent corruption
        }
        
        // Set atlas position
        delta.position.tileX = static_cast<uint16_t>(tileId);
        delta.position.tileY = 0; // Single row for now
        delta.position.columnInTile = static_cast<uint16_t>(colInTile);
        delta.position.binStart = static_cast<uint16_t>(maskCol.binStart);
        
        // Copy mask values with runtime size validation
        static_assert(sizeof(delta.values) == sizeof(maskCol.values), 
                     "MaskColumn and MaskColumnDelta must have same values array size");
        
        for (size_t i = 0; i < AtlasConfig::NUM_BINS; ++i) {
            const float scaledValue = maskCol.values[i] * maskCol.intensity;
            delta.values[i] = scaledValue;
            
            // Debug-only finite value check to catch NaN/infinity corruption
            #if JUCE_DEBUG
            jassertquiet(std::isfinite(scaledValue));
            #endif
        }
        
        // Set timing metadata
        delta.metadata.samplePosition = maskCol.timestampSamples > 0 ? 
                                       maskCol.timestampSamples : currentSamplePos;
        delta.metadata.timeSeconds = static_cast<float>(delta.metadata.samplePosition / currentSampleRate);
        delta.metadata.sequenceNumber = maskCol.sequenceNumber;
        delta.metadata.fundamentalHz = 0.0f; // Will be filled by analysis later
        delta.metadata.spectralCentroid = 0.0f; // Will be filled by analysis later
        
        // Push to HopScheduler input queue (RT-safe, non-blocking)
        if (maskDeltaQueue.push(delta)) {
            conversionsThisBlock++;
        } else {
            // Queue full - could increment drop counter
            break;
        }
    }
    
    // Track total delta conversions for diagnostics
}

// RT-safe latency delay application for non-STFT paths
void SpectralCanvasProAudioProcessor::applyLatencyDelayIfNeeded(juce::AudioBuffer<float>& buffer) noexcept
{
    // Only apply delay if we have a configured latency line
    if (latencyLine_.getNumSamples() == 0 || latencyLine_.getNumChannels() == 0) {
        return; // No delay line configured
    }
    
    const int numSamples = buffer.getNumSamples();
    const int numChannels = juce::jmin(buffer.getNumChannels(), latencyLine_.getNumChannels());
    const int delaySize = latencyLine_.getNumSamples(); // Should be 384 samples
    
    // Process each channel independently
    for (int ch = 0; ch < numChannels; ++ch) {
        const float* input = buffer.getReadPointer(ch);
        float* output = buffer.getWritePointer(ch);
        float* delayBuffer = latencyLine_.getWritePointer(ch);
        
        // Apply circular buffer delay (RT-safe, no allocations)
        int writePos = latencyWritePos_;
        for (int n = 0; n < numSamples; ++n) {
            // Read delayed sample from circular buffer
            const float delayedSample = delayBuffer[writePos];
            
            // Write current input to delay buffer
            delayBuffer[writePos] = input[n];
            
            // Output the delayed sample
            output[n] = std::isfinite(delayedSample) ? delayedSample : 0.0f;
            
            // Advance write position (circular)
            writePos = (writePos + 1) % delaySize;
        }
    }
    
    // Update shared write position after processing all channels
    latencyWritePos_ = (latencyWritePos_ + numSamples) % delaySize;
    
    // Clear any extra channels that don't have delay processing
    for (int ch = numChannels; ch < buffer.getNumChannels(); ++ch) {
        buffer.clear(ch, 0, numSamples);
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralCanvasProAudioProcessor();
}
