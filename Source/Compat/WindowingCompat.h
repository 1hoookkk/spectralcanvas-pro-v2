/* ==========================================================================
   spectralcanvas-pro-v2 — Compatibility + Thread-Safety Hardening
   WindowingCompat shim + std::isfinite portability

   Summary:
   - Introduce compat::fillWindow(...) to abstract JUCE's windowing API
     differences (fillWindowingTable vs fillWindowingTables).
   - Provide math fallback for Hann/Hamming/Blackman if neither helper exists.
   - Use <cmath> std::isfinite for portability.

   Invariants:
   - Atomic latency reporting: getLatencySamples() returns atomic value.
   - GUI reads immutable snapshots via atomic shared_ptr load (no live audio ptrs).
   - No placement-new of WindowingFunction on audio thread.

   Testing:
   - Build on target JUCE version(s)
   - Run pluginval (expect stable latency & no editor-open crash)
   - Optional: ASan/UBSan run if issues persist
   ========================================================================== */

#pragma once

#include <cmath>
#include <cstdint>
#include <type_traits>

#include <juce_dsp/juce_dsp.h>

// Notes:
// - Some JUCE versions expose: WindowingFunction<T>::fillWindowingTable(...)
// - Others expose:               WindowingFunction<T>::fillWindowingTables(...)
// - This wrapper chooses whichever exists at compile-time.
// - If neither exists, we compute a few common windows ourselves (Hann/Hamming/Blackman).

namespace compat
{
    template <typename Sample>
    using WF = juce::dsp::WindowingFunction<Sample>;

    // --- compile-time detection helpers (SFINAE) ---
    template <typename Sample>
    static auto has_fillTables (Sample*)
        -> decltype( WF<Sample>::fillWindowingTables((Sample*)nullptr, 0,
                                                     typename WF<Sample>::WindowingMethod{}), std::true_type{} );

    static auto has_fillTables(...) -> std::false_type;

    template <typename Sample>
    static auto has_fillTable (Sample*)
        -> decltype( WF<Sample>::fillWindowingTable ((Sample*)nullptr, 0,
                                                     typename WF<Sample>::WindowingMethod{}), std::true_type{} );

    static auto has_fillTable(...) -> std::false_type;

    // --- simple math fallbacks for common windows ---
    template <typename Sample>
    inline void fallbackFill (Sample* dest, int size,
                              typename WF<Sample>::WindowingMethod method)
    {
        // Only implements a few common windows; extend if you need more.
        constexpr Sample pi = static_cast<Sample>(3.1415926535897932384626433832795L);
        switch (method)
        {
            case WF<Sample>::hann:
            {
                for (int n = 0; n < size; ++n)
                    dest[n] = static_cast<Sample>(0.5) * (static_cast<Sample>(1)
                            - std::cos(static_cast<Sample>(2) * pi * n / (size - 1)));
                break;
            }
            case WF<Sample>::hamming:
            {
                // α = 0.54, β = 0.46
                for (int n = 0; n < size; ++n)
                    dest[n] = static_cast<Sample>(0.54) - static_cast<Sample>(0.46)
                            * std::cos(static_cast<Sample>(2) * pi * n / (size - 1));
                break;
            }
            case WF<Sample>::blackman:
            {
                // Classic Blackman (a0=0.42,a1=0.5,a2=0.08)
                for (int n = 0; n < size; ++n)
                {
                    auto a = static_cast<Sample>(2) * pi * n / (size - 1);
                    dest[n] = static_cast<Sample>(0.42)
                            - static_cast<Sample>(0.5) * std::cos(a)
                            + static_cast<Sample>(0.08) * std::cos(static_cast<Sample>(2) * a);
                }
                break;
            }
            default:
            {
                // Fallback to Hann if unsupported method
                for (int n = 0; n < size; ++n)
                    dest[n] = static_cast<Sample>(0.5) * (static_cast<Sample>(1)
                            - std::cos(static_cast<Sample>(2) * pi * n / (size - 1)));
                break;
            }
        }
    }

    // --- single entry point you should call in your code ---
    template <typename Sample>
    inline void fillWindow (Sample* dest, int size,
                            typename WF<Sample>::WindowingMethod method)
    {
        if constexpr (decltype(has_fillTables((Sample*)nullptr))::value)
        {
            // Newer JUCE API
            WF<Sample>::fillWindowingTables(dest, size, method);
        }
        else if constexpr (decltype(has_fillTable((Sample*)nullptr))::value)
        {
            // Older JUCE API
            WF<Sample>::fillWindowingTable(dest, size, method);
        }
        else
        {
            // No helper available -> do it ourselves
            fallbackFill(dest, size, method);
        }
    }
} // namespace compat