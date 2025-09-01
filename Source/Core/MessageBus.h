#pragma once
#include <atomic>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <cstring>
#include <cstdint>
#include <cstddef>  // For offsetof
#include "RealtimeSafeTypes.h"
#include "../DSP/SpscRingBuffer.h"
#include "SampleMessage.h"

#ifdef _MSC_VER
  #pragma warning(disable: 4324) // 'structure was padded due to alignment specifier'
#endif

// Phase 4 experiment flag - oscillator bank with key filter
#define PHASE4_EXPERIMENT 1

// Using improved SpscRingBuffer from ARTEFACT_Production codebase
// Provides better performance and additional utility methods

// Spectral data frame for Audio Thread → UI Thread communication
#if _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4324)
#endif

struct SpectralFrame
{
    static constexpr size_t FFT_SIZE = 512;
    static constexpr size_t NUM_BINS = FFT_SIZE / 2 + 1; // 257 bins for real FFT
    
    alignas(32) float magnitude[NUM_BINS];    // Spectral magnitudes
    alignas(32) float phase[NUM_BINS];        // Spectral phases
    
    uint32_t sequenceNumber;                  // Monotonic frame counter
    uint32_t version;                         // Message schema version for compatibility
    uint64_t engineFrameId;                   // Engine timeline reference
    double timestampSamples;                  // Sample-accurate timestamp
    float fundamentalFreq;                    // Detected fundamental (0 if none)
    float spectralCentroid;                   // Brightness measure
    
    SpectralFrame() noexcept
    {
        std::memset(magnitude, 0, sizeof(magnitude));
        std::memset(phase, 0, sizeof(phase));
        sequenceNumber = 0;
        version = 1;
        engineFrameId = 0;
        timestampSamples = 0.0;
        fundamentalFreq = 0.0f;
        spectralCentroid = 0.0f;
    }
};

#if _MSC_VER
  #pragma warning(pop)
#endif

// Parameter update for UI Thread → Audio Thread communication  
struct ParameterUpdate
{
    uint32_t parameterId;                     // Parameter identifier
    float normalizedValue;                    // 0.0 to 1.0 range
    uint32_t sampleOffset;                    // Buffer-relative timing for sample accuracy
    uint32_t version;                         // Message schema version for compatibility
    uint64_t engineFrameId;                   // Engine timeline reference (0 = apply immediately)
    uint64_t uiTimestampMicros;              // UI timestamp for latency tracking
    
    ParameterUpdate() noexcept 
        : parameterId(0), normalizedValue(0.0f), sampleOffset(0), 
          version(1), engineFrameId(0), uiTimestampMicros(0) {}
    
    ParameterUpdate(uint32_t id, float value, uint32_t offset = 0) noexcept
        : parameterId(id), normalizedValue(value), sampleOffset(offset),
          version(1), engineFrameId(0), uiTimestampMicros(0) {}
};

// MaskColumn for GPU → Audio Thread communication
#if _MSC_VER
  #pragma warning(push)
  #pragma warning(disable:4324)
#endif

struct MaskColumn
{
    static constexpr size_t MAX_BINS = 1024; // Support up to 2048 FFT (1024 bins)
    
    alignas(32) float values[MAX_BINS];      // Mask values 0..1 
    uint32_t frameIndex;                     // FFT frame this applies to
    uint32_t numBins;                        // Actual number of bins (≤ MAX_BINS)
    double timestampSamples;                 // Sample-accurate timing
    uint64_t uiTimestampMicros;              // High-res UI timestamp for latency tracking
    uint32_t sequenceNumber;                 // For drop detection and debugging
    uint32_t version;                        // Message schema version for compatibility
    uint64_t engineFrameId;                  // Engine timeline reference
    
    MaskColumn() noexcept
    {
        std::memset(values, 0, sizeof(values));
        frameIndex = 0;
        numBins = 0;
        timestampSamples = 0.0;
        uiTimestampMicros = 0;
        sequenceNumber = 0;
        version = 1;
        engineFrameId = 0;
        
        // Initialize to 1.0 (no masking by default)
        for (size_t i = 0; i < MAX_BINS; ++i)
        {
            values[i] = 1.0f;
        }
    }
};

// RT-SAFE: Static validations for all message types
RT_SAFE_TYPE_ASSERT(MaskColumn);
RT_SAFE_TYPE_ASSERT(SpectralFrame);
RT_SAFE_TYPE_ASSERT(ParameterUpdate);

static_assert(std::is_trivially_copyable_v<MaskColumn>, "MaskColumn must be POD for queue safety");
static_assert(alignof(MaskColumn) == 32, "MaskColumn alignment changed - potential cache issues");
static_assert(offsetof(MaskColumn, values) == 0, "values array must be at offset 0");

// Lock-free atomic validations for key types
LOCK_FREE_ATOMIC_ASSERT(size_t);
LOCK_FREE_ATOMIC_ASSERT(uint32_t);
LOCK_FREE_ATOMIC_ASSERT(uint64_t);
LOCK_FREE_ATOMIC_ASSERT(float);

#if _MSC_VER
  #pragma warning(pop)
#endif

// Type aliases for specific queue configurations using improved SpscRingBuffer
using SpectralDataQueue = SpscRingBuffer<SpectralFrame, 16>;      // Audio → UI
using ParameterQueue = SpscRingBuffer<ParameterUpdate, 64>;       // UI → Audio  
using MaskColumnQueue = SpscRingBuffer<MaskColumn, 64>;           // GPU → Audio
using SampleQueue = SpscRingBuffer<SampleMessage, 8>;             // UI → Audio

// RT-safe assertions are now defined in RealtimeSafeTypes.h to avoid redefinition