# SpectralCanvas Pro - Development Partnership

This is a passion project: a revolutionary spectral manipulation synthesizer. We build production code together with you guiding architecture and catching complexity early.

## Core Vision & Mission

**One-sentence mission**: Make spectral creation immediate, musical, and playable — where visual gestures become reliable musical outcomes.

**Key philosophy**: Every brushstroke must create musically sensible, expressive results. We're building an instrument where an image *is* the sound, inspired by MetaSynth and spectral art traditions.

## Current Implementation Status

**Architecture**: JUCE-based audio plugin with DirectX 11 GPU rendering, C++20, CMake build system
**Threading**: Lock-free Audio ↔ UI ↔ GPU communication via SPSC queues
**Current Phase**: Phase 4 (HEAR) - RT-safe STFT masking pipeline implemented
**Status**: Core implementation complete on branch `phase4/hear-stft-masking`, needs compilation fixes

## Development Workflow: Research → Plan → Implement → Validate

**Always start with**: "Let me research the codebase and create a plan before implementing."

### 1. Research Phase
- Understand existing patterns in `Source/Core/`, `Source/DSP/`, `Source/GUI/`
- Check `docs/BLUEPRINT.md` for technical architecture
- Review `docs/Developer-Notes.md` for RT-safety requirements
- Look at `CMakeLists.txt` to understand build configuration

### 2. Plan Phase  
- Propose approach and verify alignment with project vision
- Consider RT-safety implications (no allocations in `processBlock()`)
- Validate against performance targets (<5ms latency, 60fps GPU)

### 3. Implement Phase
- Build with comprehensive error handling
- Follow existing code patterns and naming conventions
- Maintain the three-thread architecture integrity

### 4. Validate Phase
- **ALWAYS** run formatters, linters, and tests after implementation
- Use `cmake --build build --config RelWithDebInfo` 
- Validate with pluginval when changing audio processing

## Critical RT-Safety Requirements

### ❌ NEVER in Audio Thread (`processBlock()`)
- No `new`, `delete`, `malloc`, `free`
- No `std::vector<>.push_back()`, `.resize()`, `.reserve()`
- No logging (`juce::Logger`, `printf`, `std::cout`)
- No mutexes, locks, or blocking calls
- No file I/O or network operations

### ✅ RT-Safe Alternatives
- Pre-allocated `std::array<T, N>`, fixed-size buffers
- Lock-free atomics: `std::atomic<T>` with `memory_order_relaxed`
- SPSC queues: `Core/MessageBus.h` templates
- Stack-allocated variables, pass by reference/value

## Architecture Principles

**This is always a feature branch**:
- Delete old code completely - no deprecation needed
- No versioned names (processV2, handleNew, ClientOld)  
- No migration code unless explicitly requested
- No "removed code" comments - just delete it

**Thread Boundaries (Sacred)**:
```cpp
Audio Thread → UI Thread: SpectralDataQueue (SPSC)
UI Thread → Audio Thread: MaskDeltaQueue (SPSC)  
UI Thread → GPU Thread: RenderCommandQueue (SPSC)
GPU Thread → UI Thread: DeviceEventQueue (SPSC)
```

**Performance Targets (Non-Negotiable)**:
- Audio latency: <5ms (p50), <10ms (p99) paint→audio
- GPU framerate: 60fps sustained  
- CPU usage: <15% @ 48kHz/64 samples
- Memory: 0 allocations in `processBlock()`

## Key Constants & Data Structures

**Atlas Configuration** (`Source/Core/AtlasIds.h`):
```cpp
FFT = 2048
HOP = 256  
BINS = FFT / 2     // 1024
TILE_H = BINS
TILE_W = 2048
```

**Core Delta Message** (Phase 4 format):
```cpp
struct MaskColumnDelta {
    AtlasPosition position;     // tileX, tileY, columnInTile, binStart
    float values[NUM_BINS];     // Attenuation values 0.0-1.0
    struct {
        uint64_t samplePosition;
        float timeSeconds;
        uint32_t sequenceNumber;
        float fundamentalHz;
        float spectralCentroid;
    } metadata;
};
```

## File Organization & Boundaries

### Core System (`Source/Core/`)
- `AtlasIds.h` - Canonical constants (single source of truth)
- `MessageBus.h` - Lock-free SPSC queue templates including MaskDeltaQueue
- `MaskAtlas.h` - Double-buffered mask storage with atomic page flipping
- `TiledAtlas.{h,cpp}` - SpectralTile + MaskTile management
- `ColumnOps.h` - RT-safe column copy operations with compile-time size enforcement
- `RealtimeSafeTypes.h` - RT-safe containers and assertions

### DSP Processing (`Source/DSP/`)
- `SpectralEngine.{h,cpp}` - STFT/iFFT with RT-safe spectral buffers
- `HopScheduler.h` - Bounded delta draining (≤16/block) with time→column mapping  
- `RealtimeSpectral.h` - RT-safe STFT→mask→iSTFT→OLA pipeline for Phase 4
- `OfflineStftAnalyzer.{h,cpp}` - Background spectral analysis

### GPU Rendering (`Source/Viz/`)  
- `GpuRenderer.h` - Abstract cross-platform interface
- `D3D11Renderer.{h,cpp}` - Windows DirectX 11 implementation
- `RenderThread.h` - GPU thread management
- `shaders/` - HLSL compute/vertex/fragment shaders

### UI Components (`Source/GUI/`)
- `SpectrogramComponent.{h,cpp}` - Main canvas with painting, wired to MaskDeltaQueue
- `BrushTools.{h,cpp}` - Musical brushes (attenuation, harmonic)
- `PerfHUD.{h,cpp}` - Real-time performance monitoring with delta drain metrics

## Phase 4 (HEAR) - RT-Safe STFT Masking Pipeline

**Data Flow**: UI Painting → MaskDeltaQueue → HopScheduler → MaskAtlas → RealtimeSpectral → Audio Output

**Key Components**:
- `MaskAtlas.h:25-45` - Double-buffered mask storage with atomic page flipping
- `HopScheduler.h:35-55` - Bounded delta draining (≤16/block) with time→column mapping
- `RealtimeSpectral.h:29-85` - Pre-allocated STFT→mask→iSTFT→OLA pipeline
- `SpectralCanvasProAudioProcessor.cpp:467-582` - Phase 4 processBlock implementation

**RT-Safety Guarantees**:
- Zero allocations in processBlock() - all buffers pre-allocated in prepare()
- Atomic page flipping for lock-free mask updates
- Bounded delta processing (≤16 deltas per audio block)
- Single column copies only (NUM_BINS floats per operation)
- Denormal protection via juce::ScopedNoDenormals

**Performance Metrics**:
- Audio latency: FFT_SIZE - HOP_SIZE (384 samples at 2048/256)
- Delta processing: Monitored via getDeltaDrainsPerBlock()
- Queue monitoring: MaskDeltaQueue depth and drop counts in PerfHUD

## Musical Helpers Philosophy

**Everything must be musical by default**:
- Brushes are pitch-aware (harmonic brush, key filter)
- Visual feedback maps to musical constructs (piano strip, harmonic grid)
- Key-aware filtering produces musically coherent results
- Harmonic relationships guide spectral modifications

## Code Style & Patterns

### Function Organization
- Keep functions small and focused
- If you need comments to explain sections, split into functions
- Group related functionality into clear packages
- Prefer many small files over few large ones

### Naming Conventions
- Follow existing project patterns
- Clear function names over clever abstractions  
- Obvious data flow over hidden magic
- Direct dependencies over service locators

### Error Handling
- `fmt::Errorf("context: %w", err)` style error chaining
- Early returns to reduce nesting
- Graceful degradation for non-critical failures

## Testing Strategy

### Match complexity to testing approach:
- **Complex business logic**: Write tests first (TDD)
- **Simple operations**: Write code first, then tests  
- **Hot paths**: Add benchmarks after implementation

### RT-Safety Validation:
```bash
# Build with RT safety checks
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_RT_SAFETY=ON

# Run pluginval validation
pluginval --strictness-level 8 build/SpectralCanvasPro_artefacts/RelWithDebInfo/VST3/SpectralCanvasPro.vst3
```

## Phase Implementation Status

### PHASE 1 - Core Plumbing ✅ COMPLETE
- ✅ `Source/Core/AtlasIds.h` (constants)
- ✅ `Source/Core/MessageBus.h` (MaskColumnDelta + SPSC queues)  
- ✅ `Source/Core/TiledAtlas.h/.cpp` (SpectralTile + MaskTile)
- ✅ `Source/Core/ColumnOps.h` (RT-safe column operations)

### PHASE 2 - Progressive Spectrogram ✅ COMPLETE
- ✅ `Source/DSP/OfflineStftAnalyzer.h/.cpp` (background analysis)
- ✅ Per-column upload in SpectrogramComponent
- ✅ File-load UI with drag-and-drop integration

### PHASE 3 - Painting Tools ✅ COMPLETE  
- ✅ UI→Audio mask delta queue system
- ✅ SpectrogramComponent painting with MaskDeltaQueue wiring
- ✅ Real-time visual feedback with PerfHUD metrics

### PHASE 4 - Audio Processing (HEAR) ✅ IMPLEMENTED
**Branch**: `phase4/hear-stft-masking` (commit 79793ff)
- ✅ `Source/Core/MaskAtlas.h` - Double-buffered mask storage with atomic flip
- ✅ `Source/DSP/HopScheduler.h` - Bounded delta draining (≤16/block) + time→column mapping
- ✅ `Source/DSP/RealtimeSpectral.h` - Pre-allocated STFT→mask→iSTFT→OLA pipeline
- ✅ Audio processor integration with Phase 4 processBlock priority
- ✅ SpectrogramComponent wired to MaskDeltaQueue for UI→Audio flow

**Status**: Implementation architecturally complete, needs compilation fixes:
1. JUCE WindowingFunction constructor compatibility (assignment operator deleted)
2. Duplicate getDeltaDrainsPerBlock() method declarations  
3. API compatibility with existing HopScheduler calls

**Integration Points**:
- UI painting flows through MaskDeltaQueue → HopScheduler.drainAndApply(≤16) → MaskAtlas.flip()
- RealtimeSpectral.processBlock() applies current mask column to STFT bins
- PerfHUD displays deltaDrains and maskDropCount for real-time monitoring
- Latency correctly reported as FFT_SIZE - HOP_SIZE (384 samples)

## Commit Message Format

Use exact titles from FINAL SPEC for consistency:
- `Build: core tiled atlas + message bus + cursor scaffold` ✅ COMPLETE
- `GUI: progressive spectrogram (per-column uploads)` ✅ COMPLETE
- `Tools: attenuation brush + column delta emitter` ✅ COMPLETE  
- `DSP: apply mask per hop (WYSIWYH)` ✅ COMPLETE (commit 79793ff)

## Problem Solving Approach

**When stuck**: Stop. The simple solution is usually correct.

**When uncertain**: "Let me research the architecture and understand the existing patterns."

**When choosing**: "I see approach A (simple) vs B (flexible). Which aligns better with the RT-safety requirements and performance targets?"

Your guidance prevents over-engineering. When uncertain about implementation, stop and ask for direction that aligns with the musical vision and technical constraints.

## Emergency Procedures

### Audio Dropouts
1. Check `processBlock()` for new allocations
2. Verify queue capacities aren't exceeded
3. Profile CPU usage with audio-thread tools  
4. Disable non-essential features temporarily

### GPU Issues  
1. Force WARP fallback: `D3D_DRIVER_TYPE_WARP`
2. Disable particle system: `particlesEnabled_ = false`
3. Check for resource leaks with debug layer

Remember: This is a passion project focused on the intersection of visual art and music creation. Every decision should serve the goal of making spectral manipulation immediate, musical, and expressive.
## Sessions System Behaviors

@CLAUDE.sessions.md
