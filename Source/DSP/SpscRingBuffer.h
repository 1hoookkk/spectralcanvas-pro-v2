#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <optional>

/**
 * Single-producer / single-consumer lock-free ring buffer.
 * Power-of-two capacity for efficient masking operations.
 * 
 * Thread-safe communication between UI and audio threads.
 * Cache-line aligned to prevent false sharing.
 */
template <typename T, size_t CapacityPow2>
class SpscRingBuffer
{
    static_assert((CapacityPow2 & (CapacityPow2 - 1)) == 0, "Capacity must be power of two");
    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable for lock-free SPSC");

public:
    SpscRingBuffer() : head(0), tail(0) {}

    // Producer thread (UI/Message thread)
    bool push(const T& value) noexcept
    {
        const auto h = head.load(std::memory_order_relaxed);
        const auto t = tail.load(std::memory_order_acquire);
        if (((h + 1) & mask()) == (t & mask()))
            return false; // Buffer full
        
        buffer[h & mask()] = value;
        head.store(h + 1, std::memory_order_release);
        return true;
    }

    // Consumer thread (audio thread) - returns optional for compatibility
    std::optional<T> pop() noexcept
    {
        const auto t = tail.load(std::memory_order_relaxed);
        const auto h = head.load(std::memory_order_acquire);
        if ((t & mask()) == (h & mask()))
            return std::nullopt; // Buffer empty
        
        T value = buffer[t & mask()];
        tail.store(t + 1, std::memory_order_release);
        return value;
    }
    
    // Alternative pop method with reference parameter
    bool pop(T& out) noexcept
    {
        const auto t = tail.load(std::memory_order_relaxed);
        const auto h = head.load(std::memory_order_acquire);
        if ((t & mask()) == (h & mask()))
            return false; // Buffer empty
        
        out = buffer[t & mask()];
        tail.store(t + 1, std::memory_order_release);
        return true;
    }

    size_t size() const noexcept
    {
        const auto h = head.load(std::memory_order_acquire);
        const auto t = tail.load(std::memory_order_acquire);
        return (h - t) & ((size_t{1} << kBits) - 1);
    }

    size_t freeSpace() const noexcept { return capacity() - size() - 1; }
    static constexpr size_t capacity() noexcept { return CapacityPow2; }
    bool empty() const noexcept { return size() == 0; }
    bool full() const noexcept { return freeSpace() == 0; }
    
    // API compatibility with existing MessageBus usage
    void clear() noexcept 
    { 
        head.store(0, std::memory_order_relaxed); 
        tail.store(0, std::memory_order_relaxed);
    }
    
    bool hasSpaceAvailable() const noexcept { return !full(); }
    bool hasDataAvailable() const noexcept { return !empty(); }
    
    // Statistics methods for compatibility
    size_t getApproxQueueDepth() const noexcept { return size(); }
    size_t getDropCount() const noexcept { return 0; } // Not tracked in this implementation

private:
    static constexpr size_t kBits = [] {
        size_t n = 0, c = CapacityPow2;
        while (c >>= 1) ++n;
        return n;
    }();

    static constexpr size_t mask() noexcept { return CapacityPow2 - 1; }

    // Cache-line aligned to prevent false sharing
    alignas(64) std::atomic<size_t> head;
    alignas(64) std::atomic<size_t> tail;
    alignas(64) T buffer[CapacityPow2];
};