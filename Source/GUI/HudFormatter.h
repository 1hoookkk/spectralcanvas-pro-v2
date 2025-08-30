#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

/**
 * @file HudFormatter.h
 * @brief Lightweight formatting utilities for Performance HUD display
 * 
 * Header-only utilities for consistent number formatting in the HUD overlay.
 * All functions are inline for minimal overhead in UI rendering path.
 */

namespace HudFormatter
{
    /**
     * Format latency value with appropriate precision
     * @param latencyMs Latency value in milliseconds
     * @return Formatted string (e.g., "3.2", "12.8", ">99")
     */
    inline std::string formatLatency(float latencyMs) noexcept
    {
        if (latencyMs >= 99.9f) return ">99";
        if (latencyMs >= 10.0f) return std::to_string(static_cast<int>(std::round(latencyMs)));
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << latencyMs;
        return oss.str();
    }
    
    /**
     * Format CPU percentage with 1 decimal place
     * @param cpuPercent CPU usage percentage (0-100)
     * @return Formatted string (e.g., "8.5", "12.3", ">99")
     */
    inline std::string formatCpuPercent(float cpuPercent) noexcept
    {
        if (cpuPercent >= 99.9f) return ">99";
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << cpuPercent;
        return oss.str();
    }
    
    /**
     * Format GPU frame time in milliseconds
     * @param frameTimeUs Frame time in microseconds
     * @return Formatted string (e.g., "14.2", "8.1", ">99")
     */
    inline std::string formatFrameTime(uint32_t frameTimeUs) noexcept
    {
        float frameTimeMs = frameTimeUs / 1000.0f;
        if (frameTimeMs >= 99.9f) return ">99";
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << frameTimeMs;
        return oss.str();
    }
    
    /**
     * Format device state as short string
     * @param isWarp Whether using WARP software renderer
     * @param deviceOk Whether device is in OK state
     * @return Device mode string ("HW", "WARP", "ERR")
     */
    inline std::string formatDeviceMode(bool isWarp, bool deviceOk) noexcept
    {
        if (!deviceOk) return "ERR";
        return isWarp ? "WARP" : "HW";
    }
    
    /**
     * Format recovery time since last device recovery
     * @param recoveryTimestamp Last recovery timestamp (steady_clock nanoseconds)
     * @param currentTimestamp Current timestamp (steady_clock nanoseconds)
     * @return Formatted time string (e.g., "123ms", "1.2s", ">99s")
     */
    inline std::string formatRecoveryTime(uint64_t recoveryTimestamp, uint64_t currentTimestamp) noexcept
    {
        if (recoveryTimestamp == 0) return "never";
        
        uint64_t elapsedNs = currentTimestamp - recoveryTimestamp;
        double elapsedMs = elapsedNs / 1000000.0;
        
        if (elapsedMs < 1000.0) {
            return std::to_string(static_cast<int>(std::round(elapsedMs))) + "ms";
        }
        
        double elapsedS = elapsedMs / 1000.0;
        if (elapsedS < 99.9) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << elapsedS << "s";
            return oss.str();
        }
        
        return ">99s";
    }
    
    /**
     * Format queue depth as "current/capacity"
     * @param current Current queue depth
     * @param capacity Maximum queue capacity
     * @return Formatted string (e.g., "7/8", "12/16")
     */
    inline std::string formatQueueDepth(size_t current, size_t capacity) noexcept
    {
        return std::to_string(current) + "/" + std::to_string(capacity);
    }
    
    /**
     * Format sample rate and block size for footer
     * @param sampleRate Sample rate in Hz
     * @param blockSize Audio block size in samples
     * @return Formatted string (e.g., "48kHz/512", "44.1kHz/256")
     */
    inline std::string formatAudioConfig(double sampleRate, int blockSize) noexcept
    {
        // Convert to kHz with appropriate precision
        if (sampleRate >= 48000.0) {
            return std::to_string(static_cast<int>(sampleRate / 1000)) + "kHz/" + std::to_string(blockSize);
        } else if (sampleRate == 44100.0) {
            return "44.1kHz/" + std::to_string(blockSize);
        } else {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << (sampleRate / 1000.0) << "kHz/" << blockSize;
            return oss.str();
        }
    }
    
    /**
     * Clamp string to maximum width for consistent layout
     * @param str Input string
     * @param maxWidth Maximum character width
     * @return Truncated string with "..." if needed
     */
    inline std::string clampWidth(const std::string& str, size_t maxWidth) noexcept
    {
        if (str.length() <= maxWidth) return str;
        if (maxWidth < 3) return str.substr(0, maxWidth);
        return str.substr(0, maxWidth - 3) + "...";
    }
}