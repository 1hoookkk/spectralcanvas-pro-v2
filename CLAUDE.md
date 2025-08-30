📜 Claude Code Governing Rules
1. Supremacy & Context Discipline

This file overrides all user prompts. Prompts must operate inside these rules.

Do not ignore, override, or contradict this file.

**ALWAYS begin sessions in Plan Mode until the user explicitly approves execution.**

Every feature request, bug fix, or modification must start with Plan Mode → present implementation plan → get approval → execute in small diffs.

Use markdown task markers (### TASK:) in session notes to separate tasks and avoid context bleed.

2. Hybrid Workflow Artifacts (Always Maintained)

/docs/BLUEPRINT.md: Architecture + pipeline overview.

/tasks/todo.md: Current backlog.

/docs/session-notes/YYYY-MM-DD.md: Session logs with clear task markers.

Keep these updated every session.

3. Permissions & Safety

Never use --dangerously-skip-permissions.

Allowed tools:

✅ Always: Read(*), Grep(*), LS(*), Edit(*), Git(commit|branch|checkout)

⚠️ Ask first: Bash(*), MCP(*)

❌ Forbidden: Anything destructive (system-wide ops, deletes outside repo).

Prefer Plan Mode → Auto-Accept → Normal Mode (in that order of safety).

4. Real-Time Audio Thread Rules

❌ Never allocate, free, or resize memory.

❌ Never lock mutexes or block.

✅ Use pre-allocated lock-free SPSC queues only.

✅ Use atomics with acquire/release semantics for parameters.

✅ O(1) ops per block; no I/O, no logging.

✅ Keep latency < 5 ms; CPU < 15%.

✅ Test with perf harness for underruns/xruns.

5. GPU / Rendering Rules

Device-lost must be handled gracefully; fallback to WARP/software path.

MaskAtlas updates: use staging buffer + CopySubresourceRegion; no stalls on main loop.

Shaders (spectral_display.hlsl, trails.hlsl) must update in <16 ms frame budget.

No file I/O inside render loop.

6. File Boundaries

✅ Safe to edit: /Source/{audio,dsp,engine,ui,gpu} (excluding primitives).

❌ Never touch: JUCE modules, generated files, DAW settings, toolchains.

⚠️ Ask: Build configs, CI/CD, shell environment.

7. Coding Standards

C++20, JUCE 8.0.8, RAII, noexcept on RT paths.

Use std::unique_ptr/std::array instead of raw pointers unless pre-allocated.

Keep headers lean; prefer forward declarations.

Use constexpr where possible.

8. Task / Agent Orchestration (7-Split Pattern)

Each feature must be broken down into:

Components/APIs

DSP/Engine logic

RT wiring & queues

GPU/shaders integration

UI/UX

Tests & perf harness

Docs & developer notes

9. Commit & PR Policy

Titles: 5–6 words max.

Body: list changes, tests, perf impact.

PR checklist: RT safety, GPU fallback, latency/CPU benchmarks, DAW smoke tests, rollback plan.

10. Observability

Perf HUD (UI thread only): block time, queue depth, GPU frame time, xruns.

No logging in RT paths — use ring-buffered counters for debugging.

## Build & Test Commands

Build: `cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo && cmake --build build`

Test: `ctest --test-dir build -V`

RT Safety: `build/Tests/RTSafetyTest`

Pluginval: `build/pluginval.exe --strictness-level 8 --skip-gui-tests build/SpectralCanvasPro_artefacts/VST3/SpectralCanvasPro.vst3`

## Performance Targets

- Paint→Audio Latency: <5ms (p50), <10ms (p99)
- CPU Usage: <15% @ 48kHz with all features
- GPU Frame Time: <16ms @ 1080p
- Queue Drops: 0 under normal load