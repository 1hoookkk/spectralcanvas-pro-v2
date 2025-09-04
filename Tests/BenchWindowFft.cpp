#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <chrono>
#include <vector>
#include <iostream>

class BenchWindowFFT {
public:
    static constexpr int FFT_SIZE = 512;
    static constexpr int NUM_TRIALS = 1000;
    
    BenchWindowFFT() 
        : fft(juce::dsp::FFT::findBestFFTSizeFor(FFT_SIZE))
        , window(FFT_SIZE, juce::dsp::WindowingFunction<float>::hann) {
        
        // Preallocate buffers
        input.resize(FFT_SIZE);
        output.resize(FFT_SIZE * 2); // Complex output
        
        // Generate test signal
        for (int i = 0; i < FFT_SIZE; ++i) {
            input[i] = std::sin(2.0f * juce::MathConstants<float>::pi * 440.0f * i / 48000.0f);
        }
    }
    
    // Current approach: separate windowing then FFT
    double benchmarkSeparateSteps() {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int trial = 0; trial < NUM_TRIALS; ++trial) {
            // Step 1: Apply window
            std::copy(input.begin(), input.end(), output.begin());
            window.multiplyWithWindowingTable(output.data(), FFT_SIZE);
            
            // Step 2: Perform FFT
            fft.performRealOnlyForwardTransform(output.data());
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / double(NUM_TRIALS);
    }
    
    // Optimized approach: fused windowing+FFT
    double benchmarkFusedApproach() {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int trial = 0; trial < NUM_TRIALS; ++trial) {
            // Fused: window coefficients applied during FFT input preparation
            const float* windowTable = window.getWindowingTable();
            for (int i = 0; i < FFT_SIZE; ++i) {
                output[i] = input[i] * windowTable[i];
            }
            
            // FFT on pre-windowed data
            fft.performRealOnlyForwardTransform(output.data());
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / double(NUM_TRIALS);
    }
    
    // Validate bit-perfect accuracy between approaches
    bool validateAccuracy() {
        std::vector<float> output1(FFT_SIZE * 2);
        std::vector<float> output2(FFT_SIZE * 2);
        
        // Separate approach
        std::copy(input.begin(), input.end(), output1.begin());
        window.multiplyWithWindowingTable(output1.data(), FFT_SIZE);
        fft.performRealOnlyForwardTransform(output1.data());
        
        // Fused approach  
        const float* windowTable = window.getWindowingTable();
        for (int i = 0; i < FFT_SIZE; ++i) {
            output2[i] = input[i] * windowTable[i];
        }
        fft.performRealOnlyForwardTransform(output2.data());
        
        // Compare results
        constexpr float tolerance = 1e-6f;
        for (int i = 0; i < FFT_SIZE; ++i) {
            if (std::abs(output1[i] - output2[i]) > tolerance) {
                return false;
            }
        }
        return true;
    }
    
    void runBenchmark() {
        std::cout << "FFT Windowing Benchmark (FFT_SIZE=" << FFT_SIZE << ")\n";
        std::cout << "==========================================\n";
        
        // Validate accuracy first
        if (!validateAccuracy()) {
            std::cout << "ERROR: Accuracy validation failed!\n";
            return;
        }
        std::cout << "✓ Accuracy validation passed\n\n";
        
        // Benchmark both approaches
        double separateTime = benchmarkSeparateSteps();
        double fusedTime = benchmarkFusedApproach();
        double improvement = ((separateTime - fusedTime) / separateTime) * 100.0;
        
        std::cout << "Separate windowing + FFT: " << separateTime << " μs/frame\n";
        std::cout << "Fused windowing + FFT:    " << fusedTime << " μs/frame\n";
        std::cout << "Performance improvement:   " << improvement << "%\n";
        
        if (improvement > 15.0) {
            std::cout << "✓ Significant improvement achieved!\n";
        } else {
            std::cout << "⚠ Improvement below 15% threshold\n";
        }
    }
    
private:
    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> window;
    std::vector<float> input;
    std::vector<float> output;
};

int main() {
    BenchWindowFFT benchmark;
    benchmark.runBenchmark();
    return 0;
}