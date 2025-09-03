# SpectralCanvas Pro — Project Overview

A JUCE-based spectral synthesis and painting plugin/standalone app that lets users “paint sound” on a canvas. The engine performs real-time STFT spectral processing while a GPU-accelerated renderer visualizes audio in sync. Targets VST3 and Standalone on Windows (D3D11), with RT-safe audio at <5ms latency.

## Quick Start
- Prereqs: CMake ≥ 3.22, Visual Studio 2022 (MSVC), Windows SDK 10+, Git.
- Configure: `cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DENABLE_TESTS=ON`
- Build: `cmake --build build --config RelWithDebInfo`
- Run (Standalone): `build/SpectralCanvasPro_artefacts/RelWithDebInfo/Standalone/SpectralCanvasPro.exe` (or your chosen config).
- Run (VST3): `build/SpectralCanvasPro_artefacts/RelWithDebInfo/VST3/SpectralCanvasPro.vst3` — copy to your VST3 folder if needed.
- Tests: `ctest --test-dir build --output-on-failure` or run binaries in `build/Tests/`.

## Tech Stack
- C++20, CMake, JUCE 8.0.8 (audio, GUI, formats, DSP)
- Windows: Direct3D 11 GPU rendering with WARP fallback
- Formats: VST3 + Standalone

## Repository Layout
- `Source/` — application code
  - `Core/` — RT-safe primitives: message bus, parameters, telemetry, ring buffers
  - `DSP/` — spectral engine, STFT/iFFT, masks, sample loading, player, schedulers
  - `Audio/` — sample/preset management and audio pipeline glue
  - `GUI/` — canvas, HUD, top/bottom strips, spectrogram component
  - `Viz/` — GPU renderer abstraction, D3D11 backend, render thread, device recovery
  - `nodes/` — modular processing nodes (e.g., `SpectralPaintNode`)
  - Plugin entry: `SpectralCanvasProAudioProcessor.*`, `SpectralCanvasProEditor.*`, `SpectralPaintProcessor.*`
- `Tests/` — RT safety, queue, process block, GPU resilience, paint-node and sample-loading tests
- `docs/` — architecture blueprint, developer notes, validation notes and snippets
- `CMakeLists.txt` — plugin targets, feature flags, JUCE integration, platform libs

## Build Options (CMake)
- `-DENABLE_TESTS=ON` — build test executables (default OFF in main CMake)
- `-DDISABLE_GPU_RESILIENCE=ON` — disable device-lost recovery + WARP fallback
- `-DDISABLE_PERF_HUD=ON` — compile out the performance HUD overlay
- `-DSPECTRAL_ENABLE_MP3=OFF` — disable MP3 support and Windows Media Foundation usage

## Architecture
- Threads: Audio (RT), UI (message), GPU (render). Communication via lock-free SPSC queues.
- Audio Path: `SpectralEngine` performs STFT/iFFT, applies masks/filters, and drives playback.
- UI/Gestures: Canvas interaction generates gestures/parameters, sent to audio via parameter queue.
- GPU Renderer: `GpuRenderer` with `D3D11Renderer` implements frame loop, texture updates, and particles.
- RT Safety: No allocations/locks in audio thread; fixed-size buffers and atomics.

## Key Components
- Core
  - `Params.h` — parameter IDs, ranges, normalization
  - `MessageBus.h`, `SpscRingBuffer.h` — lock-free queues
  - `RealtimeSafeTypes.h`, `RealtimeMemoryManager.h` — RT-safe types/utilities
  - `DiagnosticLogger.*`, `LatencyTracker.h`, `GPUStatus.h` — observability
- DSP
  - `SpectralEngine.*`, `HopScheduler.*`, `KeyFilter.*`, `SpectralModel.*`, `SpectralPlayer.*`
  - `SampleLoader.*` + `Core/SampleLoaderService.*` — disk → memory, decoding, preparation
  - `SpectralMask.h`, `MaskTestFeeder.*` — applying masks to spectral frames
- GUI
  - `CanvasComponent.*`, `SpectrogramComponent.*` — visual interaction/feedback
  - `PerfHUD.*`, `TopStrip.*`, `BottomBar.*`, `ToastManager.*`
- Viz
  - `GpuRenderer.*`, `backends/D3D11Renderer.*`, `RenderThread.h`, `DeviceLostHandler.h`
- Nodes
  - `SpectralPaintNode.*` — paint gestures → spectral operations

## Development Workflow
- Coding standards: C++20, consistent headers/impl pairs, avoid one-letter vars. Follow RT-safety rules from `docs/Developer-Notes.md`.
- Preferred patterns: dependency-injected services, pure functions where possible, fixed-size containers in RT.
- Compiler warnings: `/W4` (MSVC) or `-Wall -Wextra -Wpedantic`.
- Tests
  - Configure: `cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON`
  - Run all: `ctest --test-dir build`
  - Useful bins: `RTSafetyTest`, `ProcessBlockTest`, `QueueTest`, `GPUResilienceTest`, `PerfHarness`, `SampleLoadTest`, `SpectralPaintNodeTest`

## Common Tasks
- Add a parameter
  1) Define ID/range in `Source/Core/Params.h`
  2) Wire to `SpectralCanvasProAudioProcessor` parameter layout
  3) Read via relaxed atomics in RT code; send updates via parameter queue
- Implement a DSP node
  1) Create `Source/nodes/MyNode.{h,cpp}`
  2) Integrate with `SpectralPaintProcessor` or the appropriate pipeline stage
  3) Add a targeted test under `Tests/` (mirror existing patterns)
- Handle GPU device-lost
  - Use `DeviceLostHandler` hooks; recreate all D3D resources
  - Prefer staging buffers for texture uploads; drop frames over blocking

## Troubleshooting
- JUCE fetch/build issues: ensure Git and network access are available (or vendor JUCE locally).
- Missing D3D libs: install Desktop development with C++ workload and Windows SDK.
- Plugin not visible: confirm `.vst3` is in your host’s VST3 directory.
- Device lost / black screen: run `GPUResilienceTest` and confirm WARP fallback.

## References
- High-level blueprint: `docs/BLUEPRINT.md`
- RT and GPU safety checklist: `docs/Developer-Notes.md`
- Tests configuration: `Tests/CMakeLists.txt`
- Ownership: `CODEOWNERS`

---
Questions or stuck? Open an issue with the failing step, your toolchain version, and any logs from `build_output.log`.

