// New processBlock implementation demonstrating all 7 architectural patterns
// This file shows the target implementation - to be integrated into AudioProcessor

void SpectralCanvasProAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, 
                                                   juce::MidiBuffer& midiMessages) noexcept
{
    juce::ignoreUnused(midiMessages);
    
    // Pattern #7: Continuous Verification - RT-safety guards
    juce::ScopedNoDenormals noDenormals;
    juce::AudioProcessLoadMeasurer::ScopedTimer loadTimer(loadMeasurer_);
    
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    
    // Pattern #5: Timebase Everywhere - read playhead ONCE at block start
    updateTimebase();
    
    // Pattern #2: Single Source of Truth - build parameter snapshot from atomics ONLY
    // (Never call APVTS methods on audio thread - they use locks!)
    buildParameterSnapshot();
    
    // Pattern #2: Apply smoothing targets before processing
    parameterSmoother.setTargets(currentSnapshot);
    
    // Pattern #4: Apply any prepared reconfigurations at safe gate
    reconfigManager.applyReconfig();
    
    // Clear output buffer once
    buffer.clear();
    
    // Pattern #3: Process all pending commands with timestamps
    processQueuedCommands();
    
    // Pattern #1: Route to pure DSP engine (no JUCE types)
    if (coreEngine && coreEngine->isInitialized())
    {
        // Extract raw float pointers for engine (thin wrapper pattern)
        const float* const* inputPtrs = buffer.getArrayOfReadPointers();
        float* const* outputPtrs = buffer.getArrayOfWritePointers();
        
        // Core engine processes with parameter snapshot (pure DSP)
        coreEngine->process(inputPtrs, outputPtrs, numChannels, numSamples, currentSnapshot);
        
        // Extract telemetry for UI/GPU threads
        publishTelemetry(coreEngine->getStateSnapshot());
    }
    else
    {
        // Fallback path using legacy engines for compatibility
        processLegacyPath(buffer, numSamples);
    }
    
    // Pattern #5: Update timeline counters
    const uint64_t currentFrameId = engineFrameId_.fetch_add(1, std::memory_order_relaxed);
    totalProcessedSamples_.fetch_add(numSamples, std::memory_order_relaxed);
    blockStartSample_ += numSamples;
    
    // Pattern #2: Skip unused smoothed values for efficiency
    parameterSmoother.skip(numSamples);
}

// Pattern #5: Timebase Everywhere - sample-accurate timing
void SpectralCanvasProAudioProcessor::updateTimebase() noexcept
{
    // Read playhead position ONLY here (JUCE playhead is only valid in processBlock)
    auto playhead = getPlayHead();
    if (playhead != nullptr)
    {
        juce::AudioPlayHead::CurrentPositionInfo positionInfo;
        if (playhead->getCurrentPosition(positionInfo))
        {
            hostPlaying_ = positionInfo.isPlaying;
            hostBPM_ = positionInfo.bpm > 0 ? positionInfo.bpm : 120.0;
            hostTimeSeconds_ = positionInfo.timeInSeconds;
        }
    }
    
    // Calculate sample-accurate block start
    const uint64_t totalSamples = totalProcessedSamples_.load(std::memory_order_relaxed);
    blockStartSample_ = totalSamples;
}

// Pattern #2: Build POD parameter snapshot from atomic reads ONLY
void SpectralCanvasProAudioProcessor::buildParameterSnapshot() noexcept
{
    // CRITICAL: Never call APVTS methods here - they use locks!
    // Use getRawParameterValue() which returns atomics
    
    // Read all parameters atomically
    auto* masterGainParam = apvts.getRawParameterValue(Params::ParameterIDs::gain);
    auto* spectralGainParam = apvts.getRawParameterValue(Params::ParameterIDs::spectralGain);
    auto* spectralMixParam = apvts.getRawParameterValue(Params::ParameterIDs::spectralMix);
    auto* blendParam = apvts.getRawParameterValue(Params::ParameterIDs::blend);
    auto* maskDepthParam = apvts.getRawParameterValue(Params::ParameterIDs::maskDepth);
    auto* fftSizeParam = apvts.getRawParameterValue(Params::ParameterIDs::fftSize);
    auto* modeParam = apvts.getRawParameterValue(Params::ParameterIDs::mode);
    
    // Build snapshot with default fallbacks
    currentSnapshot.masterGain = masterGainParam ? masterGainParam->load() : 0.0f;
    currentSnapshot.spectralGain = spectralGainParam ? spectralGainParam->load() : 1.0f;
    currentSnapshot.spectralMix = spectralMixParam ? spectralMixParam->load() : 1.0f;
    currentSnapshot.blend = blendParam ? blendParam->load() : 0.0f;
    currentSnapshot.maskDepth = maskDepthParam ? maskDepthParam->load() : 0.7f;
    currentSnapshot.fftSize = fftSizeParam ? static_cast<uint16_t>(fftSizeParam->load()) : 512;
    currentSnapshot.synthMode = modeParam ? static_cast<uint8_t>(modeParam->load()) : 0;
    
    // Set timing information
    currentSnapshot.blockStartSample = blockStartSample_;
    currentSnapshot.engineFrameId = engineFrameId_.load(std::memory_order_relaxed);
    
    // Audio path selection
    if (useFullSpectralEngine_.load(std::memory_order_relaxed))
        currentSnapshot.audioPath = static_cast<uint8_t>(AudioPath::SpectralEngine);
    else if (useTestFeeder_.load(std::memory_order_relaxed))
        currentSnapshot.audioPath = static_cast<uint8_t>(AudioPath::TestFeeder);
    else
        currentSnapshot.audioPath = static_cast<uint8_t>(AudioPath::Phase4Synth);
}

// Pattern #3: Process timestamped commands from UI/GPU threads
void SpectralCanvasProAudioProcessor::processQueuedCommands() noexcept
{
    // Process parameter updates with sample-accurate timing
    ParameterUpdate paramUpdate;
    while (parameterQueue.pop(paramUpdate))
    {
        const auto& update = paramUpdate;
        
        // Validate command version for compatibility
        if (update.version != 1) continue;
        
        // Apply parameter at specified sample offset within block
        // (This would require more sophisticated per-sample processing)
        // For now, apply immediately - sample accuracy can be added later
        applyParameterUpdate(update);
    }
    
    // Process mask columns from paint system
    MaskColumn maskUpdate;
    while (maskColumnQueue.pop(maskUpdate))
    {
        const auto& mask = maskUpdate;
        
        // Validate and apply mask with engine frame timing
        if (mask.version == 1 && coreEngine)
        {
            coreEngine->applyMaskColumn(mask.values, mask.numBins, mask.engineFrameId);
        }
    }
}

// Pattern #4: VST3 setProcessing handler for safe reconfiguration gates
void SpectralCanvasProAudioProcessor::setProcessing(bool isNowPlaying) 
{
    // This can be called from the audio thread in VST3!
    // It's a safe reconfiguration gate
    reconfigManager.handleSetProcessing(isNowPlaying);
    
    if (!isNowPlaying)
    {
        // Processing stopped - reset all engines to clean state
        if (coreEngine) coreEngine->reset();
        if (spectralEngine) spectralEngine->reset();
        
        // Clear all queues
        spectralDataQueue.clear();
        parameterQueue.clear();
        maskColumnQueue.clear();
    }
    
    // Call base class implementation
    AudioProcessor::setProcessing(isNowPlaying);
}

// Pattern #4: prepareToPlay integration with reconfiguration
void SpectralCanvasProAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    
    // Pattern #4: Handle reconfiguration at safe gate
    reconfigManager.handlePrepareToPlay(sampleRate, samplesPerBlock);
    
    // Pattern #2: Prepare parameter smoothing
    parameterSmoother.prepare(sampleRate, samplesPerBlock);
    
    // Initialize core engine if not already done
    if (!coreEngine)
    {
        coreEngine = std::make_unique<SpectralCoreEngine>();
        
        ReconfigManager::EngineConfig config;
        config.sampleRate = sampleRate;
        config.maxBlockSize = samplesPerBlock;
        config.fftSize = ReconfigManager::FFTSize::Size512;
        config.hopSize = 128;
        config.numChannels = getTotalNumOutputChannels();
        
        coreEngine->prepare(config);
    }
    
    // Set latency based on current configuration
    setLatencySamples(reconfigManager.getCurrentLatencySamples());
    
    // Initialize legacy engines for compatibility
    if (!spectralEngine)
    {
        spectralEngine = std::make_unique<SpectralEngine>();
        spectralEngine->initialize(sampleRate, samplesPerBlock);
    }
    
#ifdef PHASE4_EXPERIMENT
    spectralStub.prepare(sampleRate, 512, 257, getTotalNumOutputChannels());
#endif
}

// Helper: Apply parameter update (could be sample-accurate)
void SpectralCanvasProAudioProcessor::applyParameterUpdate(const ParameterUpdate& update) noexcept
{
    // For now, apply immediately
    // Sample-accurate application would require per-sample processing loop
    switch (update.parameterId)
    {
        // This would map parameter IDs to direct atomic updates
        // Implementation depends on specific parameter mapping
        default:
            break;
    }
}

// Helper: Legacy audio path for compatibility
void SpectralCanvasProAudioProcessor::processLegacyPath(juce::AudioBuffer<float>& buffer, int numSamples) noexcept
{
    // Use existing audio path logic as fallback
    const auto path = currentPath_.load(std::memory_order_acquire);
    
    switch (path)
    {
        case AudioPath::Silent:
            // Already cleared
            break;
            
        case AudioPath::TestFeeder:
            // Generate test tone
            generateTestTone(buffer, numSamples);
            break;
            
#ifdef PHASE4_EXPERIMENT            
        case AudioPath::Phase4Synth:
            // Use experimental stub
            spectralStub.popAllMaskColumnsInto(maskColumnQueue);
            spectralStub.process(buffer, oscGain_.load(std::memory_order_relaxed));
            break;
#endif
            
        case AudioPath::SpectralEngine:
            // Use full spectral engine
            if (spectralEngine && spectralEngine->isInitialized())
            {
                spectralEngine->processBlock(
                    buffer.getReadPointer(0),
                    buffer.getWritePointer(0),
                    numSamples
                );
            }
            break;
    }
    
    wroteAudioFlag_.store(true, std::memory_order_relaxed);
}

// Helper: Publish telemetry to UI/GPU threads
void SpectralCanvasProAudioProcessor::publishTelemetry(const SpectralCoreEngine::EngineStateSnapshot& snapshot) noexcept
{
    // Create spectral frame for UI
    SpectralFrame frame;
    frame.sequenceNumber = static_cast<uint32_t>(snapshot.frameId);
    frame.version = 1;
    frame.engineFrameId = snapshot.frameId;
    frame.timestampSamples = static_cast<double>(blockStartSample_);
    frame.spectralCentroid = snapshot.spectralCentroid;
    
    // Extract spectral data from engine
    if (coreEngine)
    {
        coreEngine->extractSpectralData(frame.magnitude, frame.phase, SpectralFrame::NUM_BINS);
    }
    
    // Push to UI thread (drop-oldest policy)
    spectralDataQueue.push(frame);
}

// Helper: Generate test tone for TestFeeder path
void SpectralCanvasProAudioProcessor::generateTestTone(juce::AudioBuffer<float>& buffer, int numSamples) noexcept
{
    static float phase = 0.0f;
    const float freq = 440.0f;
    const float increment = juce::MathConstants<float>::twoPi * freq / static_cast<float>(currentSampleRate);
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float value = 0.2f * std::sin(phase);
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.setSample(channel, sample, value);
        }
        
        phase += increment;
        if (phase >= juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;
    }
}