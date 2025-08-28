#include "MaskTestFeeder.h"
#include <algorithm>
#include <cmath>

MaskTestFeeder::MaskTestFeeder() noexcept
{
    // Ring buffer indices already initialized to 0
    // Test patterns will be generated in initialize()
}

void MaskTestFeeder::initialize(double sampleRate, size_t numBins)
{
    sampleRate_ = sampleRate;
    numBins_ = std::min(numBins, static_cast<size_t>(MaskColumn::MAX_BINS));
    
    // Generate diagonal sweep test patterns
    generateDiagonalSweep();
    
    // Reset ring buffer state
    reset();
    
    isInitialized_.store(true, std::memory_order_release);
}

void MaskTestFeeder::reset() noexcept
{
    readIndex_.store(0, std::memory_order_relaxed);
    writeIndex_.store(0, std::memory_order_relaxed);
    isActive_.store(false, std::memory_order_relaxed);
    droppedCount_.store(0, std::memory_order_relaxed);
}

void MaskTestFeeder::startDiagonalSweep() noexcept
{
    if (!isInitialized_.load(std::memory_order_acquire))
        return;
    
    // Feed all test patterns into ring buffer
    for (size_t i = 0; i < NUM_TEST_PATTERNS; ++i)
    {
        if (!tryPushMask(testPatterns_[i]))
        {
            // Ring buffer full - increment drop counter
            droppedCount_.fetch_add(1, std::memory_order_relaxed);
        }
    }
    
    isActive_.store(true, std::memory_order_release);
}

void MaskTestFeeder::stop() noexcept
{
    isActive_.store(false, std::memory_order_relaxed);
}

bool MaskTestFeeder::tryPopMask(MaskColumn& mask) noexcept
{
    const size_t currentRead = readIndex_.load(std::memory_order_relaxed);
    const size_t currentWrite = writeIndex_.load(std::memory_order_acquire);
    
    // Check if ring buffer is empty
    if (currentRead == currentWrite)
        return false;
    
    // Copy mask data
    mask = ringBuffer_[currentRead];
    
    // Advance read index with wraparound
    const size_t nextRead = (currentRead + 1) & (RING_CAPACITY - 1);
    readIndex_.store(nextRead, std::memory_order_release);
    
    return true;
}

void MaskTestFeeder::generateDiagonalSweep() noexcept
{
    for (size_t i = 0; i < NUM_TEST_PATTERNS; ++i)
    {
        // Create diagonal sweep from low to high frequencies
        const float binRatio = static_cast<float>(i) / static_cast<float>(NUM_TEST_PATTERNS - 1);
        generateTestPattern(i, binRatio);
    }
}

void MaskTestFeeder::generateTestPattern(size_t patternIndex, float binRatio) noexcept
{
    auto& pattern = testPatterns_[patternIndex];
    
    // Initialize mask column
    pattern.numBins = static_cast<uint32_t>(numBins_);
    pattern.frameIndex = static_cast<uint32_t>(patternIndex);
    pattern.timestampSamples = patternIndex * 512.0; // Assume hop size of 512
    
    // Initialize all bins to baseline (no effect)
    for (size_t bin = 0; bin < numBins_; ++bin)
    {
        pattern.values[bin] = 1.0f;
    }
    
    // Create focused frequency emphasis
    const size_t centerBin = static_cast<size_t>(binRatio * (numBins_ - 1));
    const size_t bandwidth = std::max(size_t(1), numBins_ / 32); // Narrow band
    
    // Apply Gaussian-like emphasis around center frequency
    for (size_t bin = 0; bin < numBins_; ++bin)
    {
        const float distance = static_cast<float>(static_cast<int>(bin) - static_cast<int>(centerBin));
        const float normalizedDistance = distance / static_cast<float>(bandwidth);
        
        // Gaussian falloff: exp(-x²/2)
        const float emphasis = std::exp(-0.5f * normalizedDistance * normalizedDistance);
        
        // Scale from 1.0 (no effect) to 3.0 (3x emphasis)
        pattern.values[bin] = 1.0f + 2.0f * emphasis;
    }
    
    // Ensure bins beyond numBins are set to no-effect
    for (size_t bin = numBins_; bin < MaskColumn::MAX_BINS; ++bin)
    {
        pattern.values[bin] = 1.0f;
    }
}

bool MaskTestFeeder::tryPushMask(const MaskColumn& mask) noexcept
{
    const size_t currentWrite = writeIndex_.load(std::memory_order_relaxed);
    const size_t nextWrite = (currentWrite + 1) & (RING_CAPACITY - 1);
    
    // Check if ring buffer is full
    if (nextWrite == readIndex_.load(std::memory_order_acquire))
        return false;
    
    // Copy mask data
    ringBuffer_[currentWrite] = mask;
    
    // Advance write index
    writeIndex_.store(nextWrite, std::memory_order_release);
    
    return true;
}