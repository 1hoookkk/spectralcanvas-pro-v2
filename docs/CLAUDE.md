# CLAUDE.md — Project Development Contract

This file provides guidance to Claude Code when working with this SpectralCanvasPro codebase.

## Project Overview

Real-time audio plugin with spectrogram painting → STFT masking (WYSIWYH).
Tech stack: JUCE C++, GPU rendering, SPSC lock-free queues.

## Non-Negotiable RT-Safety Rules

**Audio thread constraints:**
* Zero heap allocations, mutex locks, or file I/O in `processBlock()`
* Bounded queue drains (≤16 deltas/block maximum)
* Hard clamp output to ±1.0f at end of each block
* **Latency policy:** Centered STFT (report FFT_SIZE/2 to host)

**UI↔Audio contract:**
* SPSC queues for lock-free communication
* Atomic page flips for mask data
* UI writes staging; audio reads active page
* Generation counters prevent stale writes

## Validation Requirements

**Must pass before merge:**
* E2E tests: Impulse latency measurement ≈ FFT_SIZE/2
* E2E tests: Sine attenuation when painted 
* FFT health: Delta round-trip error < 1e-6, all finite values
* RT safety: No allocs/locks detected in `processBlock()`
* `pluginval --strictness-level 6` minimum (level 8 preferred)
* CMake grep gates: Forbid RT-unsafe patterns on audio thread

## Workflow Rules

**Development process:**
* Plan Mode → small PR slices → tests → merge
* One working audio path before adding features
* No `_Fixed` or versioned function names
* Delete old code completely (no deprecation)

**Code organization:**
* Keep functions small and focused
* Prefer explicit over clever abstractions
* Clear data flow over hidden magic
* Group related functionality logically

## Build & Testing

**Essential commands:**
```bash
# Build & validate
cmake --build build --config RelWithDebInfo
pluginval --strictness-level 8 build/**/SpectralCanvasPro.vst3
```

**File boundaries:**
* **Safe to edit:** `Source/Core/`, `Source/DSP/`, `Source/GUI/`
* **Protected:** Plan-only proposals for major engine changes
* **Never touch:** `build/`, `*_deps/`, `JUCE/` (generated/external)

## Performance Targets

* Audio latency: <5ms paint→audio
* CPU: <15% @ 48kHz/64 samples  
* Memory: 0 allocations in `processBlock()`

## STFT Implementation Details

**Real-only FFT (JUCE):**
* Uses `performRealOnlyForwardTransform` / `performRealOnlyInverseTransform`
* Buffer layout: DC at `[0]`, Nyquist at `[N]`, bins 1..(N/2-1) as re/im pairs
* JUCE inverse applies 1/N scaling automatically
* DC/Nyquist bins are real-only; mask applied once each

**Processing order (critical):**
1. Set current column → 2. Drain paint queue (≤16) → 3. FFT → 4. Apply masks → 5. iFFT+OLA → 6. Advance mask ring

**Mask semantics:**
* Runtime: One-pole smoothing `g_t = (1-α)·g_paint + α·g_prev`; decay to 1.0 when no paint
* Test bypass: Write exact paint gain directly; no smoothing/decay
* Column alignment: Audio reads from current active column (no staging)

## Style Guidelines

* Boring but bulletproof implementations
* Simplicity over features
* Clarity over abstraction
* Short, descriptive commit messages

