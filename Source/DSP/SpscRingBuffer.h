#pragma once
#include <atomic>
#include <cstddef>

// Single‑producer / single‑consumer lock‑free ring buffer.
// Capacity must be a power of two.
template <typename T, std::size_t Capacity>
class SpscRingBuffer
{
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be power of two");
public:
    SpscRingBuffer() noexcept : head_(0), tail_(0) {}

    bool push(const T& item) noexcept
    {
        const std::size_t h = head_.load(std::memory_order_relaxed);
        const std::size_t next = (h + 1) & (Capacity - 1);
        if (next == tail_.load(std::memory_order_acquire))
            return false; // full
        buffer_[h] = item; // trivial copy
        head_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& out) noexcept
    {
        const std::size_t t = tail_.load(std::memory_order_relaxed);
        if (t == head_.load(std::memory_order_acquire))
            return false; // empty
        out = buffer_[t];
        tail_.store((t + 1) & (Capacity - 1), std::memory_order_release);
        return true;
    }

    bool empty() const noexcept { return head_.load() == tail_.load(); }

private:
    T buffer_[Capacity];
    std::atomic<std::size_t> head_;
    std::atomic<std::size_t> tail_;
};
