# Doc–Code Consistency Audit

This document captures mismatches between docs and the current codebase so contributors can align expectations and prioritize fixes. Note: A large portion of code/docs were AI-synthesized; treat this as the source of truth until items are resolved.

## Summary (Current State)
- Rendering backend: Direct3D 11 only, with WARP fallback. No OpenGL renderer present.
- Shaders: HLSL sources exist under `Source/Viz/shaders/`, but are not compiled/packaged via CMake (commented out in `CMakeLists.txt`).
- DSP features: No `ZPlaneFilter` or `TubeSaturation` modules present.
- Tests: Core tests exist (RT, queues, process block, GPU resilience, sample load, paint node, perf harness). Some referenced tools are not present.

## Mismatches
- Renderer
  - Docs reference `OpenGLRenderer.{h,cpp}`: NOT PRESENT.
  - Docs reference `ParticleSystem.{h,cpp}`: NOT PRESENT as C++; HLSL `ParticleSystem.hlsl` exists.
- DSP
  - Docs reference `ZPlaneFilter.{h,cpp}`: NOT PRESENT.
  - Docs reference `TubeSaturation.{h,cpp}`: NOT PRESENT.
- Tests/Tools
  - Docs reference `GPUProfilerTest`, `WarpFallbackTest`: NOT PRESENT.
  - Present tests: `RTSafetyTest`, `ProcessBlockTest`, `QueueTest`, `GPUResilienceTest`, `PerfHarness`, `SampleLoadTest`, `SpectralPaintNodeTest`.
- Build Integration
  - `CMakeLists.txt` comments out shader sources; HLSL files do exist under `Source/Viz/shaders/*`.

## Recommendations
- Documentation
  - Mark missing modules as “Planned” to prevent confusion.
  - Replace OpenGL fallback wording with “D3D11 + WARP fallback”.
  - Update test command examples to match existing test binaries.
- Build System
  - Added `SCP_INSTALL_SHADERS` CMake option to stage HLSL assets next to built binaries (default ON).
  - Keep `ENABLE_TESTS` guarded; note that Googletest fetch requires network.
- Code Hygiene
  - Consider removing or quarantining legacy files (e.g., `MessageBus_Old.h`, `CanvasComponent_Backup.cpp`) or add a `legacy/` folder.

## Next Steps (Proposed)
1) Patch docs to label planned vs implemented components.
2) Add README that links to `plan.md` and key docs.
3) Add CMake option `SCP_INSTALL_SHADERS=ON` to stage shaders into artefacts (non-blocking).
4) Create follow-up issues for missing DSP modules and renderer variants.
