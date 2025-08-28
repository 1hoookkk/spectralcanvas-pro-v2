# JUCE Audio Plugin Development Standards
## SpectralCanvas Pro - EMU Audity 2000 Hybrid Implementation

**Focus Areas:** Real-Time Safety, DSP Processing, JUCE Integration, Clean-Room Implementation  
**Target Performance:** <5ms latency, 64-voice polyphony, zero audio dropouts

---

## 🔧 CORE DEVELOPMENT PRINCIPLES

### Real-Time Safety First
- **No Dynamic Allocation:** All memory allocation must be done during initialization
- **Lock-Free Design:** Use lock-free queues and atomic operations for inter-thread communication
- **Predictable Performance:** All operations must have bounded execution time
- **No Exceptions:** Exception handling is forbidden in audio processing threads

### Clean-Room Implementation
- **Behavioral Modeling:** Recreate functionality through analysis, not copying
- **Original Algorithms:** Implement our own DSP algorithms based on behavioral understanding
- **Documentation:** Complete documentation of all reverse-engineering activities
- **Legal Compliance:** Strict adherence to intellectual property guidelines

---

## 🏗️ ARCHITECTURE STANDARDS

### Plugin Structure
```cpp
// CORRECT: Standard JUCE plugin structure
class SpectralCanvasProAudioProcessor : public juce::AudioProcessor
{
public:
    // Constructor - initialize all resources
    SpectralCanvasProAudioProcessor();
    
    // Destructor - clean up resources
    ~SpectralCanvasProAudioProcessor() override;
    
    // Audio processing - RT-safe only
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    // Parameter management
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    
private:
    // RT-safe components
    std::unique_ptr<SpectralSynthEngine> synthEngine;
    std::unique_ptr<PaintQueue> paintQueue;
    std::unique_ptr<ZPlaneFilterEngine> filterEngine;
    
    // Parameter system
    juce::AudioProcessorValueTreeState apvts;
    
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR
};
```

### Memory Management
```cpp
// CORRECT: Pre-allocated memory pools
class VoiceAllocator
{
public:
    VoiceAllocator(size_t maxVoices)
    {
        // Pre-allocate all voice objects
        voices.reserve(maxVoices);
        for (size_t i = 0; i < maxVoices; ++i)
        {
            voices.emplace_back(std::make_unique<Voice>());
        }
    }
    
    // RT-safe voice allocation
    Voice* allocateVoice()
    {
        // Use lock-free allocation strategy
        return availableVoices.pop();
    }
    
private:
    std::vector<std::unique_ptr<Voice>> voices;
    LockFreeQueue<Voice*> availableVoices;
};
```

---

## 🎵 DSP PROCESSING STANDARDS

### Real-Time Processing Chain
```cpp
// CORRECT: RT-safe processing pipeline
void SpectralCanvasProAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer, 
    juce::MidiBuffer& midiMessages)
{
    // 1. Process paint gesture queue (lock-free)
    paintQueue->processPendingGestures();
    
    // 2. Update parameters (atomic operations only)
    updateParameters();
    
    // 3. Process audio (no allocations, no exceptions)
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Process each sample - bounded execution time
            channelData[sample] = processSample(channelData[sample], channel);
        }
    }
    
    // 4. Handle MIDI (RT-safe)
    handleMidiMessages(midiMessages);
}
```

### Z-Plane Filter Implementation
```cpp
// CORRECT: Behavioral recreation approach
class ZPlaneFilter
{
public:
    // Filter morphing - our own implementation
    void morphBetweenTypes(FilterType from, FilterType to, float morphAmount)
    {
        // Calculate interpolation coefficients
        auto coeffs = calculateMorphCoefficients(from, to, morphAmount);
        
        // Apply to our filter implementation
        filter.setCoefficients(coeffs);
    }
    
private:
    // Our own filter implementation
    IIRFilter filter;
    
    // Behavioral model from analysis
    FilterBehaviorModel behaviorModel;
    
    // Calculate morph coefficients based on behavioral analysis
    FilterCoefficients calculateMorphCoefficients(
        FilterType from, FilterType to, float morph);
};
```

### Sample Playback Engine
```cpp
// CORRECT: Efficient sample management
class SamplePlaybackEngine
{
public:
    // Pre-load samples during initialization
    void loadSampleLibrary(const juce::File& sampleDirectory)
    {
        // Load all samples into memory during initialization
        for (auto& sampleFile : sampleDirectory.findChildFiles(juce::File::findFiles))
        {
            auto sample = std::make_unique<Sample>();
            sample->loadFromFile(sampleFile);
            sampleLibrary.emplace(sampleFile.getFileNameWithoutExtension(), 
                                 std::move(sample));
        }
    }
    
    // RT-safe sample playback
    float playSample(const juce::String& sampleName, float position)
    {
        auto it = sampleLibrary.find(sampleName);
        if (it != sampleLibrary.end())
        {
            return it->second->getSample(position);
        }
        return 0.0f;
    }
    
private:
    // Pre-allocated sample library
    std::unordered_map<juce::String, std::unique_ptr<Sample>> sampleLibrary;
};
```

---

## 🎨 UI/UX DEVELOPMENT STANDARDS

### Paint Canvas Integration
```cpp
// CORRECT: RT-safe paint gesture processing
class PaintCanvasComponent : public juce::Component
{
public:
    // Handle paint gestures
    void mouseDown(const juce::MouseEvent& event) override
    {
        // Queue gesture for RT thread processing
        paintQueue->addGesture(PaintGesture::Type::Start, 
                              event.position, 
                              event.pressure);
    }
    
    void mouseDrag(const juce::MouseEvent& event) override
    {
        // Queue continuous gesture updates
        paintQueue->addGesture(PaintGesture::Type::Continue, 
                              event.position, 
                              event.pressure);
    }
    
private:
    // Lock-free queue for RT communication
    std::shared_ptr<PaintQueue> paintQueue;
};
```

### Parameter Visualization
```cpp
// CORRECT: Real-time parameter display
class FilterMorphingDisplay : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {
        // Visualize current filter morphing state
        auto currentFilter = filterEngine->getCurrentFilterType();
        auto morphAmount = filterEngine->getMorphAmount();
        
        // Draw filter type indicator
        drawFilterType(g, currentFilter);
        
        // Draw morphing progress
        drawMorphingProgress(g, morphAmount);
    }
    
private:
    std::shared_ptr<ZPlaneFilterEngine> filterEngine;
};
```

---

## 🧪 TESTING STANDARDS

### Real-Time Safety Testing
```cpp
// CORRECT: RT-safety validation
class RTSafetyTest : public juce::UnitTest
{
public:
    void runTest() override
    {
        beginTest("Real-time safety validation");
        
        // Test for dynamic allocations
        expectNoDynamicAllocations();
        
        // Test for exceptions
        expectNoExceptions();
        
        // Test for bounded execution time
        expectBoundedExecutionTime();
        
        // Test for lock-free operations
        expectLockFreeOperations();
    }
    
private:
    void expectNoDynamicAllocations()
    {
        // Monitor memory allocation during audio processing
        // Should be zero allocations in RT thread
    }
    
    void expectNoExceptions()
    {
        // Process audio with various inputs
        // Should never throw exceptions
    }
    
    void expectBoundedExecutionTime()
    {
        // Measure processing time under various loads
        // Should always be under 5ms
    }
    
    void expectLockFreeOperations()
    {
        // Verify all inter-thread communication is lock-free
    }
};
```

### Performance Benchmarking
```cpp
// CORRECT: Performance validation
class PerformanceTest : public juce::UnitTest
{
public:
    void runTest() override
    {
        beginTest("Performance benchmarking");
        
        // Test maximum polyphony
        testMaxPolyphony();
        
        // Test complex filter morphing
        testFilterMorphingPerformance();
        
        // Test paint gesture latency
        testPaintGestureLatency();
        
        // Test memory usage
        testMemoryUsage();
    }
    
private:
    void testMaxPolyphony()
    {
        // Create 64 simultaneous voices
        // Verify no audio dropouts
        // Verify CPU usage within limits
    }
    
    void testFilterMorphingPerformance()
    {
        // Rapidly morph between filter types
        // Verify smooth transitions
        // Verify no audio artifacts
    }
    
    void testPaintGestureLatency()
    {
        // Measure time from gesture to audio response
        // Should be <5ms
    }
    
    void testMemoryUsage()
    {
        // Monitor memory usage during operation
        // Should be stable and predictable
    }
};
```

---

## 📋 CODE QUALITY STANDARDS

### Naming Conventions
```cpp
// CORRECT: Clear, descriptive naming
class SpectralCanvasZPlaneFilterEngine  // Descriptive class names
{
public:
    void morphBetweenFilterTypes(FilterType from, FilterType to, float amount);
    float processSample(float input, int channel);
    void setMorphingSensitivity(float sensitivity);
    
private:
    std::unique_ptr<IIRFilter> filterImplementation;
    FilterBehaviorConfig behaviorConfiguration;
    std::atomic<float> currentMorphAmount{0.0f};
};
```

### Documentation Standards
```cpp
/**
 * @brief Z-Plane filter morphing engine for SpectralCanvas Pro
 * 
 * This class implements the behavioral recreation of EMU Audity 2000's
 * Z-plane filter system. It provides real-time morphing between 50
 * different filter types with smooth interpolation.
 * 
 * @note This is a clean-room implementation based on behavioral analysis
 * of the original hardware. No copyrighted algorithms are used.
 * 
 * @warning All methods must be RT-safe for audio processing threads.
 */
class ZPlaneFilterEngine
{
public:
    /**
     * @brief Morph between two filter types
     * @param from Starting filter type
     * @param to Target filter type  
     * @param morphAmount Interpolation amount (0.0 to 1.0)
     * 
     * @note RT-safe: Can be called from audio processing thread
     * @note Thread-safe: Uses atomic operations for parameter updates
     */
    void morphBetweenTypes(FilterType from, FilterType to, float morphAmount);
};
```

### Error Handling
```cpp
// CORRECT: RT-safe error handling
class SafeAudioProcessor
{
public:
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
    {
        // Use CrashToggles for feature safety
        if (crashToggles.isFeatureEnabled("Z_PLANE_FILTERS"))
        {
            try
            {
                filterEngine->processBlock(buffer);
            }
            catch (...)
            {
                // Log error but don't crash audio thread
                errorLogger.logError("Filter processing failed");
                
                // Fall back to safe processing
                safeFallbackProcessing(buffer);
            }
        }
        else
        {
            // Feature disabled - use basic processing
            basicProcessing(buffer);
        }
    }
    
private:
    CrashToggles crashToggles;
    ErrorLogger errorLogger;
};
```

---

## 🔒 LEGAL COMPLIANCE STANDARDS

### Clean-Room Implementation
```cpp
// CORRECT: Behavioral recreation approach
class EMUBehavioralModel
{
public:
    // Study and document behavior, then recreate
    void analyzeFilterBehavior(const FilterPreset& preset)
    {
        // Document parameter relationships
        // Document response characteristics
        // Document modulation behaviors
        
        // Create our own implementation based on understanding
        createBehavioralModel(preset);
    }
    
private:
    // Our own behavioral model
    FilterBehaviorModel ourModel;
    
    // Document all analysis activities
    void documentAnalysis(const FilterPreset& preset);
};
```

### Intellectual Property Protection
```cpp
// CORRECT: No copyrighted content usage
class SampleLibrary
{
public:
    // Create our own sample library
    void createCompatibleSamples()
    {
        // Generate compatible waveforms
        // Use our own synthesis algorithms
        // Create original content
    }
    
private:
    // Our own sample generation
    void generateCompatibleWaveform(const WaveformSpec& spec);
};
```

---

## 🚀 BUILD & DEPLOYMENT STANDARDS

### CMake Configuration
```cmake
# CORRECT: Proper CMake configuration
cmake_minimum_required(VERSION 3.15)

project(SpectralCanvasPro VERSION 1.0.0)

# JUCE integration
find_package(JUCE REQUIRED)

# Audio plugin target
juce_add_plugin(SpectralCanvasPro
    COMPANY_NAME "Your Company"
    PLUGIN_NAME "SpectralCanvas Pro"
    COMPANY_WEBSITE "https://yourcompany.com"
    COMPANY_EMAIL "support@yourcompany.com"
    PLUGIN_MANUFACTURER_CODE Ycom
    PLUGIN_CODE Spcp
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "SpectralCanvas Pro"
    COMPANY_WEBSITE "https://yourcompany.com"
    COMPANY_EMAIL "support@yourcompany.com"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    VST3_CATEGORIES "Synth"
    AU_MAIN_TYPE "kAudioUnitType_MusicDevice"
)

# Source files
target_sources(SpectralCanvasPro
    PRIVATE
        Source/Core/PluginProcessor.cpp
        Source/Core/SpectralSynthEngine.cpp
        Source/Core/ZPlaneFilterEngine.cpp
        Source/Core/PaintQueue.cpp
        Source/GUI/PluginEditor.cpp
        Source/GUI/PaintCanvasComponent.cpp
)

# JUCE modules
target_link_libraries(SpectralCanvasPro
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_gui_extra
        juce::juce_dsp
)
```

### Testing Integration
```cmake
# CORRECT: Testing configuration
option(BUILD_TESTS "Build test suite" ON)

if(BUILD_TESTS)
    enable_testing()
    
    # Add test executable
    add_executable(SpectralCanvasProTests
        Tests/UnitTests.cpp
        Tests/RTSafetyTests.cpp
        Tests/PerformanceTests.cpp
    )
    
    target_link_libraries(SpectralCanvasProTests
        PRIVATE
            SpectralCanvasPro
            juce::juce_unit_tests
    )
    
    # Add tests
    add_test(NAME UnitTests COMMAND SpectralCanvasProTests)
    add_test(NAME RTSafetyTests COMMAND SpectralCanvasProTests --gtest_filter="RTSafety*")
    add_test(NAME PerformanceTests COMMAND SpectralCanvasProTests --gtest_filter="Performance*")
endif()
```

---

## 📊 MONITORING & DEBUGGING

### Performance Monitoring
```cpp
// CORRECT: RT-safe performance monitoring
class PerformanceMonitor
{
public:
    void startFrame()
    {
        frameStartTime = std::chrono::high_resolution_clock::now();
    }
    
    void endFrame()
    {
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEndTime - frameStartTime;
        
        // Update running average (atomic)
        updateAverageFrameTime(frameDuration);
        
        // Check for performance issues
        if (frameDuration > maxAllowedFrameTime)
        {
            performanceLogger.logWarning("Frame time exceeded limit");
        }
    }
    
private:
    std::chrono::high_resolution_clock::time_point frameStartTime;
    std::atomic<std::chrono::nanoseconds> averageFrameTime{0};
    std::chrono::nanoseconds maxAllowedFrameTime{std::chrono::milliseconds(5)};
};
```

### Debugging Tools
```cpp
// CORRECT: RT-safe debugging
class AudioDebugger
{
public:
    // Emergency test tone (RT-safe)
    void toggleTestTone()
    {
        testToneEnabled = !testToneEnabled;
    }
    
    // Generate test tone in audio processing
    float generateTestTone(float frequency, float amplitude)
    {
        if (!testToneEnabled) return 0.0f;
        
        // Generate sine wave test tone
        phase += 2.0f * M_PI * frequency / sampleRate;
        return amplitude * std::sin(phase);
    }
    
private:
    std::atomic<bool> testToneEnabled{false};
    float phase = 0.0f;
    float sampleRate = 44100.0f;
};
```

---

**Document Status:** ACTIVE - Development Ready  
**Last Updated:** August 26, 2025  
**Next Review:** Phase 1 Milestone Completion
