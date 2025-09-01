#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#ifdef _MSC_VER
  #pragma warning(disable: 4324) // 'structure was padded due to alignment specifier'
#endif

/**
 * @file SpscQueueLite.h  
 * @brief Lock-free Single Producer Single Consumer queue for RT use
 *
 * Optimized for minimal latency and zero allocations. Uses power-of-2
 * capacity for efficient modulo via bitwise AND. Drops items on full
 * rather than blocking (never stall the audio thread).
 *
 * Thread Safety:
 * - Single producer (UI thread) calls try_push()
 * - Single consumer (audio thread) calls try_pop()
 * - No locks, no syscalls, no allocations
 * - Memory ordering ensures visibility across threads
 */

namespace rt {

// Forward declaration
struct MaskColumnLite;

template <typename T, size_t CapacityPow2>
class SpscQueueLite {
    static_assert((CapacityPow2 & (CapacityPow2 - 1)) == 0, 
                  "Capacity must be power of two for efficient modulo");
    static_assert(CapacityPow2 >= 2 && CapacityPow2 <= 65536,
                  "Capacity must be reasonable size");
    static_assert(std::is_trivially_copyable_v<T>,
                  "Queue element must be trivially copyable for RT safety");

public:
    /// Initialize empty queue
    SpscQueueLite() noexcept : head_(0), tail_(0) {}
    
    /// Non-copyable for safety
    SpscQueueLite(const SpscQueueLite&) = delete;
    SpscQueueLite& operator=(const SpscQueueLite&) = delete;
    
    /**
     * @brief Try to push item (producer thread only)
     * @param item Item to enqueue
     * @return true if pushed, false if queue full (item dropped)
     * 
     * Never blocks - drops on full to maintain RT guarantees.
     * Producer should monitor drop count if needed.
     */
    bool try_push(const T& item) noexcept {
        const auto head = head_.load(std::memory_order_relaxed);
        const auto next_head = (head + 1) & MASK;
        
        // Queue full? Drop item (never block)
        if (next_head == tail_.load(std::memory_order_acquire)) {
            ++drop_count_;
            return false;
        }
        
        // Store item and publish
        buffer_[head] = item;
        head_.store(next_head, std::memory_order_release);
        ++push_count_;
        return true;
    }
    
    /**
     * @brief Try to pop item (consumer thread only)
     * @param out Output item if pop succeeds
     * @return true if popped, false if queue empty
     */
    bool try_pop(T& out) noexcept {
        const auto tail = tail_.load(std::memory_order_relaxed);
        
        // Queue empty?
        if (tail == head_.load(std::memory_order_acquire)) {
            return false;
        }
        
        // Load item and advance
        out = buffer_[tail];
        tail_.store((tail + 1) & MASK, std::memory_order_release);
        ++pop_count_;
        return true;
    }
    
    /**
     * @brief Check if queue is empty (approximate)
     * Safe to call from any thread, but result may be stale.
     */
    bool empty() const noexcept {
        return head_.load(std::memory_order_acquire) == 
               tail_.load(std::memory_order_acquire);
    }
    
    /**
     * @brief Get approximate current size
     * Safe to call from any thread, but result may be stale.
     */
    size_t size() const noexcept {
        const auto head = head_.load(std::memory_order_acquire);
        const auto tail = tail_.load(std::memory_order_acquire);
        return (head - tail) & MASK;
    }
    
    /// Get maximum capacity
    constexpr size_t capacity() const noexcept { return CapacityPow2; }
    
    /// Statistics for monitoring (relaxed ordering)
    size_t get_push_count() const noexcept { return push_count_; }
    size_t get_pop_count() const noexcept { return pop_count_; }
    size_t get_drop_count() const noexcept { return drop_count_; }
    
    /// Reset statistics (not thread-safe - call when idle)
    void reset_stats() noexcept {
        push_count_ = pop_count_ = drop_count_ = 0;
    }

private:
    static constexpr size_t MASK = CapacityPow2 - 1;
    
    // Cache-line aligned to avoid false sharing
    alignas(64) T buffer_[CapacityPow2];
    
    // Separate cache lines for head/tail to avoid bouncing
    alignas(64) std::atomic<size_t> head_;
    alignas(64) std::atomic<size_t> tail_;
    
    // Statistics (relaxed - not critical path)
    size_t push_count_ = 0;
    size_t pop_count_ = 0;
    size_t drop_count_ = 0;
};

// Common queue sizes for different use cases
using MaskQueue64  = SpscQueueLite<MaskColumnLite, 64>;
using MaskQueue128 = SpscQueueLite<MaskColumnLite, 128>;
using MaskQueue256 = SpscQueueLite<MaskColumnLite, 256>;

} // namespace rt