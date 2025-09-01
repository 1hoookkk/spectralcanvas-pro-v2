#include "TestHarness.h"
#include "AllocationDetector.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace test;

// Mock processor for testing processBlock performance
class TestAudioProcessor : public juce::AudioProcessor {
public:
    TestAudioProcessor() : AudioProcessor(BusesProperties()
                                        .withInput("Input", juce::AudioChannelSet::stereo(), true)
                                        .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {
        // Initialize for typical plugin configuration
        setPlayConfigDetails(2, 2, 44100.0, 512);
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        sampleRate_ = sampleRate;
        blockSize_ = samplesPerBlock;
        
        // Initialize any buffers or state here
        processCallCount_.store(0, std::memory_order_relaxed);
    }
    
    void releaseResources() override {}
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) noexcept override {
        // Simulate typical audio processing workload
        processCallCount_.fetch_add(1, std::memory_order_relaxed);
        
        const int numSamples = buffer.getNumSamples();
        const int numChannels = buffer.getNumChannels();
        
        // Simple processing: apply gain and basic filtering
        for (int ch = 0; ch < numChannels; ++ch) {
            float* channelData = buffer.getWritePointer(ch);
            
            for (int n = 0; n < numSamples; ++n) {
                // Simple lowpass filter simulation (1-pole)
                float input = channelData[n];
                lastSample_[ch] = lastSample_[ch] * 0.99f + input * 0.01f;
                channelData[n] = lastSample_[ch] * gain_;
            }
        }
    }
    
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }
    
    const juce::String getName() const override { return "TestProcessor"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "Default"; }
    void changeProgramName(int, const juce::String&) override {}
    
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}
    
    uint64_t getProcessCallCount() const { 
        return processCallCount_.load(std::memory_order_relaxed); 
    }

private:
    double sampleRate_ = 44100.0;
    int blockSize_ = 512;
    float gain_ = 0.5f;
    float lastSample_[2] = {0.0f, 0.0f};
    std::atomic<uint64_t> processCallCount_{0};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TestAudioProcessor)
};

// Test: processBlock latency measurement
class ProcessBlockLatencyTest : public TestCase {
public:
    ProcessBlockLatencyTest(double targetLatencyMs, double targetCpuPercent) 
        : TestCase("ProcessBlockLatency")
        , targetLatencyMs_(targetLatencyMs)
        , targetCpuPercent_(targetCpuPercent) {}
    
    void run() override {
        TestAudioProcessor processor;
        
        const double sampleRate = 48000.0;
        const int blockSize = 64;  // Small block size for low latency
        const int numChannels = 2;
        
        processor.prepareToPlay(sampleRate, blockSize);
        
        // Create test buffer
        juce::AudioBuffer<float> buffer(numChannels, blockSize);
        juce::MidiBuffer midiBuffer;
        
        // Fill with test signal
        for (int ch = 0; ch < numChannels; ++ch) {
            for (int n = 0; n < blockSize; ++n) {
                auto sample = static_cast<float>(0.1 * std::sin(2.0 * M_PI * 440.0 * n / sampleRate));
                buffer.setSample(ch, n, sample);
            }
        }
        
        // Warm up
        for (int i = 0; i < 100; ++i) {
            processor.processBlock(buffer, midiBuffer);
        }
        
        // Measure performance
        PerfStats latencyStats;
        const int numIterations = 10000;
        
        std::cout << "  Measuring processBlock latency over " << numIterations << " iterations..." << std::endl;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < numIterations; ++i) {
            HiResTimer timer;
            
            processor.processBlock(buffer, midiBuffer);
            
            double latencyMs = timer.elapsedMs();
            latencyStats.addSample(latencyMs);
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        
        // Calculate CPU usage
        double totalAudioTimeSeconds = (numIterations * blockSize) / sampleRate;
        double actualTimeSeconds = totalDuration.count() / 1e9;
        double cpuUsagePercent = (actualTimeSeconds / totalAudioTimeSeconds) * 100.0;
        
        // Print statistics
        latencyStats.print("ProcessBlock Latency");
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  CPU Usage: " << cpuUsagePercent << "%" << std::endl;
        std::cout << "  Target Latency: " << targetLatencyMs_ << " ms" << std::endl;
        std::cout << "  Target CPU: " << targetCpuPercent_ << "%" << std::endl;
        
        // Verify performance targets
        double p50Latency = latencyStats.percentile(0.5);
        double p99Latency = latencyStats.percentile(0.99);
        
        TEST_EXPECT_LT(p50Latency, targetLatencyMs_);
        TEST_EXPECT_LT(p99Latency, targetLatencyMs_ * 2.0);  // P99 can be 2x target
        TEST_EXPECT_LT(cpuUsagePercent, targetCpuPercent_);
        
        // Verify processor was actually called
        TEST_ASSERT(processor.getProcessCallCount() >= numIterations);
        
        std::cout << "  ✓ Latency targets met!" << std::endl;
    }
    
private:
    double targetLatencyMs_;
    double targetCpuPercent_;
};

// Test: RT safety during processBlock
class ProcessBlockRTSafetyTest : public TestCase {
public:
    ProcessBlockRTSafetyTest() : TestCase("ProcessBlockRTSafety") {}
    
    void run() override {
        TestAudioProcessor processor;
        
        const double sampleRate = 48000.0;
        const int blockSize = 64;
        const int numChannels = 2;
        
        processor.prepareToPlay(sampleRate, blockSize);
        
        // Create test buffer
        juce::AudioBuffer<float> buffer(numChannels, blockSize);
        juce::MidiBuffer midiBuffer;
        
        // Simulate RT thread
        std::thread rtThread([&]() {
            RTModeGuard guard(std::this_thread::get_id());
            RTSafeAllocator::resetCounters();
            
            // Process multiple blocks to ensure no allocations
            const int numBlocks = 1000;
            for (int i = 0; i < numBlocks; ++i) {
                processor.processBlock(buffer, midiBuffer);
            }
            
            // Verify no allocations occurred
            size_t allocCount = RTSafeAllocator::getAllocCount();
            size_t deallocCount = RTSafeAllocator::getDeallocCount();
            
            TEST_EXPECT_EQ(0u, allocCount);
            TEST_EXPECT_EQ(0u, deallocCount);
            
            std::cout << "  ✓ No allocations detected in " << numBlocks << " processBlock calls" << std::endl;
        });
        
        rtThread.join();
    }
};

// Test: Sustained performance under load
class SustainedPerformanceTest : public TestCase {
public:
    SustainedPerformanceTest() : TestCase("SustainedPerformance") {}
    
    void run() override {
        TestAudioProcessor processor;
        
        const double sampleRate = 48000.0;
        const int blockSize = 64;
        const int numChannels = 2;
        
        processor.prepareToPlay(sampleRate, blockSize);
        
        juce::AudioBuffer<float> buffer(numChannels, blockSize);
        juce::MidiBuffer midiBuffer;
        
        // Test sustained processing for 10 seconds of audio
        const double testDurationSeconds = 10.0;
        const int totalBlocks = static_cast<int>((testDurationSeconds * sampleRate) / blockSize);
        
        std::cout << "  Testing sustained performance for " << testDurationSeconds 
                  << " seconds (" << totalBlocks << " blocks)..." << std::endl;
        
        PerfStats latencyStats;
        auto startTime = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < totalBlocks; ++i) {
            HiResTimer timer;
            processor.processBlock(buffer, midiBuffer);
            double latencyMs = timer.elapsedMs();
            latencyStats.addSample(latencyMs);
            
            // Add some variability to simulate real-world conditions
            if (i % 1000 == 0) {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        
        double actualTimeSeconds = totalDuration.count() / 1e9;
        double cpuUsagePercent = (actualTimeSeconds / testDurationSeconds) * 100.0;
        
        latencyStats.print("Sustained Performance");
        std::cout << "  Sustained CPU Usage: " << std::fixed << std::setprecision(2) 
                  << cpuUsagePercent << "%" << std::endl;
        
        // Verify performance remains stable
        TEST_EXPECT_LT(latencyStats.percentile(0.95), 5.0);  // P95 < 5ms
        TEST_EXPECT_LT(cpuUsagePercent, 20.0);  // CPU < 20% for sustained load
        
        std::cout << "  ✓ Sustained performance targets met!" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    // Parse command line arguments
    double latencyTarget = 5.0;  // ms
    double cpuTarget = 15.0;     // %
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--latency-target" && i + 1 < argc) {
            latencyTarget = std::stod(argv[++i]);
        } else if (arg == "--cpu-target" && i + 1 < argc) {
            cpuTarget = std::stod(argv[++i]);
        }
    }
    
    std::cout << "ProcessBlock Performance Test" << std::endl;
    std::cout << "Targets - Latency: " << latencyTarget << "ms, CPU: " << cpuTarget << "%" << std::endl;
    std::cout << std::endl;
    
    TestRunner runner;
    
    runner.addTest(std::make_unique<ProcessBlockLatencyTest>(latencyTarget, cpuTarget));
    runner.addTest(std::make_unique<ProcessBlockRTSafetyTest>());
    runner.addTest(std::make_unique<SustainedPerformanceTest>());
    
    return runner.runAll();
}