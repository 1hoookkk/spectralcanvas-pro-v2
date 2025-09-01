#pragma once
#include <juce_core/juce_core.h>
#include <array>
#include <atomic>
#include <memory>
#include <type_traits>
#include <optional>

/**
 * Real-Time Memory Management System
 * 
 * Zero real-time allocation infrastructure:
 * - Pre-allocated memory pools for all audio objects
 * - Lock-free allocation/deallocation
 * - RAII-style memory management
 * - Performance monitoring and leak detection
 */

struct SampleView
{
    const float* const* channels = nullptr;
    uint32_t numChannels = 0;
    uint32_t numFrames = 0;
    double sampleRate = 0.0;
    
    SampleView() = default;
    SampleView(const float* const* ch, uint32_t nCh, uint32_t nF, double sr)
        : channels(ch), numChannels(nCh), numFrames(nF), sampleRate(sr) {}
};

static_assert(std::is_trivially_copyable_v<SampleView>, "SampleView must be trivially copyable");

class SamplePool
{
public:
    static constexpr uint32_t kSlots = 8;

private:
    struct Slot
    {
        enum : uint32_t { Empty = 0, Filling = 1, Ready = 2 };
        std::atomic<uint32_t> state{Empty};
        std::atomic<uint32_t> generation{1};
        std::atomic<SampleView> view{SampleView{}};
        
        Slot() = default;
    };
    
    std::array<Slot, kSlots> slots_{};

public:
    
    // 32-bit handle: [ gen:24 | idx:8 ]
    static constexpr uint32_t kIdxMask = 0xFFu;
    static inline uint8_t idxOf(uint32_t h) { return static_cast<uint8_t>(h & kIdxMask); }
    static inline uint32_t genOf(uint32_t h) { return h >> 8; }
    static inline uint32_t makeHandle(uint8_t idx, uint32_t gen) { return (gen << 8) | idx; }
    
    std::optional<uint32_t> allocate() noexcept
    {
        for (uint8_t i = 0; i < kSlots; ++i)
        {
            uint32_t expected = Slot::Empty;
            if (slots_[i].state.compare_exchange_strong(expected, Slot::Filling, std::memory_order_acq_rel))
            {
                const uint32_t h = makeHandle(i, slots_[i].generation.load(std::memory_order_relaxed));
                return h;
            }
        }
        return std::nullopt;
    }
    
    bool publish(uint32_t handle, const SampleView& view) noexcept
    {
        const uint8_t i = idxOf(handle);
        if (i >= kSlots) return false;
        Slot& s = slots_[i];
        if (genOf(handle) != s.generation.load(std::memory_order_acquire)) return false;
        uint32_t expected = Slot::Filling;
        if (!s.state.compare_exchange_strong(expected, Slot::Filling, std::memory_order_acq_rel)) return false;
        
        s.view.store(view, std::memory_order_relaxed);
        s.state.store(Slot::Ready, std::memory_order_release);
        return true;
    }
    
    bool free(uint32_t handle) noexcept
    {
        const uint8_t i = idxOf(handle);
        if (i >= kSlots) return false;
        Slot& s = slots_[i];
        
        if (genOf(handle) != s.generation.load(std::memory_order_acquire)) return false;
        uint32_t st = s.state.load(std::memory_order_acquire);
        if (st != Slot::Ready && st != Slot::Filling) return false;
        
        s.state.store(Slot::Empty, std::memory_order_release);
        s.generation.fetch_add(1, std::memory_order_acq_rel);
        return true;
    }
    
    std::optional<SampleView> lookup(uint32_t handle) const noexcept
    {
        const uint8_t i = idxOf(handle);
        if (i >= kSlots) return std::nullopt;
        const Slot& s = slots_[i];
        
        if (s.state.load(std::memory_order_acquire) != Slot::Ready) return std::nullopt;
        if (genOf(handle) != s.generation.load(std::memory_order_acquire)) return std::nullopt;
        
        return s.view.load(std::memory_order_relaxed);
    }
    
    struct Stats { uint32_t ready = 0, filling = 0, empty = 0; };
    Stats stats() const noexcept
    {
        Stats st;
        for (uint8_t i = 0; i < kSlots; ++i)
        {
            switch (slots_[i].state.load(std::memory_order_relaxed))
            {
                case Slot::Ready:   ++st.ready; break;
                case Slot::Filling: ++st.filling; break;
                default:            ++st.empty; break;
            }
        }
        return st;
    }
};

struct RealtimeMemoryStats
{
    std::atomic<size_t> totalAllocatedBytes{0};
    std::atomic<size_t> peakMemoryUsage{0};
    std::atomic<uint64_t> allocationCount{0};
    std::atomic<uint64_t> deallocationCount{0};
    std::atomic<uint64_t> failedAllocations{0};
    
    RealtimeMemoryStats(const RealtimeMemoryStats&) = delete;
    RealtimeMemoryStats& operator=(const RealtimeMemoryStats&) = delete;
    RealtimeMemoryStats() = default;
    
    void recordAllocation(size_t bytes) noexcept
    {
        totalAllocatedBytes.fetch_add(bytes, std::memory_order_relaxed);
        allocationCount.fetch_add(1, std::memory_order_relaxed);
        
        size_t current = totalAllocatedBytes.load(std::memory_order_relaxed);
        size_t peak = peakMemoryUsage.load(std::memory_order_relaxed);
        while (current > peak && !peakMemoryUsage.compare_exchange_weak(peak, current, std::memory_order_relaxed))
        {
            peak = peakMemoryUsage.load(std::memory_order_relaxed);
        }
    }
    
    void recordDeallocation(size_t bytes) noexcept
    {
        totalAllocatedBytes.fetch_sub(bytes, std::memory_order_relaxed);
        deallocationCount.fetch_add(1, std::memory_order_relaxed);
    }
    
    void recordFailedAllocation() noexcept
    {
        failedAllocations.fetch_add(1, std::memory_order_relaxed);
    }
};

template<typename T, size_t PoolSize = 256>
class RealtimeObjectPool
{
public:
    RealtimeObjectPool()
    {
        for (size_t i = 0; i < PoolSize; ++i)
        {
            freeList_[i] = i;
        }
        nextFreeIndex_.store(0, std::memory_order_relaxed);
    }
    
    T* acquire() noexcept
    {
        int freeIndex = nextFreeIndex_.fetch_add(1, std::memory_order_acquire);
        if (freeIndex >= static_cast<int>(PoolSize))
        {
            nextFreeIndex_.fetch_sub(1, std::memory_order_acq_rel);
            stats_.recordFailedAllocation();
            return nullptr;
        }
        
        size_t objectIndex = freeList_[freeIndex];
        T* object = &pool_[objectIndex];
        
        new (object) T();
        
        stats_.recordAllocation(sizeof(T));
        return object;
    }
    
    void release(T* object) noexcept
    {
        if (!object) return;
        
        size_t objectIndex = object - pool_.data();
        if (objectIndex >= PoolSize) return;
        
        object->~T();
        
        int freeIndex = nextFreeIndex_.fetch_sub(1, std::memory_order_acq_rel) - 1;
        if (freeIndex >= 0)
        {
            freeList_[freeIndex] = objectIndex;
        }
        
        stats_.recordDeallocation(sizeof(T));
    }
    
    class ScopedObject
    {
    public:
        ScopedObject(RealtimeObjectPool& pool) noexcept
            : pool_(pool), object_(pool.acquire()) {}
        
        ~ScopedObject() noexcept
        {
            if (object_) pool_.release(object_);
        }
        
        T* get() const noexcept { return object_; }
        T* operator->() const noexcept { return object_; }
        T& operator*() const noexcept { return *object_; }
        explicit operator bool() const noexcept { return object_ != nullptr; }
        
        ScopedObject(const ScopedObject&) = delete;
        ScopedObject& operator=(const ScopedObject&) = delete;
        
        ScopedObject(ScopedObject&& other) noexcept
            : pool_(other.pool_), object_(other.object_)
        {
            other.object_ = nullptr;
        }
        
        ScopedObject& operator=(ScopedObject&& other) noexcept
        {
            if (this != &other)
            {
                if (object_) pool_.release(object_);
                object_ = other.object_;
                other.object_ = nullptr;
            }
            return *this;
        }
        
    private:
        RealtimeObjectPool& pool_;
        T* object_;
    };
    
    const RealtimeMemoryStats& getStats() const noexcept { return stats_; }
    
    float getUtilization() const noexcept
    {
        int used = nextFreeIndex_.load(std::memory_order_relaxed);
        return (float)std::max(0, used) / (float)PoolSize;
    }
    
private:
    alignas(64) std::array<T, PoolSize> pool_;
    std::array<size_t, PoolSize> freeList_;
    std::atomic<int> nextFreeIndex_{0};
    RealtimeMemoryStats stats_;
};

template<size_t BufferSize = 4096, size_t NumBuffers = 32>
class AudioBufferPool
{
public:
    struct AudioBufferWrapper
    {
        std::array<float, BufferSize> data;
        std::atomic<bool> inUse{false};
        
        float* getData() noexcept { return data.data(); }
        const float* getData() const noexcept { return data.data(); }
        size_t getSize() const noexcept { return BufferSize; }
        void clear() noexcept { data.fill(0.0f); }
    };
    
    AudioBufferWrapper* acquireBuffer() noexcept
    {
        for (size_t i = 0; i < NumBuffers; ++i)
        {
            bool expected = false;
            if (buffers_[i].inUse.compare_exchange_weak(expected, true, std::memory_order_acquire))
            {
                buffers_[i].clear();
                stats_.recordAllocation(BufferSize * sizeof(float));
                return &buffers_[i];
            }
        }
        
        stats_.recordFailedAllocation();
        return nullptr;
    }
    
    void releaseBuffer(AudioBufferWrapper* buffer) noexcept
    {
        if (!buffer) return;
        
        size_t index = buffer - buffers_.data();
        if (index >= NumBuffers) return;
        
        buffer->inUse.store(false, std::memory_order_release);
        stats_.recordDeallocation(BufferSize * sizeof(float));
    }
    
    class ScopedBuffer
    {
    public:
        ScopedBuffer(AudioBufferPool& pool) noexcept
            : pool_(pool), buffer_(pool.acquireBuffer()) {}
        
        ~ScopedBuffer() noexcept
        {
            if (buffer_) pool_.releaseBuffer(buffer_);
        }
        
        AudioBufferWrapper* get() const noexcept { return buffer_; }
        AudioBufferWrapper* operator->() const noexcept { return buffer_; }
        AudioBufferWrapper& operator*() const noexcept { return *buffer_; }
        explicit operator bool() const noexcept { return buffer_ != nullptr; }
        
        ScopedBuffer(const ScopedBuffer&) = delete;
        ScopedBuffer& operator=(const ScopedBuffer&) = delete;
        
        ScopedBuffer(ScopedBuffer&& other) noexcept
            : pool_(other.pool_), buffer_(other.buffer_)
        {
            other.buffer_ = nullptr;
        }
        
        ScopedBuffer& operator=(ScopedBuffer&& other) noexcept
        {
            if (this != &other)
            {
                if (buffer_) pool_.releaseBuffer(buffer_);
                buffer_ = other.buffer_;
                other.buffer_ = nullptr;
            }
            return *this;
        }
        
    private:
        AudioBufferPool& pool_;
        AudioBufferWrapper* buffer_;
    };
    
    const RealtimeMemoryStats& getStats() const noexcept { return stats_; }
    
    float getUtilization() const noexcept
    {
        size_t used = 0;
        for (const auto& buffer : buffers_)
        {
            if (buffer.inUse.load(std::memory_order_relaxed)) used++;
        }
        return (float)used / (float)NumBuffers;
    }
    
private:
    std::array<AudioBufferWrapper, NumBuffers> buffers_;
    RealtimeMemoryStats stats_;
};

class RealtimeProfiler
{
public:
    struct ProfileEntry
    {
        std::atomic<uint64_t> totalCalls{0};
        std::atomic<uint64_t> totalTime{0};
        std::atomic<uint64_t> maxTime{0};
        char name[64] = {};
        
        ProfileEntry(const ProfileEntry&) = delete;
        ProfileEntry& operator=(const ProfileEntry&) = delete;
        ProfileEntry() = default;
        
        void recordTiming(uint64_t microseconds) noexcept
        {
            totalCalls.fetch_add(1, std::memory_order_relaxed);
            totalTime.fetch_add(microseconds, std::memory_order_relaxed);
            
            uint64_t currentMax = maxTime.load(std::memory_order_relaxed);
            while (microseconds > currentMax && 
                   !maxTime.compare_exchange_weak(currentMax, microseconds, std::memory_order_relaxed))
            {
                currentMax = maxTime.load(std::memory_order_relaxed);
            }
        }
        
        double getAverageTime() const noexcept
        {
            uint64_t calls = totalCalls.load(std::memory_order_relaxed);
            if (calls == 0) return 0.0;
            return (double)totalTime.load(std::memory_order_relaxed) / (double)calls;
        }
    };
    
    class ScopedTimer
    {
    public:
        ScopedTimer(ProfileEntry& entry) noexcept
            : entry_(entry), startTime_(juce::Time::getHighResolutionTicks()) {}
        
        ~ScopedTimer() noexcept
        {
            auto endTime = juce::Time::getHighResolutionTicks();
            auto elapsed = juce::Time::highResolutionTicksToSeconds(endTime - startTime_) * 1000000.0;
            entry_.recordTiming(static_cast<uint64_t>(elapsed));
        }
        
    private:
        ProfileEntry& entry_;
        juce::int64 startTime_;
    };
    
    ProfileEntry& getEntry(const char* name) noexcept
    {
        for (auto& entry : entries_)
        {
            if (std::strcmp(entry.name, name) == 0)
            {
                return entry;
            }
        }
        
        int nextIndex = nextEntryIndex_.fetch_add(1, std::memory_order_acquire);
        if (nextIndex < MaxEntries)
        {
            ProfileEntry& entry = entries_[nextIndex];
            strncpy_s(entry.name, sizeof(entry.name), name, sizeof(entry.name) - 1);
            entry.name[sizeof(entry.name) - 1] = '\0';
            return entry;
        }
        else
        {
            nextEntryIndex_.fetch_sub(1, std::memory_order_acq_rel);
            return dummyEntry_;
        }
    }
    
    ScopedTimer createScopedTimer(const char* name) noexcept
    {
        return ScopedTimer(getEntry(name));
    }
    
private:
    static constexpr int MaxEntries = 128;
    std::array<ProfileEntry, MaxEntries> entries_;
    std::atomic<int> nextEntryIndex_{0};
    ProfileEntry dummyEntry_;
};


class RealtimeMemorySystem
{
public:
    static RealtimeMemorySystem& getInstance() noexcept
    {
        static RealtimeMemorySystem instance;
        return instance;
    }
    
    RealtimeObjectPool<juce::AudioBuffer<float>, 64> audioBufferPool;
    RealtimeObjectPool<std::vector<float>, 128> vectorPool;
    AudioBufferPool<4096, 32> largeBufferPool;
    AudioBufferPool<1024, 64> smallBufferPool;
    
    SamplePool samples;
    RealtimeProfiler profiler;
    
private:
    RealtimeMemorySystem() = default;
};

#define RT_SCOPED_TIMER(name) \
    auto _timer = RealtimeMemorySystem::getInstance().profiler.createScopedTimer(name)

#define RT_SCOPED_BUFFER() \
    RealtimeMemorySystem::getInstance().largeBufferPool.ScopedBuffer( \
        RealtimeMemorySystem::getInstance().largeBufferPool)