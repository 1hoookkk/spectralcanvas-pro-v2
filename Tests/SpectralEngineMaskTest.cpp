/**
 * SpectralEngine Mask Test Suite
 * 
 * Tests mask application, clamping to [0.1, 10.0] range, generative mode with 
 * carrier injection, and spectral processing behavior under various conditions.
 */

#include "TestHarness.h"
#include "../Source/DSP/SpectralEngine.h"
#include "../Source/Core/MessageBus.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace test;

class SpectralEngineMaskTest : public TestCase
{
public:
    SpectralEngineMaskTest() : TestCase("SpectralEngineMask") {}
    
    void run() override
    {
        std::cout << "Testing SpectralEngine mask application..." << std::endl;
        
        testBasicInitialization();
        testMaskApplication();
        testMaskClamping();
        testGenerativeMode();
        testSilenceToCarrier();
        testMaskColumnStructure();
        
        std::cout << "SpectralEngine mask tests passed!" << std::endl;
    }
    
private:
    void testBasicInitialization()
    {
        std::cout << "  Testing basic initialization..." << std::endl;
        
        SpectralEngine engine;
        TEST_ASSERT(!engine.isInitialized());
        
        // Initialize with standard audio parameters
        engine.initialize(44100.0, 512);
        TEST_ASSERT(engine.isInitialized());
        TEST_EXPECT_EQ(44100.0, engine.getSampleRate());
        
        engine.reset();
        TEST_ASSERT(engine.isInitialized()); // Should remain initialized after reset
    }
    
    void testMaskApplication()
    {
        std::cout << "  Testing mask application to spectrum..." << std::endl;
        
        SpectralEngine engine;
        engine.initialize(44100.0, 512);
        
        // Create a test mask with known values in valid range
        MaskColumn testMask;
        testMask.numBins = static_cast<uint32_t>(SpectralEngine::NUM_BINS);
        testMask.frameIndex = 0;
        testMask.timestampSamples = 0.0;
        testMask.uiTimestampMicros = 0;
        testMask.sequenceNumber = 1;
        testMask.version = 1;
        testMask.engineFrameId = 0;
        
        // Fill mask with alternating attenuation/boost pattern within valid range
        for (size_t i = 0; i < SpectralEngine::NUM_BINS; ++i)
        {
            testMask.values[i] = (i % 2 == 0) ? 0.5f : 2.0f;
        }
        
        // Generate test input signal (sine wave at 1kHz)
        const int blockSize = 512;
        std::vector<float> inputBuffer(blockSize);
        std::vector<float> outputBuffer(blockSize, 0.0f);
        
        generateTestSineWave(inputBuffer.data(), blockSize, 1000.0f, 44100.0f);
        
        // Process several blocks to let the engine warm up
        for (int block = 0; block < 5; ++block)
        {
            std::fill(outputBuffer.begin(), outputBuffer.end(), 0.0f);
            engine.processBlock(inputBuffer.data(), outputBuffer.data(), blockSize);
        }
        
        float outputEnergyBefore = calculateRMSEnergy(outputBuffer.data(), blockSize);
        
        // Apply mask and process several more blocks
        engine.applyMaskColumn(testMask);
        
        for (int block = 0; block < 5; ++block)
        {
            std::fill(outputBuffer.begin(), outputBuffer.end(), 0.0f);
            engine.processBlock(inputBuffer.data(), outputBuffer.data(), blockSize);
        }
        
        float outputEnergyAfter = calculateRMSEnergy(outputBuffer.data(), blockSize);
        
        // With alternating 0.5x/2.0x mask and 2.0f boost, should produce audible output
        TEST_ASSERT(outputEnergyAfter > 0.0f);
        std::cout << "    Energy before mask: " << outputEnergyBefore << std::endl;
        std::cout << "    Energy after mask: " << outputEnergyAfter << std::endl;
    }
    
    void testMaskClamping()
    {
        std::cout << "  Testing mask value clamping [0.1, 10.0]..." << std::endl;
        
        SpectralEngine engine;
        engine.initialize(44100.0, 512);
        
        // Create mask with extreme values that should be clamped
        MaskColumn extremeMask;
        extremeMask.numBins = static_cast<uint32_t>(SpectralEngine::NUM_BINS);
        extremeMask.frameIndex = 0;
        extremeMask.timestampSamples = 0.0;
        extremeMask.uiTimestampMicros = 0;
        extremeMask.sequenceNumber = 1;
        extremeMask.version = 1;
        extremeMask.engineFrameId = 0;
        
        // Fill with values outside expected range [0.1, 10.0]
        for (size_t i = 0; i < SpectralEngine::NUM_BINS; ++i)
        {
            if (i < SpectralEngine::NUM_BINS / 2)
                extremeMask.values[i] = 0.001f; // Should clamp to 0.1f
            else
                extremeMask.values[i] = 100.0f; // Should clamp to 10.0f
        }
        
        // Generate test signal
        const int blockSize = 512;
        std::vector<float> inputBuffer(blockSize);
        std::vector<float> outputBuffer(blockSize, 0.0f);
        
        generateTestSineWave(inputBuffer.data(), blockSize, 1000.0f, 44100.0f, 0.1f);
        
        // Process with extreme mask - should not crash due to clamping
        engine.applyMaskColumn(extremeMask);
        
        for (int block = 0; block < 3; ++block)
        {
            std::fill(outputBuffer.begin(), outputBuffer.end(), 0.0f);
            engine.processBlock(inputBuffer.data(), outputBuffer.data(), blockSize);
        }
        
        // Should produce reasonable output due to clamping and 2.0f boost
        float outputEnergy = calculateRMSEnergy(outputBuffer.data(), blockSize);
        TEST_ASSERT(outputEnergy > 0.0f);
        TEST_ASSERT(outputEnergy < 10.0f); // Reasonable upper bound due to clamping
        
        // Check for NaN or infinite values
        bool hasInvalidValues = false;
        for (int i = 0; i < blockSize; ++i)
        {
            if (!std::isfinite(outputBuffer[i]))
            {
                hasInvalidValues = true;
                break;
            }
        }
        TEST_ASSERT(!hasInvalidValues);
        
        std::cout << "    Clamped mask output energy: " << outputEnergy << std::endl;
    }
    
    void testGenerativeMode()
    {
        std::cout << "  Testing generative mode with carrier injection..." << std::endl;
        
        SpectralEngine engine;
        engine.initialize(44100.0, 512);
        
        // Create mask for boosting frequencies
        MaskColumn boostMask;
        boostMask.numBins = static_cast<uint32_t>(SpectralEngine::NUM_BINS);
        boostMask.frameIndex = 0;
        boostMask.timestampSamples = 0.0;
        boostMask.uiTimestampMicros = 0;
        boostMask.sequenceNumber = 1;
        boostMask.version = 1;
        boostMask.engineFrameId = 0;
        
        std::fill_n(boostMask.values, SpectralEngine::NUM_BINS, 5.0f); // Strong boost
        
        const int blockSize = 512;
        std::vector<float> silentInput(blockSize, 0.0f); // Silent input
        std::vector<float> outputBuffer(blockSize, 0.0f);
        
        // Process silent input with boost mask - should generate carrier
        engine.applyMaskColumn(boostMask);
        
        for (int block = 0; block < 10; ++block)
        {
            std::fill(outputBuffer.begin(), outputBuffer.end(), 0.0f);
            engine.processBlock(silentInput.data(), outputBuffer.data(), blockSize);
        }
        
        float outputEnergy = calculateRMSEnergy(outputBuffer.data(), blockSize);
        
        // Should produce audible output from silence due to carrier injection
        TEST_ASSERT(outputEnergy > 1e-6f);
        std::cout << "    Generative output energy from silence: " << outputEnergy << std::endl;
    }
    
    void testSilenceToCarrier()
    {
        std::cout << "  Testing silence detection and carrier injection..." << std::endl;
        
        SpectralEngine engine;
        engine.initialize(44100.0, 512);
        
        const int blockSize = 512;
        std::vector<float> veryQuietInput(blockSize);
        std::vector<float> outputBuffer(blockSize, 0.0f);
        
        // Generate extremely quiet input (below -80dB threshold)
        generateTestSineWave(veryQuietInput.data(), blockSize, 440.0f, 44100.0f, 1e-6f);
        
        // Create unity mask
        MaskColumn unityMask;
        unityMask.numBins = static_cast<uint32_t>(SpectralEngine::NUM_BINS);
        unityMask.frameIndex = 0;
        unityMask.timestampSamples = 0.0;
        unityMask.uiTimestampMicros = 0;
        unityMask.sequenceNumber = 1;
        unityMask.version = 1;
        unityMask.engineFrameId = 0;
        
        std::fill_n(unityMask.values, SpectralEngine::NUM_BINS, 1.0f);
        
        engine.applyMaskColumn(unityMask);
        
        for (int block = 0; block < 5; ++block)
        {
            std::fill(outputBuffer.begin(), outputBuffer.end(), 0.0f);
            engine.processBlock(veryQuietInput.data(), outputBuffer.data(), blockSize);
        }
        
        float outputEnergy = calculateRMSEnergy(outputBuffer.data(), blockSize);
        
        // Should inject carrier for very quiet input
        TEST_ASSERT(outputEnergy > 1e-5f);
        std::cout << "    Carrier injection energy: " << outputEnergy << std::endl;
    }
    
    void testMaskColumnStructure()
    {
        std::cout << "  Testing MaskColumn structure constraints..." << std::endl;
        
        // Verify the structure meets RT-safety requirements
        static_assert(std::is_trivially_copyable_v<MaskColumn>, 
                     "MaskColumn must be trivially copyable for RT safety");
        
        MaskColumn testMask;
        
        // Test default initialization
        TEST_EXPECT_EQ(0U, testMask.frameIndex);
        TEST_EXPECT_EQ(0U, testMask.numBins);
        TEST_EXPECT_EQ(0.0, testMask.timestampSamples);
        TEST_EXPECT_EQ(0U, testMask.uiTimestampMicros);
        TEST_EXPECT_EQ(0U, testMask.sequenceNumber);
        TEST_EXPECT_EQ(1U, testMask.version);
        TEST_EXPECT_EQ(0ULL, testMask.engineFrameId);
        
        // All values should be initialized to 1.0 (no masking by default)
        for (size_t i = 0; i < MaskColumn::MAX_BINS; ++i)
        {
            TEST_EXPECT_EQ(1.0f, testMask.values[i]);
        }
        
        std::cout << "    MaskColumn structure validation passed" << std::endl;
    }
    
    // Helper methods
    void generateTestSineWave(float* buffer, int numSamples, float frequency, 
                             float sampleRate, float amplitude = 0.1f)
    {
        const float phaseIncrement = 2.0f * static_cast<float>(M_PI) * frequency / sampleRate;
        static float phase = 0.0f;
        
        for (int i = 0; i < numSamples; ++i)
        {
            buffer[i] = amplitude * std::sin(phase);
            phase += phaseIncrement;
            if (phase > 2.0f * static_cast<float>(M_PI))
                phase -= 2.0f * static_cast<float>(M_PI);
        }
    }
    
    float calculateRMSEnergy(const float* buffer, int numSamples)
    {
        float sum = 0.0f;
        for (int i = 0; i < numSamples; ++i)
        {
            sum += buffer[i] * buffer[i];
        }
        return std::sqrt(sum / numSamples);
    }
};

int main()
{
    TestRunner runner;
    runner.addTest(std::make_unique<SpectralEngineMaskTest>());
    return runner.runAll();
}