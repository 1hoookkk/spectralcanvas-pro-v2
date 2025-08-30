#pragma once

#include <atomic>
#include <chrono>

/// @brief Lock-free GPU telemetry struct for UI thread sampling
/// 
/// This struct provides atomic counters and status flags that can be safely
/// read by the UI thread while being updated by the GPU/render thread.
/// All values use atomic operations to ensure thread safety without locks.
///
/// RT Safety: Read operations are lock-free and suitable for RT contexts.
/// The UI thread samples these values at 60fps for status display.
struct alignas(64) GPUStatus
{
    /// Device states for resilience state machine
    enum DeviceState : int32_t
    {
        OK = 0,           ///< Device operational, hardware rendering
        REMOVED = 1,      ///< Device lost, recovery in progress  
        RECREATING = 2,   ///< Attempting device recreation
        WARP_FALLBACK = 3 ///< Using WARP software renderer
    };
    
    /// Current device state (atomic read/write)
    std::atomic<DeviceState> device_state{OK};
    
    /// Rolling average frame time in microseconds
    std::atomic<uint32_t> frame_time_us{16666}; // Default ~60fps
    
    /// Number of device recovery attempts completed
    std::atomic<uint32_t> recovery_count{0};
    
    /// Timestamp of last successful recovery (steady_clock::time_point as uint64_t)
    std::atomic<uint64_t> last_recovery_timestamp{0};
    
    /// Frame counter (increments each present, for continuity validation)
    std::atomic<uint64_t> frame_counter{0};
    
    /// Peak frame time in current second (resets every ~60 frames)
    std::atomic<uint32_t> peak_frame_time_us{0};
    
    /// Number of frames dropped due to device issues
    std::atomic<uint32_t> dropped_frame_count{0};
    
    /// Whether currently using WARP renderer (for UI indication)
    std::atomic<bool> is_warp_mode{false};
    
    /// GPU memory usage estimate in bytes (updated periodically)
    std::atomic<uint64_t> gpu_memory_usage{0};
    
    /// RT-safe methods for updating telemetry
    
    /// @brief Record frame completion with timing
    inline void recordFrameTime(uint32_t microseconds) noexcept
    {
        frame_time_us.store(microseconds, std::memory_order_release);
        frame_counter.fetch_add(1, std::memory_order_acq_rel);
        
        // Update peak within current measurement window
        uint32_t current_peak = peak_frame_time_us.load(std::memory_order_acquire);
        if (microseconds > current_peak)
        {
            peak_frame_time_us.compare_exchange_weak(current_peak, microseconds, 
                                                     std::memory_order_release);
        }
    }
    
    /// @brief Signal device state change
    inline void setDeviceState(DeviceState new_state) noexcept
    {
        device_state.store(new_state, std::memory_order_release);
        
        // Record recovery completion timestamp
        if (new_state == OK || new_state == WARP_FALLBACK)
        {
            auto now = std::chrono::steady_clock::now();
            auto timestamp = now.time_since_epoch().count();
            last_recovery_timestamp.store(static_cast<uint64_t>(timestamp), 
                                         std::memory_order_release);
            
            if (new_state == OK && recovery_count.load(std::memory_order_acquire) > 0)
            {
                // Successful recovery from previous failure
                recovery_count.fetch_add(1, std::memory_order_acq_rel);
            }
        }
        
        is_warp_mode.store(new_state == WARP_FALLBACK, std::memory_order_release);
    }
    
    /// @brief Increment dropped frame counter
    inline void recordDroppedFrame() noexcept
    {
        dropped_frame_count.fetch_add(1, std::memory_order_acq_rel);
    }
    
    /// @brief Reset peak frame time (called periodically from UI thread)
    inline void resetPeakFrameTime() noexcept
    {
        peak_frame_time_us.store(0, std::memory_order_release);
    }
    
    /// UI thread sampling methods (all lock-free reads)
    
    inline DeviceState getDeviceState() const noexcept
    {
        return device_state.load(std::memory_order_acquire);
    }
    
    inline uint32_t getFrameTimeUs() const noexcept
    {
        return frame_time_us.load(std::memory_order_acquire);
    }
    
    inline uint32_t getPeakFrameTimeUs() const noexcept
    {
        return peak_frame_time_us.load(std::memory_order_acquire);
    }
    
    inline uint32_t getRecoveryCount() const noexcept
    {
        return recovery_count.load(std::memory_order_acquire);
    }
    
    inline uint64_t getFrameCounter() const noexcept
    {
        return frame_counter.load(std::memory_order_acquire);
    }
    
    inline bool isWarpMode() const noexcept
    {
        return is_warp_mode.load(std::memory_order_acquire);
    }
    
    inline uint32_t getDroppedFrameCount() const noexcept
    {
        return dropped_frame_count.load(std::memory_order_acquire);
    }
    
    inline uint64_t getLastRecoveryTimestamp() const noexcept
    {
        return last_recovery_timestamp.load(std::memory_order_acquire);
    }
};

static_assert(sizeof(GPUStatus) % 64 == 0, "GPUStatus must be cache-line aligned");