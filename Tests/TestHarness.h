#pragma once

#include <chrono>
#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <atomic>
#include <thread>
#include <functional>

namespace test {

// High-resolution timing for performance tests
class HiResTimer {
public:
    HiResTimer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    void reset() { start_ = std::chrono::high_resolution_clock::now(); }
    
    double elapsedMs() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_);
        return duration.count() / 1000000.0;
    }
    
    double elapsedUs() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_);
        return duration.count() / 1000.0;
    }

private:
    std::chrono::high_resolution_clock::time_point start_;
};

// Statistical analysis for performance data
class PerfStats {
public:
    void addSample(double value) {
        samples_.push_back(value);
        sum_ += value;
        if (value < min_) min_ = value;
        if (value > max_) max_ = value;
    }
    
    double mean() const {
        return samples_.empty() ? 0.0 : sum_ / static_cast<double>(samples_.size());
    }
    
    double min() const { return min_; }
    double max() const { return max_; }
    
    double percentile(double p) const {
        if (samples_.empty()) return 0.0;
        
        auto sorted = samples_;
        std::sort(sorted.begin(), sorted.end());
        
        size_t index = static_cast<size_t>(p * (sorted.size() - 1));
        return sorted[index];
    }
    
    size_t count() const { return samples_.size(); }
    
    void print(const std::string& name) const {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << name << " stats (n=" << count() << "):\n";
        std::cout << "  Mean: " << mean() << " ms\n";
        std::cout << "  Min:  " << min() << " ms\n"; 
        std::cout << "  Max:  " << max() << " ms\n";
        std::cout << "  P50:  " << percentile(0.5) << " ms\n";
        std::cout << "  P95:  " << percentile(0.95) << " ms\n";
        std::cout << "  P99:  " << percentile(0.99) << " ms\n";
    }

private:
    std::vector<double> samples_;
    double sum_ = 0.0;
    double min_ = std::numeric_limits<double>::max();
    double max_ = std::numeric_limits<double>::min();
};

// Simple test assertion macros
#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "ASSERTION FAILED: " << #condition \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(0)

#define TEST_EXPECT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::cerr << "EXPECTATION FAILED: expected " << (expected) \
                      << ", got " << (actual) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(0)

#define TEST_EXPECT_LT(value, limit) \
    do { \
        if ((value) >= (limit)) { \
            std::cerr << "EXPECTATION FAILED: " << (value) \
                      << " should be < " << (limit) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(0)

// Test runner infrastructure
class TestCase {
public:
    TestCase(const std::string& name) : name_(name) {}
    virtual ~TestCase() = default;
    
    virtual void run() = 0;
    
    const std::string& name() const { return name_; }

private:
    std::string name_;
};

class TestRunner {
public:
    void addTest(std::unique_ptr<TestCase> test) {
        tests_.push_back(std::move(test));
    }
    
    int runAll() {
        int failures = 0;
        
        std::cout << "Running " << tests_.size() << " test(s)...\n\n";
        
        for (auto& test : tests_) {
            std::cout << "[ RUN      ] " << test->name() << std::endl;
            
            try {
                HiResTimer timer;
                test->run();
                double elapsed = timer.elapsedMs();
                std::cout << "[       OK ] " << test->name() 
                          << " (" << std::fixed << std::setprecision(1) 
                          << elapsed << " ms)" << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "[  FAILED  ] " << test->name() 
                          << " - " << e.what() << std::endl;
                failures++;
            }
            catch (...) {
                std::cout << "[  FAILED  ] " << test->name() 
                          << " - Unknown exception" << std::endl;
                failures++;
            }
        }
        
        std::cout << "\n";
        if (failures == 0) {
            std::cout << "All tests passed!" << std::endl;
        } else {
            std::cout << failures << " test(s) failed." << std::endl;
        }
        
        return failures;
    }

private:
    std::vector<std::unique_ptr<TestCase>> tests_;
};

// Mock audio buffer for testing
class MockAudioBuffer {
public:
    MockAudioBuffer(int channels, int samples) 
        : channels_(channels), samples_(samples) {
        data_.resize(channels * samples, 0.0f);
    }
    
    float* getWritePointer(int channel) {
        assert(channel < channels_);
        return &data_[channel * samples_];
    }
    
    const float* getReadPointer(int channel) const {
        assert(channel < channels_);
        return &data_[channel * samples_];
    }
    
    int getNumChannels() const { return channels_; }
    int getNumSamples() const { return samples_; }
    
    void clear() {
        std::fill(data_.begin(), data_.end(), 0.0f);
    }
    
    void fillWith(float value) {
        std::fill(data_.begin(), data_.end(), value);
    }
    
    float getRMSLevel(int channel = 0) const {
        const float* ptr = getReadPointer(channel);
        double sum = 0.0;
        for (int i = 0; i < samples_; ++i) {
            sum += ptr[i] * ptr[i];
        }
        return static_cast<float>(std::sqrt(sum / samples_));
    }

private:
    std::vector<float> data_;
    int channels_;
    int samples_;
};

// Thread safety testing utilities
class ThreadBarrier {
public:
    explicit ThreadBarrier(int count) : count_(count), waiting_(0), generation_(0) {}
    
    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        int gen = generation_;
        
        if (++waiting_ == count_) {
            generation_++;
            waiting_ = 0;
            cv_.notify_all();
        } else {
            cv_.wait(lock, [this, gen] { return gen != generation_; });
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
    int waiting_;
    int generation_;
};

} // namespace test