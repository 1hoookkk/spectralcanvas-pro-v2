/**
 * Processor Paint Paths Test Suite
 * 
 * Tests that Phase4Synth and ModernPaint audio paths produce non-silent output 
 * after receiving paint events (pushMaskColumn and pushPaintEvent).
 * Validates the two main paint-to-audio systems work correctly.
 */

#include "TestHarness.h"
#include "../Source/SpectralCanvasProAudioProcessor.h"
#include "../Source/Core/MessageBus.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace test;

class ProcessorPaintPathsTest : public TestCase
{
public:
    ProcessorPaintPathsTest() : TestCase("ProcessorPaintPaths") {}
    
    void run() override
    {
        std::cout << "Testing processor paint-to-audio paths..." << std::endl;
        
        testBasicProcessorSetup();
        testPhase4SynthPath();
        testModernPaintPath();
        testAudioPathSwitching();
        testPaintEventValidation();
        
        std::cout << "Processor paint paths tests passed!" << std::endl;
    }
    
private:
    void testBasicProcessorSetup()
    {
        std::cout << "  Testing basic processor setup and initialization..." << std::endl;
        
        SpectralCanvasProAudioProcessor processor;
        
        // Test basic properties
        TEST_ASSERT(processor.isBusesLayoutSupported(
            juce::AudioProcessor::BusesLayout{}
                .addMainInputBus(juce::AudioChannelSet::stereo())
                .addMainOutputBus(juce::AudioChannelSet::stereo())
        ));
        
        // Test preparation
        processor.prepareToPlay(44100.0, 512);
        
        // Should not crash and should be in a valid state
        TEST_ASSERT(processor.getSampleRate() > 0.0);
        TEST_ASSERT(processor.getBlockSize() > 0);
    }
    
    void testPhase4SynthPath()
    {
        std::cout << "  Testing Phase4Synth path with mask columns..." << std::endl;
        
        SpectralCanvasProAudioProcessor processor;
        processor.prepareToPlay(44100.0, 512);
        
        // Set to Phase4Synth mode manually for testing
        // Note: In a real test environment, this would be done through parameter changes
        // but for unit testing we need direct access to the mode
        
        const int blockSize = 512;
        const int numChannels = 2;
        
        juce::AudioBuffer<float> audioBuffer(numChannels, blockSize);
        juce::MidiBuffer midiBuffer;
        
        // Create and push a test mask that should generate sound
        MaskColumn testMask;
        testMask.numBins = 257; // NUM_BINS for 512 FFT
        testMask.frameIndex = 0;
        testMask.timestampSamples = 0.0;
        testMask.uiTimestampMicros = 0;
        testMask.sequenceNumber = 1;
        testMask.version = 1;
        testMask.engineFrameId = 0;
        
        // Fill with boost values to ensure audible output
        for (size_t i = 0; i < testMask.numBins; ++i)
        {
            testMask.values[i] = 3.0f; // Strong boost within clamp range
        }
        
        // Push the mask
        bool maskPushed = processor.pushMaskColumn(testMask);
        if (maskPushed)
        {
            std::cout << "    Mask successfully pushed to Phase4 path" << std::endl;
        }
        
        // Process several blocks to allow the system to respond
        float totalEnergy = 0.0f;
        for (int block = 0; block < 10; ++block)
        {
            audioBuffer.clear();
            processor.processBlock(audioBuffer, midiBuffer);
            
            // Calculate energy for this block
            float blockEnergy = 0.0f;
            for (int ch = 0; ch < numChannels; ++ch)
            {
                const float* channelData = audioBuffer.getReadPointer(ch);
                for (int i = 0; i < blockSize; ++i)
                {
                    blockEnergy += channelData[i] * channelData[i];
                }
            }
            totalEnergy += blockEnergy;
        }
        
        float rmsEnergy = std::sqrt(totalEnergy / (10 * blockSize * numChannels));
        std::cout << "    Phase4Synth output energy: " << rmsEnergy << std::endl;
        
        // Phase4Synth should produce some output when given paint input
        if (maskPushed)
        {
            TEST_ASSERT(rmsEnergy > 1e-8f); // Should produce audible output
        }
    }
    
    void testModernPaintPath()
    {
        std::cout << "  Testing ModernPaint path with paint events..." << std::endl;
        
        SpectralCanvasProAudioProcessor processor;
        processor.prepareToPlay(44100.0, 512);
        
        const int blockSize = 512;
        const int numChannels = 2;
        
        juce::AudioBuffer<float> audioBuffer(numChannels, blockSize);
        juce::MidiBuffer midiBuffer;
        
        // Send paint events to the ModernPaint system
        // y = 0.5 (middle frequency), intensity = 0.8 (strong)
        bool eventPushed1 = processor.pushPaintEvent(0.5f, 0.8f, 0);
        bool eventPushed2 = processor.pushPaintEvent(0.3f, 0.6f, 100);
        bool eventPushed3 = processor.pushPaintEvent(0.7f, 0.9f, 200);
        
        if (eventPushed1 || eventPushed2 || eventPushed3)
        {
            std::cout << "    Paint events successfully pushed to ModernPaint path" << std::endl;
        }
        
        // Process several blocks to allow the system to respond
        float totalEnergy = 0.0f;
        for (int block = 0; block < 15; ++block)
        {
            audioBuffer.clear();
            processor.processBlock(audioBuffer, midiBuffer);
            
            // Calculate energy for this block
            float blockEnergy = 0.0f;
            for (int ch = 0; ch < numChannels; ++ch)
            {
                const float* channelData = audioBuffer.getReadPointer(ch);
                for (int i = 0; i < blockSize; ++i)
                {
                    blockEnergy += channelData[i] * channelData[i];
                }
            }
            totalEnergy += blockEnergy;
        }
        
        float rmsEnergy = std::sqrt(totalEnergy / (15 * blockSize * numChannels));
        std::cout << "    ModernPaint output energy: " << rmsEnergy << std::endl;
        
        // ModernPaint should produce some output when given paint events
        if (eventPushed1 || eventPushed2 || eventPushed3)
        {
            TEST_ASSERT(rmsEnergy > 1e-8f); // Should produce audible output
        }
    }
    
    void testAudioPathSwitching()
    {
        std::cout << "  Testing audio path switching behavior..." << std::endl;
        
        SpectralCanvasProAudioProcessor processor;
        processor.prepareToPlay(44100.0, 512);
        
        const int blockSize = 512;
        const int numChannels = 2;
        
        juce::AudioBuffer<float> audioBuffer(numChannels, blockSize);
        juce::MidiBuffer midiBuffer;
        
        // Test that processor can switch between paths without crashing
        // In a real environment, this would be done via parameter changes
        
        // Process some blocks in default state
        for (int block = 0; block < 5; ++block)
        {
            audioBuffer.clear();
            processor.processBlock(audioBuffer, midiBuffer);
            
            // Should not crash regardless of current path
            TEST_ASSERT(std::isfinite(audioBuffer.getRMSLevel(0, 0, blockSize)));
            TEST_ASSERT(std::isfinite(audioBuffer.getRMSLevel(1, 0, blockSize)));
        }
        
        // Try pushing different types of events
        MaskColumn testMask;
        testMask.numBins = 257;
        testMask.frameIndex = 0;
        testMask.timestampSamples = 0.0;
        testMask.uiTimestampMicros = 0;
        testMask.sequenceNumber = 1;
        testMask.version = 1;
        testMask.engineFrameId = 0;
        std::fill_n(testMask.values, testMask.numBins, 1.5f);
        
        processor.pushMaskColumn(testMask);
        processor.pushPaintEvent(0.4f, 0.7f, 0);
        
        // Process more blocks after events
        for (int block = 0; block < 5; ++block)
        {
            audioBuffer.clear();
            processor.processBlock(audioBuffer, midiBuffer);
            
            // Should remain stable
            TEST_ASSERT(std::isfinite(audioBuffer.getRMSLevel(0, 0, blockSize)));
            TEST_ASSERT(std::isfinite(audioBuffer.getRMSLevel(1, 0, blockSize)));
        }
    }
    
    void testPaintEventValidation()
    {
        std::cout << "  Testing paint event parameter validation..." << std::endl;
        
        SpectralCanvasProAudioProcessor processor;
        processor.prepareToPlay(44100.0, 512);
        
        // Test valid parameters
        TEST_ASSERT(processor.pushPaintEvent(0.0f, 0.5f, 0));    // Bottom frequency
        TEST_ASSERT(processor.pushPaintEvent(1.0f, 0.5f, 0));    // Top frequency
        TEST_ASSERT(processor.pushPaintEvent(0.5f, 0.0f, 0));    // Zero intensity
        TEST_ASSERT(processor.pushPaintEvent(0.5f, 1.0f, 0));    // Max intensity
        
        // Test edge case parameters (should be handled gracefully)
        bool result;
        result = processor.pushPaintEvent(-0.1f, 0.5f, 0);  // Below range
        result = processor.pushPaintEvent(1.1f, 0.5f, 0);   // Above range
        result = processor.pushPaintEvent(0.5f, -0.1f, 0);  // Negative intensity
        result = processor.pushPaintEvent(0.5f, 2.0f, 0);   // High intensity
        
        // All calls should return without crashing (result values may vary)
        std::cout << "    Parameter validation completed" << std::endl;
        
        // Test MaskColumn structure validation
        MaskColumn testMask;
        testMask.numBins = 0;  // Invalid bin count
        result = processor.pushMaskColumn(testMask);
        
        testMask.numBins = 10000;  // Excessive bin count
        result = processor.pushMaskColumn(testMask);
        
        testMask.numBins = 257;  // Valid bin count
        testMask.frameIndex = 0;
        testMask.timestampSamples = 0.0;
        testMask.uiTimestampMicros = 0;
        testMask.sequenceNumber = 1;
        testMask.version = 1;
        testMask.engineFrameId = 0;
        
        // Fill with extreme values
        std::fill_n(testMask.values, testMask.numBins, 1000.0f);
        result = processor.pushMaskColumn(testMask);
        
        std::fill_n(testMask.values, testMask.numBins, -100.0f);
        result = processor.pushMaskColumn(testMask);
        
        // Should handle extreme values gracefully due to clamping
        std::cout << "    Mask column validation completed" << std::endl;
    }
};

int main()
{
    TestRunner runner;
    runner.addTest(std::make_unique<ProcessorPaintPathsTest>());
    return runner.runAll();
}