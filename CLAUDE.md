# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

* Real-time audio plugin with spectrogram painting → STFT masking (WYSIWYH).
* Core requirement: **RT-safe DSP** (no locks, no heap allocations in `processBlock`).
* Latency must equal `FFT_SIZE - HOP_SIZE`.

## Non-Negotiable Rules

* **Always** use `AtlasConfig::{FFT_SIZE, HOP_SIZE, NUM_BINS, TILE_WIDTH, TILE_HEIGHT}`.
* **Never** allocate heap memory, lock mutexes, or resize vectors in `processBlock`.
* **Copy only one column (NUM_BINS floats) per delta** — never whole tiles.
* `setLatencySamples(FFT_SIZE - HOP_SIZE)` and assert it.
* Wrap each `processBlock` with `juce::ScopedNoDenormals`.
* Internal processing is **mono** → replicate identical output to all channels.
* Preserve phase: scale FFT bins uniformly by mask magnitudes.

## Architecture Overview

**Threading Model**: Audio ↔ UI ↔ GPU via lock-free SPSC queues
```
┌─────────────────┐    Lock-Free SPSC    ┌────────────────┐    Lock-Free SPSC    ┌─────────────────┐
│   Audio Thread  │◄─────────────────────►│   UI Thread    │◄─────────────────────►│   GPU Thread    │
│                 │   SpectralDataQueue   │                │  RenderCommandQueue  │                 │
│ • STFT/iFFT     │                       │ • Mouse/Touch  │                      │ • D3D11 Render  │
│ • Mask Apply    │   MaskDeltaQueue      │ • Painting     │   DeviceEventQueue   │ • Particles     │
│ • <5ms Latency  │◄─────────────────────►│ • 60fps Timer  │◄─────────────────────►│ • 60fps Target  │
│ • ZERO Allocs   │                       │ • Parameter UI │                      │ • WARP Fallback │
└─────────────────┘                       └────────────────┘                      └─────────────────┘
```

**Key Data Flow**: UI Painting → MaskDeltaQueue → HopScheduler → MaskAtlas → RealtimeSpectral → Audio Output

## Key Constants & Data Structures

**Atlas Configuration** (`Source/Core/AtlasIds.h`):
```cpp
FFT_SIZE = 512
HOP_SIZE = 128  
NUM_BINS = FFT_SIZE / 2 + 1  // 257
TILE_HEIGHT = NUM_BINS       // 257
TILE_WIDTH = 512
```

**Core Delta Message**:
```cpp
struct MaskColumnDelta {
    AtlasPosition position;     // tileX, tileY, columnInTile, binStart
    float values[NUM_BINS];     // Attenuation values 0.0-1.0
    // + metadata for timing/sequencing
};
```

## File Boundaries

**Safe to edit:**
* `Source/Core/` - MaskAtlas, AtlasIds, ColumnOps, MessageBus
* `Source/DSP/` - HopScheduler, RealtimeSpectral, SpectralEngine
* `Source/GUI/` - SpectrogramComponent, BrushTools, PerfHUD
* `Source/SpectralCanvasProAudioProcessor.*` - Main plugin integration

**Protected paths** (from `.cursorrules`):
* `Source/audio/**` - PROTECTED RT PATH - Plan-only proposals required
* `Source/engine/**` - PROTECTED ENGINE PATH - Never edit directly

**Never touch:**
* `build/`, `*_deps/`, `JUCE/` - Generated/external files
* Auto-generated CMake files, third-party dependencies

## Commands

**Build & Validation:**
```bash
# Standard build
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSCP_INSTALL_SHADERS=ON
cmake --build build --config RelWithDebInfo

# Essential validation (REQUIRED before commits)
pluginval --strictness-level 8 build/SpectralCanvasPro_artefacts/RelWithDebInfo/VST3/SpectralCanvasPro.vst3

# RT-safety testing
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_RT_SAFETY=ON
build/Tests/RTSafetyTest

# Run all tests
ctest --test-dir build -V
```

## Coding Standards

* Use C++20 (no exceptions on audio thread).
* Initialize all buffers in `prepareToPlay`.
* Keep headers lightweight — inline only where performance-critical.
* All DSP paths must be RT-safe: preallocated buffers, no logging, no I/O.
* Follow existing patterns in `Source/Core/` for RT-safe containers.

## Development Workflow

1. **Plan Mode first** for complex changes (>3 files or architecture changes).
2. **Research Phase**: Check `docs/BLUEPRINT.md`, `docs/Developer-Notes.md` first.
3. Workflow: *Research → Plan → Implement → Validate*.
4. **Always** run formatters, linters, and tests after implementation.
5. Test early with pluginval before major changes.
6. Use HUD instrumentation: surface `getDeltaDrainsPerBlock()` for debugging.

## Performance Targets (Non-Negotiable)

* **Audio latency**: <5ms (p50), <10ms (p99) paint→audio
* **GPU framerate**: 60fps sustained  
* **CPU usage**: <15% @ 48kHz/64 samples
* **Memory**: 0 allocations in `processBlock()`

## Documentation Ecosystem

Essential reading before major changes:
* `docs/BLUEPRINT.md` - Complete technical architecture and thread contracts
* `docs/Developer-Notes.md` - RT-safety checklist, patterns, and emergency procedures  
* `README.md` - Project overview, build instructions, and quick start
* `.cursorrules` - File modification permissions and protection rules

## Testing Requirements

* **Must pass** `pluginval --strictness-level 8` before any commit.
* **Manual test plan**:
  * Paint highs → highs attenuate in real-time
  * Paint lows → bass attenuates in real-time  
  * HUD shows `drains/block > 0` while painting
  * Latency reports as `FFT_SIZE - HOP_SIZE` (384 samples)

## Emergency Procedures

**Audio Dropouts:**
1. Check `processBlock()` for new allocations
2. Verify queue capacities aren't exceeded
3. Disable non-essential features temporarily

**GPU Issues:**
1. Force WARP fallback: `D3D_DRIVER_TYPE_WARP`
2. Disable particle system: `particlesEnabled_ = false`
3. Check for resource leaks with debug layer

**Build Failures:**
1. Clean build: `rm -rf build && cmake -B build`
2. Check JUCE version: should be 8.0.8
3. Update dependencies: `git submodule update --recursive`

## Commit Policy

* **Title**: ≤6 words, imperative mood
* **Body**: What changed, why, and RT-safety guarantees
* **Example**:
  ```
  Enable RT STFT masking

  Implements Phase 4 (HEAR): double-buffered MaskAtlas with atomic flip, 
  HopScheduler draining ≤16 UI column-deltas per block, and a preallocated 
  RealtimeSpectral STFT→mask→iSTFT→OLA path. Verified no allocations in 
  processBlock and correct latency reporting.
  ```

## Current Implementation Status

**Phase 4 (HEAR)**: ✅ COMPLETE - RT-safe STFT masking pipeline merged and working
* Successfully merged from `phase4/hear-stft-masking` branch (commit 79793ff)
* All compilation fixes applied:
  - Fixed JUCE WindowingFunction constructor using placement new
  - Removed duplicate getDeltaDrainsPerBlock() declarations
  - Cleaned up obsolete HopScheduler.cpp references
* Building successfully: VST3 and Standalone targets compile clean

**Ready for Next Phase**: System is now fully operational with real-time STFT masking

Remember: This is a passion project focused on making spectral manipulation immediate, musical, and expressive. Every decision should serve that goal while maintaining uncompromising RT-safety.