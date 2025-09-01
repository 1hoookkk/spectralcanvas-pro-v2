#pragma once

/**
 * DEPRECATED: SpscRingBuffer.h - Use Core/MessageBus.h SpscRing instead
 * 
 * This header is deprecated to prevent implementation divergence.
 * The canonical SPSC queue implementation is in Core/MessageBus.h
 * which provides additional features like drop counting and RT-safe statistics.
 */

// Compiler warnings to encourage migration
#if defined(_MSC_VER)
  #pragma message("Warning: Source/DSP/SpscRingBuffer.h is deprecated; use Source/Core/MessageBus.h SpscRing instead")
#elif defined(__clang__) || defined(__GNUC__)
  #warning "Source/DSP/SpscRingBuffer.h is deprecated; include Source/Core/MessageBus.h and use SpscRing instead"
#endif

// Include the canonical implementation
#include "../Core/MessageBus.h"

// Compatibility typedef for existing code
template <typename T, std::size_t Capacity>
using SpscRingBuffer = SpscRing<T, Capacity>;

// Note: The old SpscRingBuffer API was:
//   bool push(const T& item)
//   bool pop(T& out) 
//   bool empty()
//
// The new SpscRing API is:
//   bool push(const T& v)
//   std::optional<T> pop()
//   bool hasDataAvailable()
//   bool hasSpaceAvailable()
//
// Migration guidance:
// - Replace pop(T& out) with auto opt = pop(); if (opt) { T value = *opt; ... }
// - Replace empty() with !hasDataAvailable()
// - The new API provides additional statistics: getPushCount(), getDropCount(), etc.