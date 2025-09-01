#pragma once

#include "../Core/MessageBus.h"
#include <array>
#include <atomic>

/**
 * RT-safe test harness for validating MaskColumn → SpectralEngine → audio output
 * Generates precomputed diagonal sweep patterns for audible verification
 */
class MaskTestFeeder
{
public:
    static constexpr size_t RING_CAPACITY = 32; // Must be power of 2
    static constexpr size_t NUM_TEST_PATTERNS = 16; // Diagonal sweep frames
    
    MaskTestFeeder() noexcept;
    ~MaskTestFeeder() noexcept = default;
    
    // Non-copyable for RT safety
    MaskTestFeeder(const MaskTestFeeder&) = delete;
    MaskTestFeeder& operator=(const MaskTestFeeder&) = delete;
    MaskTestFeeder(MaskTestFeeder&&) = delete;
    MaskTestFeeder& operator=(MaskTestFeeder&&) = delete;
    
    // Initialization (UI thread - may allocate)
    void initialize(double sampleRate, size_t numBins);
    void reset() noexcept;
    
    // Test pattern control (UI thread)
    void startDiagonalSweep() noexcept;
    void stop() noexcept;
    
    // RT-safe access (audio thread only)
    bool tryPopMask(MaskColumn& mask) noexcept;
    bool isActive() const noexcept { return isActive_.load(std::memory_order_acquire); }
    
    // Statistics (UI thread)
    size_t getDroppedCount() const noexcept { return droppedCount_.load(std::memory_order_relaxed); }
    void clearStats() noexcept { droppedCount_.store(0, std::memory_order_relaxed); }

private:
    // Pre-generated test patterns
    std::array<MaskColumn, NUM_TEST_PATTERNS> testPatterns_;
    
    // RT-safe ring buffer
    std::array<MaskColumn, RING_CAPACITY> ringBuffer_;
    std::atomic<size_t> readIndex_{0};
    std::atomic<size_t> writeIndex_{0};
    
    // State
    std::atomic<bool> isActive_{false};
    std::atomic<bool> isInitialized_{false};
    std::atomic<size_t> droppedCount_{0};
    
    // Configuration
    double sampleRate_ = 44100.0;
    size_t numBins_ = 257; // FFT_SIZE/2 + 1
    
    // Pattern generation
    void generateDiagonalSweep() noexcept;
    void generateTestPattern(size_t patternIndex, float binRatio) noexcept;
    bool tryPushMask(const MaskColumn& mask) noexcept;
};