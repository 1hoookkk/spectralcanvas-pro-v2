# SpectralCanvas Pro - Development Partnership

This is a passion project: a revolutionary spectral manipulation synthesizer. We build production code together with you guiding architecture and catching complexity early.

## Core Vision & Mission

**One-sentence mission**: Make spectral creation immediate, musical, and playable — where visual gestures become reliable musical outcomes.

**Key philosophy**: Every brushstroke must create musically sensible, expressive results. We're building an instrument where an image *is* the sound, inspired by MetaSynth and spectral art traditions.

## Current Implementation Status

**Architecture**: JUCE-based audio plugin with DirectX 11 GPU rendering, C++20, CMake build system
**Threading**: Lock-free Audio ↔ UI ↔ GPU communication via SPSC queues
**Current Phase**: Core infrastructure and tiled atlas system implemented
**Next Priority**: Progressive spectrogram display and real-time painting

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
UI Thread → Audio Thread: ParameterQueue (SPSC)  
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

**Core Delta Message** (frozen format):
```cpp
struct MaskColumnDelta {
    uint32_t tileId;
    uint16_t colInTile; 
    uint16_t bins; // == atlas::TILE_H
    uint32_t generation;
    float    gain[atlas::TILE_H];
};
```

## File Organization & Boundaries

### Core System (`Source/Core/`)
- `AtlasIds.h` - Canonical constants (single source of truth)
- `MessageBus.h` - Lock-free SPSC queue templates  
- `TiledAtlas.{h,cpp}` - SpectralTile + MaskTile management
- `RealtimeSafeTypes.h` - RT-safe containers and assertions

### DSP Processing (`Source/DSP/`)
- `SpectralEngine.{h,cpp}` - STFT/iFFT with RT-safe spectral buffers
- `HopScheduler.{h,cpp}` - Audio thread hop processing and mask application
- `OfflineStftAnalyzer.{h,cpp}` - Background spectral analysis

### GPU Rendering (`Source/Viz/`)  
- `GpuRenderer.h` - Abstract cross-platform interface
- `D3D11Renderer.{h,cpp}` - Windows DirectX 11 implementation
- `RenderThread.h` - GPU thread management
- `shaders/` - HLSL compute/vertex/fragment shaders

### UI Components (`Source/GUI/`)
- `SpectrogramComponent.{h,cpp}` - Main canvas with painting
- `BrushTools.{h,cpp}` - Musical brushes (attenuation, harmonic)
- `PerfHUD.{h,cpp}` - Real-time performance monitoring

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

## Current Phase Actions

Based on the FINAL SPEC roadmap, we're currently implementing:

### PHASE 1 - Core Plumbing (In Progress)
- ✅ `Source/Core/AtlasIds.h` (constants)
- ✅ `Source/Core/MessageBus.h` (MaskColumnDelta + SPSC)  
- ✅ `Source/Core/TiledAtlas.h/.cpp` (SpectralTile + MaskTile)
- 🔄 `Source/DSP/HopScheduler.h/.cpp` (prepare, applyMaskDeltas, tick)
- 🔄 Wire PluginProcessor/Editor with currentColumn atomic

### PHASE 2 - Progressive Spectrogram (Next)
- Implement `Source/Analysis/OfflineStftAnalyzer.h/.cpp`
- Per-column upload path in SpectrogramComponent
- File-load UI (WAV/AIFF) integration

## Commit Message Format

Use exact titles from FINAL SPEC for consistency:
- `Build: core tiled atlas + message bus + cursor scaffold`
- `GUI: progressive spectrogram (per-column uploads)`
- `Tools: attenuation brush + column delta emitter`
- `DSP: apply mask per hop (WYSIWYH)`

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
