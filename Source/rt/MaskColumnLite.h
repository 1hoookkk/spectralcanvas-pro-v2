#pragma once

#include <cstdint>
#include <type_traits>

/**
 * @file MaskColumnLite.h
 * @brief Lightweight 12-byte POD for RT-safe spectral painting
 * 
 * Replaces the heavy 4KB+ MaskColumn with a minimal structure for
 * fast-paint mode with 256-point FFT. Single paint event per message.
 * 
 * Thread Safety:
 * - Trivially copyable POD suitable for lock-free queues
 * - No pointers, no destructors, no virtual methods
 * - Cache-line friendly at 12 bytes
 */

namespace rt {

struct MaskColumnLite {
    float y;          ///< [0,1] vertical position -> log-freq mapping
    float intensity;  ///< [0,1] brush strength for radial boost
    uint32_t tMs;     ///< Event timestamp in milliseconds
    
    /// Default constructor for zero initialization
    constexpr MaskColumnLite() noexcept
        : y(0.0f), intensity(0.0f), tMs(0) {}
    
    /// Constructor with values
    constexpr MaskColumnLite(float y_, float intensity_, uint32_t tMs_) noexcept
        : y(y_), intensity(intensity_), tMs(tMs_) {}
};

// Compile-time safety checks
static_assert(std::is_trivially_copyable_v<MaskColumnLite>, 
              "MaskColumnLite must be POD for lock-free queue safety");
static_assert(std::is_standard_layout_v<MaskColumnLite>, 
              "MaskColumnLite must have standard layout");
static_assert(sizeof(MaskColumnLite) == 12, 
              "MaskColumnLite size changed - keep this tiny for performance");
static_assert(alignof(MaskColumnLite) <= 8, 
              "MaskColumnLite alignment too large for efficient queue packing");

} // namespace rt