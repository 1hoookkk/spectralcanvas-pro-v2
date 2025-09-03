# Repository Guidelines

This repo hosts the SpectralCanvas Pro JUCE plugin. Use CMake-based builds, keep audio-thread code RT-safe, and follow the commit/PR conventions below.

## Project Structure & Modules
- `Source/`: main code
  - `Core/` threading, queues, params, atlas
  - `DSP/` engines, loaders, STFT, schedulers
  - `GUI/` editor, HUD, canvas components
  - `Audio/` sample I/O; `Viz/` GPU backends; `nodes/` DSP nodes; `rt/` RT-lite helpers
- `Tests/`: CTest-driven executables (some use GoogleTest)
- `docs/`, `CMakeLists.txt`, `CMakePresets.json`, `pluginval.cmake`

## Build, Test, Run
Examples (Windows/MSVC); adjust for your toolchain.
- Configure: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON`
- Presets: `cmake --preset x64-debug` then `cmake --build --preset x64-debug`
- Build: `cmake --build build --config Debug`
- Tests: `ctest --test-dir build --output-on-failure`
- Pluginval: `cmake --build build --target pluginval_quick` (full: `pluginval_test` → results in `build/pluginval_results`)
- Outputs (examples): `build/SpectralCanvasPro_artefacts/Debug/VST3/SpectralCanvasPro.vst3`, `.../Standalone/SpectralCanvasPro.exe`

Common options: `-DSCP_INSTALL_SHADERS=ON` (stage HLSL), `-DENABLE_TESTS=ON`, `-DDISABLE_GPU_RESILIENCE=ON`, `-DSPECTRAL_ENABLE_MP3=OFF`, `-DDISABLE_PERF_HUD=ON`.

## Coding Style & Naming
- C++20, 4-space indentation, warnings: `/W4` (MSVC) or `-Wall -Wextra -Wpedantic`.
- Types/classes and files: PascalCase (`SpectralCanvasProAudioProcessor`), methods/functions: camelCase, constants/macros: UPPER_SNAKE_CASE.
- Keep headers/sources paired; no allocations/locks/logging in `processBlock()`.

## Testing Guidelines
- Frameworks: CTest; selected targets use GoogleTest.
- Add focused tests under `Tests/` (pattern: `FooThingTest.cpp`) and register in `Tests/CMakeLists.txt`.
- Run: `ctest ...`; performance/RT safety via `PerfHarness`, `RTSafetyTest`, and pluginval targets.

## Commit & PR Guidelines
- Follow commit slice titles from README’s roadmap. Examples:
  - `Build: core tiled atlas + message bus + cursor scaffold`
  - `GUI: progressive spectrogram (per-column uploads)`
- PRs: clear description, linked issue, build/test commands, relevant logs; add screenshots for UI changes and perf numbers for audio-thread changes. Include pluginval results when applicable.

## RT Safety & Configuration
- Use SPSC queues (`Source/Core/MessageBus.h`) for cross-thread messaging.
- Validate with `pluginval_rt_safety` and `RTSafetyTest` before merging.
