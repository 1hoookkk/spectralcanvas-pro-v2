#pragma once
#include <atomic>
#include <cstddef>
#include <type_traits>

// Lock-free SPSC ring buffer (power-of-two capacity)
template <typename T, size_t CapacityPow2>
class SpscQueueLite
{
    static_assert((CapacityPow2 & (CapacityPow2 - 1)) == 0, "Capacity must be power of two");
    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");

public:
    SpscQueueLite() : head_(0), tail_(0) {}

    bool tryPush(const T& item) noexcept
    {
        const size_t head = head_.load(std::memory_order_relaxed);
        const size_t next = (head + 1) & mask();
        if (next == tail_.load(std::memory_order_acquire))
            return false; // full
        buffer_[head] = item;
        head_.store(next, std::memory_order_release);
        return true;
    }

    bool tryPop(T& out) noexcept
    {
        const size_t tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire))
            return false; // empty
        out = buffer_[tail];
        tail_.store((tail + 1) & mask(), std::memory_order_release);
        return true;
    }

    size_t sizeApprox() const noexcept
    {
        const size_t h = head_.load(std::memory_order_acquire);
        const size_t t = tail_.load(std::memory_order_acquire);
        return (h - t) & mask();
    }

    constexpr size_t capacity() const noexcept { return CapacityPow2 - 1; }

private:
    constexpr size_t mask() const noexcept { return CapacityPow2 - 1; }

    T buffer_[CapacityPow2]{};
    alignas(64) std::atomic<size_t> head_;
    alignas(64) std::atomic<size_t> tail_;
};
