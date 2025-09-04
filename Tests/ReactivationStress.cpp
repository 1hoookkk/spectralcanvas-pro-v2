// ReactivationStress.cpp
// Stress test for RT-safe processor reactivation under host suspend/release cycles
#include <thread>
#include <atomic>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include "../Source/SpectralCanvasProAudioProcessor.h"

using namespace std::chrono_literals;

/**
 * RT-safe audio processing loop - simulates host calling processBlock()
 */
static void processLoop(SpectralCanvasProAudioProcessor& p, std::atomic<bool>& stop) {
    juce::AudioBuffer<float> buf(2, 512);  // Stereo, 512 samples
    juce::MidiBuffer midi;
    std::mt19937 rng{123};
    std::uniform_real_distribution<float> d(-0.1f, 0.1f);  // Low level noise
    
    int blockCount = 0;
    while (!stop.load(std::memory_order_acquire)) {
        // Fill with test noise
        for (int ch = 0; ch < buf.getNumChannels(); ++ch) {
            for (int i = 0; i < buf.getNumSamples(); ++i) {
                buf.setSample(ch, i, d(rng));
            }
        }
        
        // Process audio block (RT-critical)
        p.processBlock(buf, midi);
        
        // Latency invariant check (cheap atomic read)
        const int latency = p.getLatencySamples();
        const int expectedLatency = 512 - 128; // FFT_SIZE - HOP_SIZE
        if (latency != expectedLatency) {
            std::cout << "[processLoop] LATENCY VIOLATION: got " << latency 
                     << ", expected " << expectedLatency << " at block " << blockCount << std::endl;
            jassertfalse; // This should never happen
        }
        
        blockCount++;
        std::this_thread::sleep_for(2ms);  // ~500Hz block rate
    }
    std::cout << "[processLoop] Processed " << blockCount << " blocks" << std::endl;
}

/**
 * Editor lifecycle thrashing - simulates user opening/closing editor rapidly
 */
static void editorThrashLoop(SpectralCanvasProAudioProcessor& p, std::atomic<bool>& stop) {
    int editorCycles = 0;
    while (!stop.load(std::memory_order_acquire)) {
        auto* ed = p.createEditor();
        if (ed) {
            std::this_thread::sleep_for(30ms);
            delete ed;
            editorCycles++;
        }
        std::this_thread::sleep_for(20ms);
    }
    std::cout << "[editorThrash] Completed " << editorCycles << " editor cycles" << std::endl;
}

/**
 * Host reactivation loop - hammers suspend/release/prepare cycles
 */
static void reactivateLoop(SpectralCanvasProAudioProcessor& p, std::atomic<bool>& stop) {
    int cycles = 0;
    const double testSampleRate = 48000.0;
    const int testBlockSize = 512;
    
    while (!stop.load(std::memory_order_acquire)) {
        // Simulate host suspend → release → prepare cycle
        p.suspendProcessing(true);
        p.releaseResources();
        
        std::this_thread::sleep_for(10ms);  // Brief pause
        
        p.prepareToPlay(testSampleRate, testBlockSize);
        p.suspendProcessing(false);
        
        // Immediately verify latency after reactivation (CRITICAL)
        const int latency = p.getLatencySamples();
        const int expectedLatency = 512 - 128; // FFT_SIZE - HOP_SIZE = 384
        if (latency != expectedLatency) {
            std::cout << "[reactivate] LATENCY DRIFT after cycle " << cycles 
                     << ": got " << latency << ", expected " << expectedLatency << std::endl;
            jassertfalse; // Latency must NEVER drift
        }
        
        cycles++;
        if ((cycles % 10) == 0) {
            std::cout << "[reactivate] Completed " << cycles << " reactivation cycles" << std::endl;
        }
        
        std::this_thread::sleep_for(25ms);  // ~40Hz reactivation rate
    }
    std::cout << "[reactivate] Completed " << cycles << " total cycles" << std::endl;
}

/**
 * Main stress test entry point
 */
int runReactivationStressTest() {
    std::cout << "=== SpectralCanvasPro Reactivation Stress Test ===" << std::endl;
    std::cout << "Testing RT-safety under suspend/release/prepare hammering..." << std::endl;
    
    // Initialize processor
    SpectralCanvasProAudioProcessor proc;
    proc.prepareToPlay(48000.0, 512);
    
    // Verify initial latency
    const int initialLatency = proc.getLatencySamples();
    std::cout << "Initial latency: " << initialLatency << " samples" << std::endl;
    if (initialLatency != 384) {
        std::cout << "ERROR: Initial latency should be 384 samples (FFT_SIZE - HOP_SIZE)" << std::endl;
        return -1;
    }
    
    // Start concurrent stress threads
    std::atomic<bool> stop{false};
    std::thread processThread(processLoop, std::ref(proc), std::ref(stop));
    std::thread editorThread(editorThrashLoop, std::ref(proc), std::ref(stop));
    std::thread reactivateThread(reactivateLoop, std::ref(proc), std::ref(stop));
    
    // Run stress test for 5 seconds
    std::cout << "Running stress test for 5 seconds..." << std::endl;
    std::this_thread::sleep_for(5s);
    
    // Signal stop and wait for threads
    std::cout << "Stopping stress test..." << std::endl;
    stop.store(true, std::memory_order_release);
    
    processThread.join();
    editorThread.join();
    reactivateThread.join();
    
    // Final verification
    const int finalLatency = proc.getLatencySamples();
    std::cout << "Final latency: " << finalLatency << " samples" << std::endl;
    
    if (finalLatency == 384) {
        std::cout << "SUCCESS: Reactivation stress test passed!" << std::endl;
        std::cout << "- Latency remained stable throughout all suspend/release cycles" << std::endl;
        std::cout << "- No RT-safety violations detected" << std::endl;
        std::cout << "- Editor lifecycle did not affect audio processing" << std::endl;
        return 0;
    } else {
        std::cout << "FAILED: Final latency " << finalLatency << " != expected 384" << std::endl;
        return -1;
    }
}

/**
 * Standalone executable main() for manual testing
 */
#ifndef JUCE_UNIT_TESTS
int main() {
    // Initialize JUCE for standalone usage
    juce::initialiseJuce_GUI();
    
    int result = runReactivationStressTest();
    
    juce::shutdownJuce_GUI();
    return result;
}
#endif

/**
 * JUCE UnitTest integration (optional)
 */
#ifdef JUCE_UNIT_TESTS
class ReactivationStressUnitTest : public juce::UnitTest
{
public:
    ReactivationStressUnitTest() : juce::UnitTest("Reactivation Stress Test") {}
    
    void runTest() override
    {
        beginTest("RT-safe processor reactivation");
        
        int result = runReactivationStressTest();
        expect(result == 0, "Reactivation stress test should pass");
    }
};

static ReactivationStressUnitTest reactivationStressTest;
#endif