#pragma once

#include <atomic>
#include <array>
#include <cstring>

// Lock-free single-producer single-consumer (SPSC) queue for RT-safe communication
// Based on Dmitry Vyukov's bounded SPSC queue design
template<typename T, size_t Size>
class SPSCQueue
{
    static_assert((Size & (Size - 1)) == 0, "Size must be power of 2");
    static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable for RT-safety");
    
public:
    SPSCQueue() noexcept : head_(0), tail_(0) {}
    
    // Non-copyable for RT safety
    SPSCQueue(const SPSCQueue&) = delete;
    SPSCQueue& operator=(const SPSCQueue&) = delete;
    
    // Try to push element (producer thread only)
    bool push(const T& item) noexcept
    {
        const size_t currentTail = tail_.load(std::memory_order_relaxed);
        const size_t nextTail = (currentTail + 1) & (Size - 1);
        
        if (nextTail == head_.load(std::memory_order_acquire))
            return false; // Queue is full
        
        buffer_[currentTail] = item;
        tail_.store(nextTail, std::memory_order_release);
        return true;
    }
    
    // Try to pop element (consumer thread only)
    bool pop(T& item) noexcept
    {
        const size_t currentHead = head_.load(std::memory_order_relaxed);
        
        if (currentHead == tail_.load(std::memory_order_acquire))
            return false; // Queue is empty
        
        item = buffer_[currentHead];
        head_.store((currentHead + 1) & (Size - 1), std::memory_order_release);
        return true;
    }
    
    // Check if queue has space (producer thread)
    bool hasSpaceAvailable() const noexcept
    {
        const size_t currentTail = tail_.load(std::memory_order_relaxed);
        const size_t nextTail = (currentTail + 1) & (Size - 1);
        return nextTail != head_.load(std::memory_order_acquire);
    }
    
    // Check if queue has data (consumer thread)
    bool hasDataAvailable() const noexcept
    {
        const size_t currentHead = head_.load(std::memory_order_relaxed);
        return currentHead != tail_.load(std::memory_order_acquire);
    }
    
    // Clear all data (use only when no other threads are accessing)
    void clear() noexcept
    {
        head_.store(0, std::memory_order_relaxed);
        tail_.store(0, std::memory_order_relaxed);
    }
    
    // Get approximate size (for debugging only)
    size_t approximateSize() const noexcept
    {
        const size_t currentHead = head_.load(std::memory_order_relaxed);
        const size_t currentTail = tail_.load(std::memory_order_relaxed);
        return (currentTail - currentHead) & (Size - 1);
    }

private:
    alignas(64) std::array<T, Size> buffer_;  // Cache line aligned
    alignas(64) std::atomic<size_t> head_;    // Consumer index
    alignas(64) std::atomic<size_t> tail_;    // Producer index
};

// Spectral data frame for Audio Thread → UI Thread communication
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

// Gesture data for UI Thread → GPU Thread communication
struct GestureCommand  
{
    enum Type : uint32_t
    {
        MouseDown = 0,
        MouseDrag = 1,
        MouseUp = 2,
        ClearGestures = 3,
        UpdateColormap = 4,
        SetParticleCount = 5
    };
    
    Type commandType;
    float positionX;                          // Normalized 0.0-1.0
    float positionY;                          // Normalized 0.0-1.0  
    float intensity;                          // Gesture strength 0.0-1.0
    uint32_t timestampMs;                     // Millisecond timestamp
    
    // Union for different command data
    union
    {
        struct { int colormapType; } colormapData;
        struct { int particleCount; } particleData;
        struct { float unused[2]; } padding;
    };
    
    GestureCommand() noexcept
        : commandType(MouseDown), positionX(0.0f), positionY(0.0f), 
          intensity(0.0f), timestampMs(0)
    {
        padding.unused[0] = 0.0f;
        padding.unused[1] = 0.0f;
    }
};

// Render command for UI Thread → GPU Thread communication
struct RenderCommand
{
    enum Type : uint32_t
    {
        UpdateSpectralTexture = 0,
        ProcessGestureCommand = 1,
        SetRenderSettings = 2,
        ClearRenderTarget = 3
    };
    
    Type commandType;
    uint32_t sequenceNumber;                  // For ordering
    
    union
    {
        SpectralFrame spectralData;           // For spectral texture updates
        GestureCommand gestureData;           // For gesture processing
        struct                                // For render settings
        {
            int windowWidth;
            int windowHeight;
            float timeSeconds;
        } renderSettings;
    };
    
    RenderCommand() noexcept : commandType(UpdateSpectralTexture), sequenceNumber(0) {}
};

// Type aliases for specific queue configurations
using SpectralDataQueue = SPSCQueue<SpectralFrame, 16>;      // Audio → UI
using ParameterQueue = SPSCQueue<ParameterUpdate, 64>;       // UI → Audio  
using RenderCommandQueue = SPSCQueue<RenderCommand, 32>;     // UI → GPU
using GestureCommandQueue = SPSCQueue<GestureCommand, 128>;  // UI → GPU (high frequency)

// RT-safe assertions for debug builds
#ifdef JUCE_DEBUG
    #define RT_SAFE_ASSERT(condition) juce_assert(condition)
    #define RT_SAFE_LOG(message) juce::Logger::writeToLog("[RT-SAFE] " message)
#else
    #define RT_SAFE_ASSERT(condition) ((void)0)
    #define RT_SAFE_LOG(message) ((void)0)
#endif