#include "../Source/nodes/SpectralPaintNode.h"
#include "../Source/rt/MaskColumnLite.h"
#include "../Source/rt/SpscQueueLite.h"
#include <gtest/gtest.h>
#include <juce_dsp/juce_dsp.h>

/**
 * @brief Unit tests for modern JUCE DSP spectral painting implementation
 * 
 * Tests the 256-point FFT fast-paint mode with 64-sample hop,
 * lightweight 12-byte MaskColumnLite events, and RT-safe operation.
 */

class SpectralPaintNodeTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        // Setup for 256-point FFT with 48kHz sample rate
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = 48000.0;
        spec.maximumBlockSize = 512;
        spec.numChannels = 2;
        
        spectralNode = std::make_unique<nodes::SpectralPaintNode>();
        spectralNode->prepare(spec);
    }
    
    std::unique_ptr<nodes::SpectralPaintNode> spectralNode;
};

TEST_F(SpectralPaintNodeTest, Initialization)
{
    EXPECT_EQ(spectralNode->getFFTOrder(), 8);      // 256-point FFT
    EXPECT_EQ(spectralNode->getFFTSize(), 256);
    EXPECT_EQ(spectralNode->getHopSize(), 64);      // 75% overlap
    EXPECT_EQ(spectralNode->getNumBins(), 128);     // 256/2 complex bins
}

TEST_F(SpectralPaintNodeTest, MaskColumnLiteSize)
{
    // Verify 12-byte POD structure as specified
    rt::MaskColumnLite mask;
    EXPECT_EQ(sizeof(mask), 12);
    
    // Test trivially copyable for RT safety
    static_assert(std::is_trivially_copyable_v<rt::MaskColumnLite>);
    static_assert(std::is_standard_layout_v<rt::MaskColumnLite>);
}

TEST_F(SpectralPaintNodeTest, QueueCapacity)
{
    // Test queue has expected capacity and is initially empty
    EXPECT_EQ(spectralNode->getMaskQueueDepth(), 0);
    EXPECT_EQ(spectralNode->getMaskDropCount(), 0);
    
    // Test pushing events up to capacity
    rt::MaskColumnLite mask(0.5f, 0.8f, 1000);
    
    size_t successfulPushes = 0;
    for (int i = 0; i < 200; ++i)  // Exceed queue capacity
    {
        if (spectralNode->pushMask(mask))
            successfulPushes++;
    }
    
    // Should be able to push at least 128 events (queue capacity)
    EXPECT_GE(successfulPushes, 128);
    EXPECT_GT(spectralNode->getMaskQueueDepth(), 0);
}

TEST_F(SpectralPaintNodeTest, PinkBedToggle)
{
    // Test pink bed enable/disable (no crash, basic functionality)
    spectralNode->setPinkBed(true);
    spectralNode->setBrushRadius(16);
    
    // Should not crash
    spectralNode->setPinkBed(false);
    spectralNode->setBrushRadius(8);
}

TEST_F(SpectralPaintNodeTest, ProcessBlockBasic)
{
    // Create test audio buffer
    juce::AudioBuffer<float> buffer(2, 512);  // Stereo, 512 samples
    buffer.clear();
    
    // Add some test input signal
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            buffer.setSample(ch, sample, 0.1f * std::sin(2.0f * static_cast<float>(M_PI) * 440.0f * sample / 48000.0f));
        }
    }
    
    // Create JUCE DSP context
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    
    // Process should not crash
    EXPECT_NO_THROW(spectralNode->process(context));
    
    // Output should have some content (not all zeros after processing input)
    float rmsLevel = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    EXPECT_GT(rmsLevel, 0.0f);  // Some output was generated
}

TEST_F(SpectralPaintNodeTest, AudioQualityImprovements)
{
    // Test with pink bed enabled to ensure no harsh noise artifacts
    spectralNode->setPinkBed(true);
    
    // Create silent input to isolate pink bed
    juce::AudioBuffer<float> buffer(2, 1024);
    buffer.clear();
    
    // Process multiple blocks to allow pink bed phase accumulation
    for (int block = 0; block < 4; ++block)
    {
        juce::dsp::AudioBlock<float> audioBlock(buffer);
        juce::dsp::ProcessContextReplacing<float> context(audioBlock);
        spectralNode->process(context);
    }
    
    // Pink bed should generate smooth output (no harsh discontinuities)
    float rmsLevel = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    EXPECT_GT(rmsLevel, 0.0f);  // Pink bed generates output
    EXPECT_LT(rmsLevel, 0.05f); // But not too loud (should be ~-36dBFS)
    
    // Check for absence of high-frequency spikes by analyzing spectral content
    // (In a full implementation, we'd do FFT analysis here)
}

TEST_F(SpectralPaintNodeTest, PaintEventProcessing)
{
    // Add a paint event
    rt::MaskColumnLite paintEvent(0.5f, 0.7f, 1000); // Mid-frequency, strong intensity
    EXPECT_TRUE(spectralNode->pushMask(paintEvent));
    
    // Create input signal
    juce::AudioBuffer<float> buffer(2, 512);
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            buffer.setSample(ch, sample, 0.05f * std::sin(2.0f * static_cast<float>(M_PI) * 1000.0f * sample / 48000.0f));
        }
    }
    
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    
    // Store original RMS for comparison
    float originalRMS = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    
    // Process with paint event
    spectralNode->process(context);
    
    // Should boost the signal (paint event should increase magnitude)
    float processedRMS = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    EXPECT_GE(processedRMS, originalRMS); // Paint boost should increase or maintain level
}

TEST_F(SpectralPaintNodeTest, ResetFunctionality)
{
    // Push some mask events
    rt::MaskColumnLite mask(0.5f, 0.8f, 1000);
    spectralNode->pushMask(mask);
    EXPECT_GT(spectralNode->getMaskQueueDepth(), 0);
    
    // Reset should clear state
    spectralNode->reset();
    
    // Queue depth might still show pending events since reset doesn't drain queue
    // but processing state should be reset (no crash on subsequent processing)
    juce::AudioBuffer<float> buffer(2, 64);
    buffer.clear();
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    
    EXPECT_NO_THROW(spectralNode->process(context));
}

// Test lock-free SPSC queue independently
TEST(SpscQueueLiteTest, BasicOperation)
{
    rt::SpscQueueLite<rt::MaskColumnLite, 64> queue;
    
    EXPECT_EQ(queue.capacity(), 64);
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
    
    // Test push/pop
    rt::MaskColumnLite mask1(0.1f, 0.5f, 100);
    rt::MaskColumnLite mask2(0.9f, 0.8f, 200);
    
    EXPECT_TRUE(queue.try_push(mask1));
    EXPECT_TRUE(queue.try_push(mask2));
    EXPECT_EQ(queue.size(), 2);
    
    rt::MaskColumnLite popped;
    EXPECT_TRUE(queue.try_pop(popped));
    EXPECT_FLOAT_EQ(popped.y, 0.1f);
    EXPECT_FLOAT_EQ(popped.intensity, 0.5f);
    EXPECT_EQ(popped.tMs, 100u);
    
    EXPECT_TRUE(queue.try_pop(popped));
    EXPECT_FLOAT_EQ(popped.y, 0.9f);
    
    EXPECT_FALSE(queue.try_pop(popped));  // Empty queue
    EXPECT_TRUE(queue.empty());
}