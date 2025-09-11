# SpectralCanvas Context Bridge

This document provides essential background from SpectralCanvasPro, curated for the lean SpectralCanvasLite workspace.

## What SpectralCanvasLite Is
- Clean, minimal JUCE plugin foundation.
- Implements working paint→audio ducking (stable base for rapid iteration).
- Use this as the authoritative starting point; integrate advanced features incrementally.

## Where the Deep Research Lives
- Research Asset Vault (4000+ files, high‑value IP): `C:\SpectralCanvasPro_Research_Assets`
- Key docs mirrored here if present:
  - `docs/BLUEPRINT.md`
  - `docs/Developer-Notes.md`
  - `CLAUDE.md`
  - `sessions/handoff/CURSOR_CONTEXT.md`

## Snapshot of Prior State (SpectralCanvasPro)
- VST3 + Standalone builds succeeded, but functionality was obscured by bloat and routing complexity.
- Paint→audio pipeline was ~90% complete via Phase4Synth; routing and snapshot publishing were key gaps.
- ~4000 files created include ambiguity, symbol conflicts, and slow iterations.

### Confirmed Architecture Elements
- UI→Audio transport via lock‑free SPSC for paint events.
- STFT pipeline and mask application (Synth/Resynth) designed with RT‑safety constraints.
- Debug overlay and snapshot bus exist but required an explicit publish call per block.

### Known Fixes/Insights From Pro
- Route processing to a single authoritative path (Phase4Synth) when enabled.
- On paint events, update a gain target; smooth and apply in the audio thread.
- Publish canvas snapshots each `processBlock()` to feed HUD/overlay.

## Why We’re Lean
- The bloat is valuable research, not trash—but it must be decoupled.
- SpectralCanvasLite keeps builds fast and intent clear while we selectively mine the vault.

## How To Integrate Research Safely
1. Start from a working feature in Lite.
2. Identify the minimal research component needed (single class/algorithm).
3. Port into a new isolated module/namespace with explicit deps.
4. Add focused tests; gate behind a compile flag.
5. Measure RT‑safety and performance before enabling by default.

## Immediate Next Steps
- Keep Lite as the product base.
- Organize the Research Asset Vault by domain (AI, Psychoacoustics, GPU, Synthesis, UI, Telemetry).
- Add links from this doc as areas are cataloged.

## Success Checklist (reference from SpectralCanvasPro)
- Plugin loads in DAW without impacting transport/metronome
- Build succeeds reliably (RelWithDebInfo)
- UI parameters respond; values persist as expected
- Real‑time safety maintained in `processBlock` (no allocs/locks/I/O)
- DAW audio input accepted (mono/stereo) via `isBusesLayoutSupported`
- `processEnabled` default is enabled for immediate processing
- Paint events route to audio path (Synth or Resynth) and are audible
- Canvas debug overlay shows live status when snapshots are published
- Basic tests green; plugin validation passes (strictness ≥ 8)

## Immediate Fixes (from SpectralCanvasPro backlog)
- Publish snapshots: Call `publishCanvasSnapshot()` at end of each `processBlock()`
- Authoritative path: Route enabled processing to a single, known path (e.g., Phase4Synth)
- Paint path hookup: Drain SPSC queue (`convertMaskColumnsToDeltas`) and apply gains before resynthesis
- UI mapping fixes: Use brush strength for intensity; correct column X mapping; unify Y (top = high)
- Guarding: Remove/relax UI guards that reject paint when the selected path is active
- Telemetry: Wire HUD counters (`processedBlocks`, `maskQueueDepth`, `drainsThisBlock`)

## Validation Commands
```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Build (Windows single-threaded)
cmake --build build --config RelWithDebInfo -- /m:1

# Tests (if present)
ctest --test-dir build --output-on-failure

# Validate plugin (adjust path as needed)
pluginval --strictness-level 8 "build/SpectralCanvasLite_artefacts/RelWithDebInfo/VST3/SpectralCanvasLite.vst3"
```

## DAW Test Procedure
- Insert plugin on a track with audio; confirm host transport/metronome unaffected
- Toggle the processing/enable control; verify audible change only when enabled
- Paint/drag in the UI; confirm immediate audible ducking/alteration
- Observe CPU; verify steady, bounded per-block work with no xruns
- Optionally run plugin validation at higher strictness (8–10)

— End of curated context —

## Current Functionality (Lite) vs Session Context (Pro)
- Lite now demonstrates the minimal, reliable behavior: click-to-duck via atomic gain, no allocations/locks in `processBlock`.
- The session context (Pro) targeted a full STFT paint→audio pipeline (Phase4Synth/Resynth) with HUD and snapshot bus.
- Strategy: keep Lite as the product base; mine specific modules from Pro (queues, MaskColumn, Atlas constants, snapshot bus), one at a time.

## PR1 Roadmap (Adapted for Lite)
- P1: Telemetry baseline
  - Add per-block counters and a minimal snapshot struct; publish every `processBlock()`.
  - Show simple text overlay (processed blocks, queue depth placeholder) to validate the UI path.
- P2: Event transport scaffold
  - Introduce SPSC queue and `MaskColumn` with bounded drain (≤16). No DSP changes yet.
- P3: Audible path
  - Integrate a minimal `SpectralEngineStub` style processor behind a compile flag; apply mask-derived gains.
- P4: UI mapping polish
  - Brush intensity, column mapping, unify Y (top = high), basic parameterization.

## Repository Strategy
- Pro = Research Vault (≈4000 files): valuable IP, but complex and intertwined.
- Lite = Product Core: fast build, clear ownership, feature flags for mined modules.
- Rule: one research module per PR; tests + pluginval before enabling by default.
