#pragma once

#include "../Core/GPUStatus.h"
#include <chrono>

#ifdef _WIN32
#include <d3d11.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#endif

/// @brief Device-lost recovery choreography for D3D11 resilience
///
/// Implements the state machine: OK → REMOVED → RECREATING → OK/WARP_FALLBACK
/// Handles graceful degradation to WARP after repeated hardware failures.
/// 
/// RT Safety: All methods are called from render thread only.
/// No allocations or blocking operations during recovery.
class DeviceLostHandler
{
public:
    /// Recovery attempt limits before fallback
    static constexpr int MAX_HW_RECOVERY_ATTEMPTS = 3;
    
    /// Timeout for device recreation attempts (milliseconds)
    static constexpr int RECOVERY_TIMEOUT_MS = 100;
    
    DeviceLostHandler() = default;
    ~DeviceLostHandler() = default;
    
    /// @brief Initialize handler with status telemetry
    void initialize(GPUStatus* gpu_status) noexcept
    {
        gpu_status_ = gpu_status;
        hw_failure_count_ = 0;
        is_warp_mode_ = false;
        last_recovery_attempt_ = std::chrono::steady_clock::time_point{};
    }
    
    /// @brief Check if device removal occurred
    /// @param device D3D11 device to check
    /// @return true if device was removed and recovery is needed
    bool checkDeviceRemoval(ID3D11Device* device) noexcept
    {
        if (!device || !gpu_status_)
            return false;
            
        HRESULT hr = device->GetDeviceRemovedReason();
        if (SUCCEEDED(hr))
            return false; // Device still OK
            
        // Device removal detected
        if (gpu_status_->getDeviceState() == GPUStatus::OK)
        {
            gpu_status_->setDeviceState(GPUStatus::REMOVED);
            last_removal_reason_ = hr;
        }
        
        return true;
    }
    
    /// @brief Determine if recovery should be attempted
    /// @return true if recovery should proceed, false if too many recent attempts
    bool shouldAttemptRecovery() const noexcept
    {
        if (!gpu_status_)
            return false;
            
        auto current_state = gpu_status_->getDeviceState();
        if (current_state != GPUStatus::REMOVED)
            return false; // Not in removal state
            
        // Rate-limit recovery attempts
        auto now = std::chrono::steady_clock::now();
        auto time_since_last = now - last_recovery_attempt_;
        
        return time_since_last > std::chrono::milliseconds(50); // Min 50ms between attempts
    }
    
    /// @brief Begin device recovery process
    void beginRecovery() noexcept
    {
        if (!gpu_status_)
            return;
            
        gpu_status_->setDeviceState(GPUStatus::RECREATING);
        last_recovery_attempt_ = std::chrono::steady_clock::now();
    }
    
    /// @brief Record successful device recovery
    /// @param used_warp true if WARP fallback was used
    void recordSuccessfulRecovery(bool used_warp) noexcept
    {
        if (!gpu_status_)
            return;
            
        if (used_warp)
        {
            gpu_status_->setDeviceState(GPUStatus::WARP_FALLBACK);
            is_warp_mode_ = true;
        }
        else
        {
            gpu_status_->setDeviceState(GPUStatus::OK);
            hw_failure_count_ = 0; // Reset failure count on successful HW recovery
        }
    }
    
    /// @brief Record failed recovery attempt
    void recordFailedRecovery() noexcept
    {
        if (!gpu_status_)
            return;
            
        hw_failure_count_++;
        
        // Return to REMOVED state for retry or fallback decision
        gpu_status_->setDeviceState(GPUStatus::REMOVED);
    }
    
    /// @brief Check if should fallback to WARP after repeated HW failures
    bool shouldFallbackToWarp() const noexcept
    {
        return hw_failure_count_ >= MAX_HW_RECOVERY_ATTEMPTS && !is_warp_mode_;
    }
    
    /// @brief Get current recovery state for logging/debugging
    struct RecoveryInfo
    {
        HRESULT last_removal_reason = S_OK;
        int hw_failure_count = 0;
        bool is_warp_mode = false;
        std::chrono::steady_clock::time_point last_attempt{};
    };
    
    RecoveryInfo getRecoveryInfo() const noexcept
    {
        return {
            last_removal_reason_,
            hw_failure_count_,
            is_warp_mode_,
            last_recovery_attempt_
        };
    }
    
    /// @brief Reset handler state (for testing or manual recovery)
    void reset() noexcept
    {
        hw_failure_count_ = 0;
        is_warp_mode_ = false;
        last_removal_reason_ = S_OK;
        last_recovery_attempt_ = std::chrono::steady_clock::time_point{};
        
        if (gpu_status_)
        {
            gpu_status_->setDeviceState(GPUStatus::OK);
        }
    }
    
    /// @brief Check if recovery attempt has timed out
    bool isRecoveryTimedOut() const noexcept
    {
        if (gpu_status_ && gpu_status_->getDeviceState() != GPUStatus::RECREATING)
            return false;
            
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - last_recovery_attempt_);
            
        return elapsed.count() > RECOVERY_TIMEOUT_MS;
    }

private:
    GPUStatus* gpu_status_ = nullptr;
    
    /// Number of consecutive hardware device creation failures
    int hw_failure_count_ = 0;
    
    /// Whether currently using WARP fallback mode
    bool is_warp_mode_ = false;
    
    /// HRESULT from last GetDeviceRemovedReason() call
    HRESULT last_removal_reason_ = S_OK;
    
    /// Timestamp of last recovery attempt (for rate limiting)
    std::chrono::steady_clock::time_point last_recovery_attempt_{};
};

/// @brief RAII helper for device recreation timing
class DeviceRecoveryTimer
{
public:
    DeviceRecoveryTimer() : start_time_(std::chrono::high_resolution_clock::now()) {}
    
    /// @brief Get elapsed recovery time in microseconds
    uint32_t getElapsedMicroseconds() const noexcept
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time_);
        return static_cast<uint32_t>(elapsed.count());
    }
    
private:
    std::chrono::high_resolution_clock::time_point start_time_;
};