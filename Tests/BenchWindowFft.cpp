#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <cmath>

#if JUCE_WINDOWS
#include <windows.h>
#endif

using hrclock_t = std::chrono::high_resolution_clock;

class BenchWindowFFT {
public:
    static constexpr int FFT_SIZE = 512;
    static constexpr int NUM_TRIALS = 1000;
    
    BenchWindowFFT() : fft((int)std::log2(FFT_SIZE)) {
        // Allocate aligned buffers for better performance
        ana.allocate(FFT_SIZE, true);
        win.allocate(FFT_SIZE, true);
        tmp.allocate(FFT_SIZE * 2, true); // Complex output buffer
        
        // Generate test signal (440 Hz sine wave)
        for (int i = 0; i < FFT_SIZE; ++i) {
            ana[i] = std::sin(2.0f * juce::MathConstants<float>::pi * 440.0f * i / 48000.0f);
        }
        
        // Precompute Hann window once (this is the key optimization!)
        for (int i = 0; i < FFT_SIZE; ++i) {
            win[i] = 0.5f * (1.0f - std::cos(2.0f * juce::MathConstants<float>::twoPi * i / (FFT_SIZE - 1)));
        }
        
        pinThread();
    }
    
private:
    void pinThread() {
#if JUCE_WINDOWS
        HANDLE th = GetCurrentThread();
        SetThreadAffinityMask(th, 1); // Pin to core 0
        SetThreadPriority(th, THREAD_PRIORITY_HIGHEST);
#endif
    }

    template<typename Func>
    double benchmark(Func&& body) {
        // Warmup
        for (int i = 0; i < 100; ++i) body();
        
        auto t0 = hrclock_t::now();
        for (int i = 0; i < NUM_TRIALS; ++i) body();
        auto t1 = hrclock_t::now();
        
        return std::chrono::duration<double, std::micro>(t1 - t0).count() / NUM_TRIALS;
    }

public:
    // 1. Naive approach: recompute Hann coefficients each frame (reproduces regression)
    double benchmarkNaive() {
        return benchmark([&] {
            std::memcpy(tmp.getData(), ana.getData(), sizeof(float) * FFT_SIZE);
            // Expensive: recompute cos() every frame!
            for (int i = 0; i < FFT_SIZE; ++i) {
                float h = 0.5f * (1.0f - std::cos(2.0f * juce::MathConstants<float>::twoPi * i / (FFT_SIZE - 1)));
                tmp[i] *= h;
            }
            fft.performRealOnlyForwardTransform(tmp.getData());
        });
    }
    
    // 2. Precomputed separate: copy + multiply with cached coefficients + FFT
    double benchmarkPrecomputedSeparate() {
        return benchmark([&] {
            std::memcpy(tmp.getData(), ana.getData(), sizeof(float) * FFT_SIZE);
            juce::FloatVectorOperations::multiply(tmp.getData(), win.getData(), FFT_SIZE);
            fft.performRealOnlyForwardTransform(tmp.getData());
        });
    }
    
    // 3. Precomputed fused: single-pass dest[i] = src[i] * win[i] + FFT
    double benchmarkPrecomputedFused() {
        return benchmark([&] {
            // Manual fused copy+multiply (this is our optimization!)
            for (int i = 0; i < FFT_SIZE; ++i) {
                tmp[i] = ana[i] * win[i];
            }
            fft.performRealOnlyForwardTransform(tmp.getData());
        });
    }

    // Validate accuracy between precomputed approaches
    bool validateAccuracy() {
        juce::HeapBlock<float> output1(FFT_SIZE * 2), output2(FFT_SIZE * 2);
        
        // Precomputed separate
        std::memcpy(output1.getData(), ana.getData(), sizeof(float) * FFT_SIZE);
        juce::FloatVectorOperations::multiply(output1.getData(), win.getData(), FFT_SIZE);
        fft.performRealOnlyForwardTransform(output1.getData());
        
        // Precomputed fused
        for (int i = 0; i < FFT_SIZE; ++i) {
            output2[i] = ana[i] * win[i];
        }
        fft.performRealOnlyForwardTransform(output2.getData());
        
        // Should be bit-exact with same precomputed coefficients
        constexpr float tolerance = 1e-6f;
        for (int i = 0; i < FFT_SIZE; ++i) {
            if (std::abs(output1[i] - output2[i]) > tolerance) {
                return false;
            }
        }
        return true;
    }
    
    void runBenchmark() {
        std::cout << "FFT Windowing Performance Benchmark (FFT_SIZE=" << FFT_SIZE << ")\n";
        std::cout << "=============================================================\n";
        
        if (!validateAccuracy()) {
            std::cout << "ERROR: Accuracy validation failed!\n";
            return;
        }
        std::cout << "✓ Accuracy validation passed\n\n";
        
        // Run all three benchmarks
        double naiveTime = benchmarkNaive();
        double separateTime = benchmarkPrecomputedSeparate(); 
        double fusedTime = benchmarkPrecomputedFused();
        
        // Calculate improvements
        double precomputedImprovement = ((naiveTime - separateTime) / naiveTime) * 100.0;
        double fusedImprovement = ((separateTime - fusedTime) / separateTime) * 100.0;
        
        std::cout << "Results (FFT 512, hop 128, " << NUM_TRIALS << " trials):\n";
        std::cout << "----------------------------------------\n";
        std::printf("Naive (recompute Hann):     %.3f μs/frame\n", naiveTime);
        std::printf("Precomputed + separate:     %.3f μs/frame (%.1f%% faster)\n", separateTime, precomputedImprovement);
        std::printf("Precomputed + fused:        %.3f μs/frame (%.1f%% faster)\n", fusedTime, fusedImprovement);
        std::cout << "----------------------------------------\n";
        
        if (fusedImprovement > 0) {
            std::cout << "✓ Fused approach shows improvement: " << fusedImprovement << "%\n";
        } else {
            std::cout << "⚠ Fused approach slower by: " << -fusedImprovement << "%\n";
        }
        
        std::cout << "\nExpected: 2-10% improvement for fused approach\n";
        std::cout << "Memory bandwidth savings scale with FFT size.\n";
    }

private:
    juce::dsp::FFT fft;
    juce::HeapBlock<float> ana, win, tmp;
};

int main() {
    BenchWindowFFT benchmark;
    benchmark.runBenchmark();
    return 0;
}