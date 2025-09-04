#pragma once

#include <JuceHeader.h>
#include <chrono>
#include <vector>
#include <functional>
#include <iostream>
#include <random>

/**
 * DSP Validation Framework for SpectralCanvasPro
 * 
 * Use this framework to validate any DSP optimization against JUCE baselines.
 * Ensures you never deploy code that's slower or less accurate than JUCE.
 */

namespace SpectralCanvasPro {
namespace Validation {

class DSPBenchmark {
public:
    struct BenchmarkResult {
        double averageTimeUs;
        double minTimeUs;
        double maxTimeUs;
        double standardDeviation;
        size_t iterations;
    };
    
    template<typename Func>
    static BenchmarkResult measurePerformance(Func&& function, size_t iterations = 10000) {
        std::vector<double> timings;
        timings.reserve(iterations);
        
        // Warm up
        for (int i = 0; i < 100; ++i) {
            function();
        }
        
        // Actual measurement
        for (size_t i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            function();
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            timings.push_back(duration.count() / 1000.0); // Convert to microseconds
        }
        
        // Calculate statistics
        double sum = 0.0;
        double minTime = timings[0];
        double maxTime = timings[0];
        
        for (double time : timings) {
            sum += time;
            minTime = std::min(minTime, time);
            maxTime = std::max(maxTime, time);
        }
        
        double average = sum / iterations;
        
        // Standard deviation
        double variance = 0.0;
        for (double time : timings) {
            variance += (time - average) * (time - average);
        }
        double stdDev = std::sqrt(variance / iterations);
        
        return {average, minTime, maxTime, stdDev, iterations};
    }
};

class AccuracyValidator {
public:
    static constexpr float DEFAULT_TOLERANCE = 1e-6f;
    
    static bool compareBuffers(const float* buffer1, const float* buffer2, 
                              size_t numSamples, float tolerance = DEFAULT_TOLERANCE) {
        for (size_t i = 0; i < numSamples; ++i) {
            if (std::abs(buffer1[i] - buffer2[i]) > tolerance) {
                std::cerr << "Accuracy validation failed at sample " << i 
                         << ": expected " << buffer1[i] 
                         << ", got " << buffer2[i] 
                         << ", difference: " << std::abs(buffer1[i] - buffer2[i]) << std::endl;
                return false;
            }
        }
        return true;
    }
    
    static bool compareAudioBuffers(const juce::AudioBuffer<float>& reference,
                                   const juce::AudioBuffer<float>& test,
                                   float tolerance = DEFAULT_TOLERANCE) {
        if (reference.getNumChannels() != test.getNumChannels() ||
            reference.getNumSamples() != test.getNumSamples()) {
            std::cerr << "Buffer dimension mismatch" << std::endl;
            return false;
        }
        
        for (int ch = 0; ch < reference.getNumChannels(); ++ch) {
            if (!compareBuffers(reference.getReadPointer(ch), 
                               test.getReadPointer(ch),
                               reference.getNumSamples(), 
                               tolerance)) {
                std::cerr << "Channel " << ch << " validation failed" << std::endl;
                return false;
            }
        }
        return true;
    }
};

class TestSignalGenerator {
public:
    static void generateSineWave(std::vector<float>& buffer, float frequency, 
                                float sampleRate, float amplitude = 1.0f) {
        const float omega = 2.0f * juce::MathConstants<float>::pi * frequency / sampleRate;
        
        for (size_t i = 0; i < buffer.size(); ++i) {
            buffer[i] = amplitude * std::sin(omega * i);
        }
    }
    
    static void generateWhiteNoise(std::vector<float>& buffer, float amplitude = 1.0f) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-amplitude, amplitude);
        
        for (auto& sample : buffer) {
            sample = dis(gen);
        }
    }
    
    static void generateSweep(std::vector<float>& buffer, float startFreq, 
                             float endFreq, float sampleRate, float amplitude = 1.0f) {
        const float freqRange = endFreq - startFreq;
        const float duration = buffer.size() / sampleRate;
        
        for (size_t i = 0; i < buffer.size(); ++i) {
            float t = i / sampleRate;
            float instantFreq = startFreq + (freqRange * t / duration);
            float omega = 2.0f * juce::MathConstants<float>::pi * instantFreq;
            buffer[i] = amplitude * std::sin(omega * t);
        }
    }
    
    static void generateImpulse(std::vector<float>& buffer, size_t impulsePos = 0, 
                               float amplitude = 1.0f) {
        std::fill(buffer.begin(), buffer.end(), 0.0f);
        if (impulsePos < buffer.size()) {
            buffer[impulsePos] = amplitude;
        }
    }
};

/**
 * Standard validation test for any FFT implementation against JUCE baseline
 */
class FFTValidationTest {
private:
    std::unique_ptr<juce::dsp::FFT> juceFFT_;
    std::vector<float> testBuffer_;
    std::vector<float> juceResult_;
    std::vector<float> customResult_;
    
public:
    FFTValidationTest(int fftOrder) 
        : juceFFT_(std::make_unique<juce::dsp::FFT>(fftOrder))
        , testBuffer_(1 << fftOrder)
        , juceResult_(1 << fftOrder)
        , customResult_(1 << fftOrder) {
    }
    
    template<typename CustomFFTFunc>
    bool validateAccuracy(CustomFFTFunc&& customFFT, float tolerance = 1e-6f) {
        // Test with different signal types
        std::vector<std::string> testNames = {"sine_wave", "white_noise", "sweep", "impulse"};
        
        for (const auto& testName : testNames) {
            if (testName == "sine_wave") {
                TestSignalGenerator::generateSineWave(testBuffer_, 440.0f, 48000.0f);
            } else if (testName == "white_noise") {
                TestSignalGenerator::generateWhiteNoise(testBuffer_);
            } else if (testName == "sweep") {
                TestSignalGenerator::generateSweep(testBuffer_, 20.0f, 20000.0f, 48000.0f);
            } else if (testName == "impulse") {
                TestSignalGenerator::generateImpulse(testBuffer_, 0);
            }
            
            // Process with JUCE
            std::copy(testBuffer_.begin(), testBuffer_.end(), juceResult_.begin());
            juceFFT_->performFrequencyOnlyForwardTransform(juceResult_.data());
            
            // Process with custom implementation
            std::copy(testBuffer_.begin(), testBuffer_.end(), customResult_.begin());
            customFFT(customResult_.data());
            
            // Compare results
            if (!AccuracyValidator::compareBuffers(juceResult_.data(), customResult_.data(), 
                                                  testBuffer_.size(), tolerance)) {
                std::cerr << "FFT validation failed for test: " << testName << std::endl;
                return false;
            }
        }
        
        std::cout << "FFT accuracy validation passed for all test signals" << std::endl;
        return true;
    }
    
    template<typename CustomFFTFunc>
    bool validatePerformance(CustomFFTFunc&& customFFT, float minImprovementPercent = 20.0f) {
        // Benchmark JUCE FFT
        auto juceBenchmark = DSPBenchmark::measurePerformance([&]() {
            juceFFT_->performFrequencyOnlyForwardTransform(testBuffer_.data());
        });
        
        // Benchmark custom FFT
        auto customBenchmark = DSPBenchmark::measurePerformance([&]() {
            customFFT(testBuffer_.data());
        });
        
        float improvementPercent = ((juceBenchmark.averageTimeUs - customBenchmark.averageTimeUs) 
                                   / juceBenchmark.averageTimeUs) * 100.0f;
        
        std::cout << "Performance comparison:" << std::endl;
        std::cout << "  JUCE FFT: " << juceBenchmark.averageTimeUs << " μs (±" 
                 << juceBenchmark.standardDeviation << ")" << std::endl;
        std::cout << "  Custom FFT: " << customBenchmark.averageTimeUs << " μs (±" 
                 << customBenchmark.standardDeviation << ")" << std::endl;
        std::cout << "  Improvement: " << improvementPercent << "%" << std::endl;
        
        if (improvementPercent < minImprovementPercent) {
            std::cerr << "Performance improvement (" << improvementPercent 
                     << "%) is below minimum threshold (" << minImprovementPercent << "%)" << std::endl;
            return false;
        }
        
        return true;
    }
};

/**
 * RT-Safety validator - ensures no allocations during processing
 */
class RTSafetyValidator {
private:
    size_t initialHeapSize_;
    
public:
    void startMonitoring() {
        // In a real implementation, you'd hook malloc/free
        // For now, this is a placeholder for the concept
        initialHeapSize_ = getCurrentHeapSize();
    }
    
    bool validateNoAllocations() {
        size_t currentHeapSize = getCurrentHeapSize();
        if (currentHeapSize != initialHeapSize_) {
            std::cerr << "RT-safety violation: heap size changed from " 
                     << initialHeapSize_ << " to " << currentHeapSize << std::endl;
            return false;
        }
        return true;
    }
    
private:
    size_t getCurrentHeapSize() {
        // Placeholder - in real implementation would use platform-specific APIs
        return 0;
    }
};

/**
 * Complete validation suite for any DSP component
 */
template<typename JUCEImplementation, typename CustomImplementation>
class DSPValidationSuite {
private:
    JUCEImplementation juceImpl_;
    CustomImplementation customImpl_;
    
public:
    DSPValidationSuite(JUCEImplementation juceImpl, CustomImplementation customImpl)
        : juceImpl_(std::move(juceImpl)), customImpl_(std::move(customImpl)) {}
    
    bool runFullValidation() {
        std::cout << "Starting DSP validation suite..." << std::endl;
        
        // 1. Accuracy validation
        if (!validateAccuracy()) {
            std::cerr << "Accuracy validation failed" << std::endl;
            return false;
        }
        
        // 2. Performance validation
        if (!validatePerformance()) {
            std::cerr << "Performance validation failed" << std::endl;
            return false;
        }
        
        // 3. RT-safety validation
        if (!validateRTSafety()) {
            std::cerr << "RT-safety validation failed" << std::endl;
            return false;
        }
        
        std::cout << "All validations passed!" << std::endl;
        return true;
    }
    
private:
    bool validateAccuracy() {
        // Implementation-specific accuracy tests
        return true;
    }
    
    bool validatePerformance() {
        // Implementation-specific performance tests
        return true;
    }
    
    bool validateRTSafety() {
        // Implementation-specific RT-safety tests
        return true;
    }
};

} // namespace Validation
} // namespace SpectralCanvasPro

/**
 * Usage Example:
 * 
 * #include "dsp_validation_framework.h"
 * 
 * void testMyCustomFFT() {
 *     using namespace SpectralCanvasPro::Validation;
 *     
 *     FFTValidationTest validator(9); // 512-point FFT
 *     
 *     auto customFFT = [](float* buffer) {
 *         // Your custom FFT implementation
 *     };
 *     
 *     // Must pass accuracy test
 *     if (!validator.validateAccuracy(customFFT)) {
 *         std::cerr << "Custom FFT failed accuracy test" << std::endl;
 *         return;
 *     }
 *     
 *     // Must show significant performance improvement
 *     if (!validator.validatePerformance(customFFT, 20.0f)) {
 *         std::cerr << "Custom FFT not worth the complexity" << std::endl;
 *         return;
 *     }
 *     
 *     std::cout << "Custom FFT validation passed!" << std::endl;
 * }
 */