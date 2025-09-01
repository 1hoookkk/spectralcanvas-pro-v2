#pragma once

#include <atomic>
#include <cstdint>
#include <chrono>

/**
 * GPU Status and Performance Telemetry
 * 
 * Atomic status tracking for D3D11 device state, frame timing, and recovery metrics.
 * Used for device-lost detection, performance monitoring, and WARP fallback coordination.
 * 
 * RT Safety: All methods are lock-free and safe to call from any thread.
 */
class GPUStatus
{
public:
    /// Device state enumeration
    enum DeviceState : uint32_t
    {
        OK = 0,              ///< Device operational
        REMOVED = 1,         ///< Device lost, recovery needed
        RECREATING = 2,      ///< Recovery in progress
        WARP_FALLBACK = 3    ///< Software rendering fallback
    };
    
    GPUStatus() noexcept
        : device_state_(OK)
        , frame_time_us_(16666)  // 60fps default
        , recovery_count_(0)
        , last_recovery_timestamp_(0)
        , dropped_frame_count_(0)
        , is_warp_mode_(false)
    {}
    
    // Device state management
    DeviceState getDeviceState() const noexcept 
    { 
        return static_cast<DeviceState>(device_state_.load(std::memory_order_acquire)); 
    }
    
    void setDeviceState(DeviceState state) noexcept 
    { 
        device_state_.store(static_cast<uint32_t>(state), std::memory_order_release);
        if (state == WARP_FALLBACK)
            is_warp_mode_.store(true, std::memory_order_relaxed);
        else if (state == OK)
            is_warp_mode_.store(false, std::memory_order_relaxed);
    }
    
    // Frame timing
    uint32_t getFrameTimeUs() const noexcept 
    { 
        return frame_time_us_.load(std::memory_order_relaxed); 
    }
    
    void recordFrameTime(uint32_t microseconds) noexcept 
    { 
        frame_time_us_.store(microseconds, std::memory_order_relaxed); 
    }
    
    // Recovery tracking
    uint32_t getRecoveryCount() const noexcept 
    { 
        return recovery_count_.load(std::memory_order_relaxed); 
    }
    
    void incrementRecoveryCount() noexcept 
    { 
        recovery_count_.fetch_add(1, std::memory_order_relaxed);
        last_recovery_timestamp_.store(
            std::chrono::steady_clock::now().time_since_epoch().count(),
            std::memory_order_relaxed
        );
    }
    
    uint64_t getLastRecoveryTimestamp() const noexcept 
    { 
        return last_recovery_timestamp_.load(std::memory_order_relaxed); 
    }
    
    // Frame drop tracking
    uint32_t getDroppedFrameCount() const noexcept 
    { 
        return dropped_frame_count_.load(std::memory_order_relaxed); 
    }
    
    void recordDroppedFrame() noexcept 
    { 
        dropped_frame_count_.fetch_add(1, std::memory_order_relaxed); 
    }
    
    // WARP mode detection
    bool isWarpMode() const noexcept 
    { 
        return is_warp_mode_.load(std::memory_order_relaxed); 
    }
    
    // Performance helpers
    float getFrameRate() const noexcept
    {
        uint32_t frame_time = getFrameTimeUs();
        return frame_time > 0 ? 1000000.0f / frame_time : 0.0f;
    }
    
    bool isPerformanceAcceptable(uint32_t max_frame_time_us = 16666) const noexcept
    {
        return getFrameTimeUs() <= max_frame_time_us;
    }

private:
    std::atomic<uint32_t> device_state_;            ///< Current device state
    std::atomic<uint32_t> frame_time_us_;           ///< Last frame time in microseconds
    std::atomic<uint32_t> recovery_count_;          ///< Number of recovery attempts
    std::atomic<uint64_t> last_recovery_timestamp_; ///< Timestamp of last recovery
    std::atomic<uint32_t> dropped_frame_count_;     ///< Number of dropped frames
    std::atomic<bool> is_warp_mode_;                ///< Software rendering active
};