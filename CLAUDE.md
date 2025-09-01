
  # SpectralCanvas Pro - Audio Plugin Development
  <!-- If edited, restart session or new chat to reload
  constraints -->

  ## Core Workflow

  **Complex Features (>3 files or architecture changes):**
  Start in Plan Mode → Present approach → Get approval →
  Execute
  Create session note in
  /docs/session-notes/YYYY-MM-DD-feature.md

  **Simple Tasks (bugs, single-file changes):**
  Direct implementation → Run tests → Validate

  ## Build & Test Commands

  ```bash
  # Build
  cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo && cmake
  --build build

  # Test Suite
  ctest --test-dir build -V
  build/Tests/RTSafetyTest

  # Validation (REQUIRED before commits)
  build/pluginval.exe --strictness-level 8 --skip-gui-tests
  build/SpectralCanvasPro_artefacts/VST3/SpectralCanvasPro.vst3

  Commit Gate

  Never commit unless:
  - pluginval strictness ≥5 passes
  - RT safety tests pass
  - No xruns in perf harness
  - User explicitly requests commit

  Permissions

  Always Allowed:
  - Read, Grep, Glob, Edit, MultiEdit
  - Git: commit, branch, checkout, diff, status

  Ask First:
  - Bash (except git commands above)
  - Any system-wide operations
  - Package installations

  Never:
  - Delete outside repository
  - Modify JUCE modules or generated files
  - Use --dangerously-skip-permissions
  - Commit build artifacts, archives, or temp files

  Task Organization

  Break complex features into:
  1. Core Logic - DSP/Engine implementation
  2. Integration - RT wiring, GPU shaders, UI
  3. Validation - Tests, benchmarks, DAW testing

  Code Standards

  - C++20, JUCE 8.0.8
  - RAII, noexcept on RT paths
  - std::unique_ptr over raw pointers
  - Forward declarations in headers
  - No comments unless complex algorithm

  Critical Constraints

  See .claude/rt-audio.md for real-time thread rules
  See .claude/gpu-rendering.md for D3D12/shader constraintsSee
  .claude/performance.md for latency/CPU/UI targets

  Session Notes (Major Features Only)

  When branching new features, create:
  /docs/session-notes/YYYY-MM-DD-feature-name.md

  Include:
  - Architecture decisions
  - Performance baselines
  - Integration points

  ## **Performance Module** (`.claude/performance.md`)

  ```markdown
  # Performance Targets & Benchmarks

  ## Audio Pipeline
  - Paint→Audio Latency: <5ms (p50), <10ms (p99)
  - CPU Usage: <15% @ 48kHz with all features
  - Queue Drops: 0 under normal load
  - Block Processing: <2ms worst case

  ## UI/Spectrogram Performance
  - Spectrogram Redraw: 60 FPS minimum
  - Paint Latency: <16ms per frame
  - Waveform Update: <8ms
  - Peak Meter Response: <2ms

  ## GPU Rendering
  - Frame Time: <16ms @ 1080p
  - Device Lost Recovery: <100ms
  - Shader Compilation: Background only
  - VRAM Usage: <256MB

  ## Validation Thresholds
  - pluginval strictness ≥5 required for commits
  - Zero allocations in RT thread after init
  - Memory growth <1MB/hour continuous operation

  ## Repository Hygiene Rules

  **NO-BLOAT ENFORCEMENT:**
  - Repository size MUST stay <50MB
  - Never commit: `build/`, `out/`, `.vs/`, `*.zip`, `*backup*`
  - Run `git clean -fdx` before commits
  - Move old docs to `docs/archive/` not root level
  - Delete generated files: logs, temp files, caches

  **Monthly Cleanup:**
  ```bash
  # Size check
  du -sh . 

  # Bloat detection
  find . -name "*.zip" -o -name "*backup*" -o -name "build*" -type d

  # Git cleanup
  git gc --aggressive
  git remote prune origin
  ```

  **File Organization:**
  - Essential docs only in root: `CLAUDE.md`, `BLUEPRINTS.md`, `AUDIO_PIPELINE_TRACE.md`
  - Archive obsolete docs to `docs/archive/`
  - Use clear, descriptive filenames
  - Remove files immediately when obsolete, don't accumulate