# SpectralCanvas Pro

A revolutionary spectral manipulation synthesizer that transforms visual painting gestures into musical expression. Paint sound directly on a living spectrogram canvas where "What You See Is What You Hear" (WYSIWYH).

## Current Status: Phase 2/3 Complete ✅

**Latest Build**: VST3 + Standalone passing **pluginval strictness 6**
**Stability**: All heap corruption fixes validated and stable
**New Features**: Progressive spectrogram display + painting scaffolds ready

### Implementation Progress

- ✅ **Phase 1**: Core tiled atlas + message bus + cursor scaffold
- ✅ **Phase 2**: Progressive spectrogram rendering (CPU fallback + GPU atlas)
- ✅ **Phase 3**: Painting tools + mask delta queue system
- 🔄 **Phase 4**: Audio thread mask application (next milestone)

### Key Features (Implemented)

- **Real-time spectral painting** with RT-safe mask delta system
- **Progressive spectrogram display** with bounded column updates (≤64/frame)
- **Performance HUD** showing FPS, safety counters, and system metrics
- **Lock-free audio processing** with comprehensive heap corruption protection
- **BrushTools system** with radial falloff and coalesced updates
- **Cross-platform support** (Windows complete, CI/CD ready)

## System Requirements

- **Windows**: Windows 10 (1903+) or Windows 11
- **Audio**: ASIO, DirectSound, or WASAPI compatible interface
- **CPU**: Modern multi-core processor (Intel i5/AMD Ryzen 5 minimum)
- **GPU**: DirectX 11.0+ compatible (WARP fallback included)
- **RAM**: 4GB minimum, 8GB recommended
- **Storage**: 100MB for installation

## Installation

### Pre-built Releases
Download the latest release from the releases page and run the installer.

### Building from Source

#### Prerequisites
- CMake 3.22+
- Visual Studio 2019+ (Windows) or Xcode 13+ (macOS) 
- Git with submodules support

#### Build Steps
```bash
# Clone with submodules
git clone --recursive https://github.com/your-org/SpectralCanvasPro.git
cd SpectralCanvasPro

# Configure build
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSCP_INSTALL_SHADERS=ON

# Build plugin
cmake --build build --config RelWithDebInfo

# Outputs:
# build/SpectralCanvasPro_artefacts/RelWithDebInfo/VST3/SpectralCanvasPro.vst3
# build/SpectralCanvasPro_artefacts/RelWithDebInfo/Standalone/SpectralCanvasPro.exe
```

## Quick Start

1. **Load SpectralCanvasPro** in your DAW or run standalone
2. **Load an audio file** using the "Load sample" button  
3. **Watch the spectrogram** appear as analysis progresses
4. **Paint with the brush** - left-click and drag to attenuate frequencies
5. **Listen in real-time** as the playhead reveals your edits
6. **Use musical helpers** - enable key filter and harmonic brush for musical results

## Architecture & Development

*This is a passion project focusing on the intersection of visual art and music creation.*

---

## FINAL SPEC & AUTHORITATIVE ROADMAP

*This section serves as the definitive technical specification. Do not change file/namespace/type names, constants, or commit titles without updating this document.*

## Inspiration, Idea, & Musical Vision

**Inspiration**
SpectralCanvas Pro grew from the lineage of *MetaSynth* and spectral art — the idea that an image *is* the instrument. We want visual creation to be musical: every brushstroke must create musically sensible, expressive results, whether sculpting a recorded sample or painting sound from scratch.

**Core idea**
Present a tiled spectrogram of audio (or an empty canvas) and let users paint an editable mask or paint magnitudes directly. The playhead sweeps; painted edits are heard instantly. Add musical helpers (key-aware filters, harmonic guides, pitch grid, piano-strip) so creative actions are inherently musical.

**What “musical” means here**

* Brushes are pitch-aware (harmonic brush, key filter) and produce musically coherent spectral changes.
* Visual feedback maps to musical constructs: piano-strip, semitone/octave grid, harmonic helper lines, pitch-snapping guides.
* Z-Plane morph filter and tube saturation give character that musicians expect (warmth, movement)—these are on the roadmap and will be integrated as controllable, musical effects, optionally “baked” into a hidden processing chain for demos.

**One-sentence mission**
Make spectral creation immediate, musical, and playable — where visual gestures become reliable musical outcomes.

---

## CANONICAL RULES (non-negotiable)

* **Namespaces**: `atlas`, `bus`, `analysis`, `dspx`, `viz`, `tools`.
* **Files & paths**: create and maintain the exact file paths listed in the FILES section below.
* **Constants (single source)**: `Source/Core/AtlasIds.h` is the canonical constants file. Defaults (MLP):

  ```cpp
  FFT = 2048
  HOP = 256
  BINS = FFT / 2     // 1024
  TILE_H = BINS
  TILE_W = 2048
  ```
* **Messaging & threading**: UI↔Audio communication only via `bus::SpscQueue<MaskColumnDelta, N>`. No locks/allocations in `processBlock()`. Audio thread never memcpy's whole tiles — columns only into inactive staging page, then atomic flip per tile.
* **Delta shape** (frozen):

  ```cpp
  struct MaskColumnDelta {
    uint32_t tileId;
    uint16_t colInTile;
    uint16_t bins; // == atlas::TILE_H
    uint32_t generation;
    float    gain[atlas::TILE_H];
  };
  ```
* **Commit titles**: use the exact commit slice titles in this document for PRs.

---

## FINAL FEATURE SET (what this plugin *is*)

### Core (v1 MLP)

1. **Load WAV/AIFF** (mono mixdown OK). Progressive offline analysis.
2. **Tiled spectrogram canvas** (GPU-backed when available; CPU fallback). Linear-frequency default.
3. **Attenuation brush**: 0..1 mask, radial falloff, per-column deltas, coalesced, RT-safe.
4. **Transport-accurate playback**: STFT → apply mask column → iFFT + OLA; yellow playhead cursor (atomic).
5. **Musical helpers**: piano-strip, semitone/octave grid, harmonic lines (visual), pitch LUT for later log mapping.
6. **UX**: blank canvas with “Load sample” CTA and optional “Draw synth (Beta)” toggle.

### Musical-first extensions (v1.1 & roadmap)

1. **Keyfilter** — key-aware filter: maps canvas vertical positions to scale/key; can auto-focus/amplify notes in current key; harmonically-aware attenuation/boosting.
2. **Harmonic Brush** — paint a fundamental and apply harmonics (2×,3×,…) with per-octave strength.
3. **Log-frequency shader** & pitch grid — visual alignment to musical notes; snapping options.
4. **Mipmaps/LOD** for long-file zoom levels.
5. **Draw-from-scratch (MetaSynth mode)** — paint magnitudes directly to create sound from nothing.
6. **Z-Plane morph filter** & **Tube Saturation** — musical character modules (secret source / baked options available for demos).

---

## VISUAL FEEDBACK RULES (how visuals make everything musical)

* **Piano strip** left of canvas; clicking focuses brush frequency or snaps harmonic brush.
* **Octave/semitone grid**: thin semitone lines, thicker octave lines, labeled notes at readable zoom levels.
* **Harmonic helper lines**: when harmonic brush active, highlight integer-multiple frequency lines.
* **Cursor**: bright yellow 1px with soft glow; read from `std::atomic<int> currentColumn`.
* **Mask overlay**: visual-only tint where mask < 1 (alpha scaled); do not alter base spectrogram display values — audio is the truth.
* **Color maps**: Fire (MetaSynth vibe), Greyscale, Ice; gamma control to avoid banding.
* **Keyfilter indicator**: highlights in-scale harmonics when a key is selected.

---

## DATA MODEL & THREAD CONTRACTS (authoritative)

* **SpectralAtlas (CPU FP32)**: sparse tiles, column-major storage `[col][bin]`.
* **MaskAtlas (CPU FP32 double-page)**: `page[2]`, `activePageIndex` (atomic), `generation` (atomic). UI writes staging page; audio reads active.
* **SPSC queue**: `bus::SpscQueue<MaskColumnDelta, 1024>` — UI pushes, audio pops. Coalesce before push.
* **Audio**: drains ≤16 deltas/block (configurable). Each delta `memcpy` to staging column (`sizeof(float)*TILE_H`) then `flipActivePage()`. No locks; no allocations.
* **Column→Tile math**:

  ```text
  column = floor((playheadSamples - startOffsetSamples) / HOP)
  tileId = column / TILE_W
  colInTile = column % TILE_W
  ```

---

## PHASED ACTIONS (do in order; each item is a discrete PR)

### PHASE 0 — Prep & repo hygiene

1. Add `docs/FINAL_SPEC.md` (this file) and keep `docs/MLP_STEP_BY_STEP.md` canonical for code details.
2. Move legacy files to `Source/legacy/`.
3. Ensure clean/build scripts and CMake shader option exist.
4. Link README → this document.

### PHASE 1 — Core plumbing + cursor (Build)

1. Add `Source/Core/AtlasIds.h` (constants).
2. Add `Source/Core/MessageBus.h` (MaskColumnDelta + SPSC).
3. Add `Source/Core/TiledAtlas.h/.cpp` (SpectralTile + MaskTile).
4. Add `Source/DSP/HopScheduler.h/.cpp` (prepare, applyMaskDeltas, tick).
5. Wire `PluginProcessor`/`PluginEditor` with `currentColumn` atomic and `SpectrogramComponent` (grid + cursor).
   **Commit:** `Build: core tiled atlas + message bus + cursor scaffold`.

### PHASE 2 — Offline analyzer & progressive spectrogram (SEE)

1. Implement `Source/Analysis/OfflineStftAnalyzer.h/.cpp` (worker STFT, column writes, progress/cancel).
2. Implement per-column upload path in `SpectrogramComponent` (≤64 uploads/frame).
3. Add file-load UI (WAV/AIFF) and decode via `juce_audio_formats`.
   **Commit:** `GUI: progressive spectrogram (per-column uploads)`.

### PHASE 3 — Painting + delta emission (EDIT)

1. Implement `Source/GUI/BrushTools.h/.cpp` (GainBrush, Eraser).
2. Map canvas XY → (column, bin); radial falloff; compute `gain[TILE_H]`.
3. Coalesce per frame by `(tileId,colInTile)`; push ≤128 deltas/frame to SPSC.
4. UI writes staging columns for visual immediacy.
   **Commit:** `Tools: attenuation brush + column delta emitter`.

### PHASE 4 — Apply mask in DSP (HEAR)

1. In audio thread: `hop.applyMaskDeltas(maskQ, atlas)` (bounded drain, memcpy, flip).
2. In STFT hop: compute column from playhead, load mask active column, multiply mags, iFFT+OLA, store `currentColumn`.
3. Latency: `setLatencySamples(FFT/2)` placeholder; refine later.
   **Commit:** `DSP: apply mask per hop (WYSIWYH)`.

### PHASE 5 — Musical UX & Keyfilter

1. Add **Keyfilter** control: choose musical key & scale; brush/harmonic tools respect key.
2. Piano-strip & pitch grid with pitch-snapping toggle.
3. Harmonic brush: paint fundamentals and map to harmonics.
   **Commit:** `UX: keyfilter, piano strip, harmonic brush`.

### PHASE 6 — Polish, LOD, and MLP release

1. HUD metrics, budget guards, stability tests.
2. CPU→GPU FP16 column uploads; GPU rendering optimized.
3. `Draw-from-scratch` mode gating and export pipeline.
   **Commit:** `MLP: polish, budgets, release`.

### PHASE 7 — Post-MLP advanced features

1. Z-Plane morph filter (spec file integrated as `Source/DSP/ZPlaneFilter.*`).
2. Tube saturation, character chain, Hybrid mode (Synth/Resynth blending).
3. Presets, image→audio import/export, LOD improvements.
   **Commit:** `PostMLP: advanced sound design features`.

---

## DETAILED UI BEHAVIOR (final rules)

* **Blank canvas** shows large centered CTA “Load sample”. A smaller **Draw synth (Beta)** toggle is visible and available but off by default.
* **Top strip**: Load, Mode (hidden until post-MLP), Brush size/strength, Keyfilter dropdown, Zoom controls, Freeze view, Debug HUD toggle.
* **Canvas**: main spectrogram, mask overlay, piano-strip at left (toggle), zoom & pan (mouse wheel/drag), fit-to-file button.
* **Brush input**: left-drag paints, right-drag pans, wheel changes radius, shift toggles strength.
* **On paint**: UI updates staging page and optionally draws overlay immediately. Emission to SPSC occurs per-frame coalesced.
* **Cursor**: reads `currentColumn` atomic $60 Hz UI refresh recommended$.

---

## ACCEPTANCE CRITERIA (final)

* **SEE**: Load a 1–3 minute file; spectrogram appears progressively with tiles; UI responds at ≈60 fps while analyzing.
* **EDIT**: Painting produces bounded deltas (no runaway memory), visual overlay shows changes, UI remains responsive.
* **HEAR**: Painted attenuation (or harmonic additions in harmonic brush) audible ≤ 1 hop after paint when cursor crosses; no clicks, NaNs, or xruns at 44.1/48/96 kHz and block sizes 64..1024.
* **STABILITY**: pluginval Automation tests pass (strictness 8) on Windows and macOS builds used in CI.
* **MUSICALITY**: Keyfilter + harmonic helpers produce musically coherent results by default (no user tuning required to hear musical output).

---

## RUNBOOK (final)

```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSCP_INSTALL_SHADERS=ON

# Build Standalone
cmake --build build --config RelWithDebInfo --target SpectralCanvasPro_Standalone

# Build VST3
cmake --build build --config RelWithDebInfo --target SpectralCanvasPro_VST3

# Validate (example)
pluginval --strictness-level 8 --skip-gui-tests build/SpectralCanvasPro_artefacts/RelWithDebInfo/VST3/SpectralCanvasPro.vst3
```

Windows tip: if FetchContent lock errors occur, use a fresh build folder (e.g., `build_mlprun`) or follow the clean script in `scripts/clean.ps1`.

---

## FILES — exact authoritative list (create these; do not rename)

```
docs/FINAL_SPEC.md            # (this file)
docs/MLP_STEP_BY_STEP.md     # canonical code-level spec
docs/ROADMAP_ACTIONS_PHASED.md
Source/Core/AtlasIds.h
Source/Core/MessageBus.h
Source/Core/TiledAtlas.h
Source/Core/TiledAtlas.cpp
Source/DSP/HopScheduler.h
Source/DSP/HopScheduler.cpp
Source/Analysis/OfflineStftAnalyzer.h
Source/Analysis/OfflineStftAnalyzer.cpp
Source/GUI/SpectrogramComponent.h
Source/GUI/SpectrogramComponent.cpp
Source/GUI/BrushTools.h
Source/GUI/BrushTools.cpp
Source/PluginProcessor.h
Source/PluginProcessor.cpp
Source/PluginEditor.h
Source/PluginEditor.cpp
Source/DSP/ZPlaneFilter.*     # Post-MLP (spec exists)
CMakeLists.txt
scripts/build.sh
scripts/build.ps1
scripts/clean.sh
scripts/clean.ps1
Source/legacy/*               # backups (if any)
```

## Errata & Clarifications
- generation: tracks page swaps, prevents stale updates.
- Coalescing: last-write-wins per (tileId,colInTile) each frame.
- Acceptance edge cases: 
  - File load errors → toast + recover.
  - Brush params spam → stable, no crashes.
- Draw Synth toggle → hides Load CTA and shows empty canvas.
- Musical test: harmonic brush in C major generates only in-scale harmonics.


---

## TESTS & TELEMETRY (dev responsibilities)

* Add unit/integration tests for:

  * `atlas::samplesToColumn` mapping.
  * `MaskColumnDelta` size invariants.
  * `hop.applyMaskDeltas` bounded drain & no whole-tile memcpy.
* Add HUD counters (atomics):

  * `maskQDepth`, `maskDrops`, `deltaDrainsPerBlock`, `badBinSkips`, `hopTimeUs`.
* Run pluginval + automated playback tests in CI for each PR.

---

## FINAL NOTE (from the heart)

This is the definitive, final design document for SpectralCanvas Pro. It captures the product vision, the musical-first philosophy, the technical invariants that keep the audio thread safe, and an actionable phased roadmap. Treat this project as a craft: ship the MLP (See → Paint → Hear) with musical helpers (Keyfilter, harmonic guidance) and then let the magic layers (Z-Plane, Tube saturation, MetaSynth-style painting) amplify the instrument.

If you hand this file to any engineer, AI, or collaborator, they should be able to continue work without ambiguity. Make art. Make music. Keep it safe in DAWs.
