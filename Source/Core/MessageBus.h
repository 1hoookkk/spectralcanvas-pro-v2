#pragma once
#include <atomic>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <cstring>
#include <cstdint>

// Phase 4 experiment flag - oscillator bank with key filter
#define PHASE4_EXPERIMENT 1

// Simple SPSC Ring Buffer (lock-free, RT-safe)
template <typename T, size_t CapacityPow2>
class SpscRing
{
public:
    static_assert((CapacityPow2 & (CapacityPow2 - 1)) == 0, "Capacity must be power of two");
    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable (POD) for RT safety");

    bool push(const T& v) noexcept
    {
        const auto w = write_.load(std::memory_order_relaxed);
        const auto n = (w + 1) & mask_;
        if (n == read_.load(std::memory_order_acquire)) {
            dropCount_.fetch_add(1, std::memory_order_relaxed);  // Track drops
            return false; // full
        }
        buf_[w] = v;
        write_.store(n, std::memory_order_release);
        pushCount_.fetch_add(1, std::memory_order_relaxed);  // Track successful pushes
        return true;
    }

    std::optional<T> pop() noexcept
    {
        const auto r = read_.load(std::memory_order_relaxed);
        if (r == write_.load(std::memory_order_acquire)) return std::nullopt; // empty
        T v = buf_[r];
        read_.store((r + 1) & mask_, std::memory_order_release);
        popCount_.fetch_add(1, std::memory_order_relaxed);  // Track successful pops
        return v;
    }

    void clear() noexcept { 
        read_.store(0, std::memory_order_relaxed); 
        write_.store(0, std::memory_order_relaxed);
        pushCount_.store(0, std::memory_order_relaxed);
        popCount_.store(0, std::memory_order_relaxed);
        dropCount_.store(0, std::memory_order_relaxed);
    }

    bool hasSpaceAvailable() const noexcept
    {
        const auto w = write_.load(std::memory_order_relaxed);
        const auto n = (w + 1) & mask_;
        return n != read_.load(std::memory_order_acquire);
    }

    bool hasDataAvailable() const noexcept
    {
        return read_.load(std::memory_order_relaxed) != write_.load(std::memory_order_acquire);
    }
    
    // Statistics accessors (safe to call from any thread)
    size_t getPushCount() const noexcept { return pushCount_.load(std::memory_order_relaxed); }
    size_t getPopCount() const noexcept { return popCount_.load(std::memory_order_relaxed); }
    size_t getDropCount() const noexcept { return dropCount_.load(std::memory_order_relaxed); }
    
    // Queue depth approximation (may be slightly stale due to relaxed ordering)
    size_t getApproxQueueDepth() const noexcept
    {
        const auto w = write_.load(std::memory_order_relaxed);
        const auto r = read_.load(std::memory_order_relaxed);
        return (w - r) & mask_;
    }

private:
    static constexpr size_t mask_ = CapacityPow2 - 1;
    T buf_[CapacityPow2]{};                   // POD buffer
    std::atomic<size_t> read_{0}, write_{0};  // true atomics, no wrappers
    
    // Statistics counters for Phase 2-3 validation
    std::atomic<size_t> pushCount_{0};        // Total successful pushes
    std::atomic<size_t> popCount_{0};         // Total successful pops  
    std::atomic<size_t> dropCount_{0};        // Total drops due to queue full
};

// Spectral data frame for Audio Thread → UI Thread communication
#if _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4324)
#endif

struct SpectralFrame
{
    static constexpr size_t FFT_SIZE = 512;
    static constexpr size_t NUM_BINS = FFT_SIZE / 2 + 1; // 257 bins for real FFT
    
    alignas(32) float magnitude[NUM_BINS];    // Spectral magnitudes
    alignas(32) float phase[NUM_BINS];        // Spectral phases
    
    uint32_t sequenceNumber;                  // Monotonic frame counter
    double timestampSamples;                  // Sample-accurate timestamp
    float fundamentalFreq;                    // Detected fundamental (0 if none)
    float spectralCentroid;                   // Brightness measure
    
    SpectralFrame() noexcept
    {
        std::memset(magnitude, 0, sizeof(magnitude));
        std::memset(phase, 0, sizeof(phase));
        sequenceNumber = 0;
        timestampSamples = 0.0;
        fundamentalFreq = 0.0f;
        spectralCentroid = 0.0f;
    }
};

#if _MSC_VER
  #pragma warning(pop)
#endif

// Parameter update for UI Thread → Audio Thread communication  
struct ParameterUpdate
{
    uint32_t parameterId;                     // Parameter identifier
    float normalizedValue;                    // 0.0 to 1.0 range
    uint32_t sampleOffset;                    // Buffer-relative timing for sample accuracy
    
    ParameterUpdate() noexcept 
        : parameterId(0), normalizedValue(0.0f), sampleOffset(0) {}
    
    ParameterUpdate(uint32_t id, float value, uint32_t offset = 0) noexcept
        : parameterId(id), normalizedValue(value), sampleOffset(offset) {}
};

// MaskColumn for GPU → Audio Thread communication
#if _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4324)
#endif

struct MaskColumn
{
    static constexpr size_t MAX_BINS = 1024; // Support up to 2048 FFT (1024 bins)
    
    alignas(32) float values[MAX_BINS];      // Mask values 0..1 
    uint32_t frameIndex;                     // FFT frame this applies to
    uint32_t numBins;                        // Actual number of bins (≤ MAX_BINS)
    double timestampSamples;                 // Sample-accurate timing
    uint64_t uiTimestampMicros;              // High-res UI timestamp for latency tracking
    uint32_t sequenceNumber;                 // For drop detection and debugging
    
    MaskColumn() noexcept
    {
        std::memset(values, 0, sizeof(values));
        frameIndex = 0;
        numBins = 0;
        timestampSamples = 0.0;
        uiTimestampMicros = 0;
        sequenceNumber = 0;
        
        // Initialize to 1.0 (no masking by default)
        for (size_t i = 0; i < MAX_BINS; ++i)
        {
            values[i] = 1.0f;
        }
    }
};

#if _MSC_VER
  #pragma warning(pop)
#endif

// Type aliases for specific queue configurations
using SpectralDataQueue = SpscRing<SpectralFrame, 16>;      // Audio → UI
using ParameterQueue = SpscRing<ParameterUpdate, 64>;       // UI → Audio  
using MaskColumnQueue = SpscRing<MaskColumn, 8>;            // GPU → Audio

// RT-safe assertions for debug builds
#ifdef JUCE_DEBUG
    #define RT_SAFE_ASSERT(condition) juce_assert(condition)
    #define RT_SAFE_LOG(message) juce::Logger::writeToLog("[RT-SAFE] " message)
#else
    #define RT_SAFE_ASSERT(condition) ((void)0)
    #define RT_SAFE_LOG(message) ((void)0)
#endif