# SpectralCanvasLite

Real-time spectral filtering VST3 plugin with persistent mask and T60 decay. **Clean mixer effect architecture** - no psychoacoustic processing, no synthesis artifacts.

## 🚨 CRITICAL: Architecture Reminders

### What Makes This Work (DON'T BREAK!)

**Pure Effect Pipeline (SpectralProcessor.cpp:processFrame)**:
1. FFT → `decayMaskTowardUnity()` → `smoothMaskAttackRelease()` → `applyMaskToSpectrum()` → iFFT → OLA
2. **Single spectrum writer** - only `applyMaskToSpectrum()` touches magnitudes
3. **Attenuation-only** - mask ∈ [maskFloor, 1.0], never >1.0 
4. **Effect-mode default** - ProcessingMode = 0 ("Effect")

### What Was Removed (NEVER ADD BACK!)
- ❌ `PsychoacousticMaskingEngine` - caused harsh codec-style filtering
- ❌ `spectralGains_`, `magnitudeSpectrum_`, `psychoacousticThresholds_`
- ❌ `applyPsychoacousticMasking()`, `computeThreshold()`, `jndClampBins()`
- ❌ Real-time synthesis in effect mode

**Why removed**: Psychoacoustic processing is for codec compression, NOT creative filtering. Created harsh artifacts that broke the 3-week development loop.

## Build Instructions

### Prerequisites
- **Visual Studio 2022** (C++ Desktop workload)
- **CMake 3.21+** (in system PATH)
- **JUCE** installed at `C:/JUCE`

### Quick Build
```powershell
# Standard build
cmake -B build
cmake --build build --config Release

# With Tracy profiling (recommended for development)
cmake -B build -DSC_ENABLE_TRACY=ON
cmake --build build --config Release

# With AddressSanitizer (debugging only)
cmake -B build-debug -DSC_ENABLE_ASAN=ON
cmake --build build-debug --config Debug
```

### Output Location
- **Windows**: `build/SpectralCanvasLite_artefacts/Release/VST3/SpectralCanvasLite.vst3`

## Performance Monitoring (Tracy)

### Tracy Zones Instrumented
- `hop_total` - Complete processBlock timing
- `frame_dsp` - FFT/mask/iFFT processing  
- `brush_drain` - Paint event queue processing
- `applyMask` - Mask application to spectrum

### Performance Targets
- **p95 hop time**: <0.5ms @ 48kHz, N=1024, hop=256
- **Queue drops**: 0 at normal paint rates
- **Memory**: No allocations in processBlock()

### Usage
1. Build with `-DSC_ENABLE_TRACY=ON`
2. Run Tracy profiler client
3. Load plugin in DAW, paint while profiling
4. Verify p95 times in profiler

## Safety Parameters

```cpp
// Safe parameter ranges (Params.h)
kBrushStrength:  0.0→1.0  (default 0.8, consider 0.2 for safety)
kMaskFloorDb:   -60→-12   (default -24dB, never allows >1.0 gain)
kOutputTrimDb:  -12→0     (default -6dB, prevents clipping)
```

## Validation Tests

### Before Each Release
```powershell
# 1. Build validation
cmake --build build --target pluginval_strict

# 2. Unity test (silence in = silence out)
# Load plugin on silent track, verify no output

# 3. COLA test (impulse reconstruction)
# Impulse train should reconstruct flat ±0.1dB

# 4. Clipping test
# -12dBFS sine + aggressive painting should never clip
```

### Manual DAW Tests
1. **Dry/Wet 100%** - Silence in = silence out (no synth)
2. **Transport stopped** - Output goes silent (no tails)
3. **Heavy painting** - No clipping with -6dB output trim
4. **Queue stress** - Fast painting should not cause dropouts

## Architecture Overview

```
UI Thread           Audio Thread
---------           ------------
Paint Events  →     Queue (1024 capacity)
                         ↓
                    drainEvents() (max 16/block)
                         ↓
                    Persistent Mask Update
                         ↓
                    FFT → Mask → iFFT
                         ↓
                    Clean Audio Output
```

### Key Components
- **SpectralProcessor**: Core STFT + persistent mask engine
- **MaskColumn**: Paint event structure (12 bytes, trivially copyable)
- **SpscQueueLite**: Lock-free UI→audio communication
- **TracyProfiler**: Real-time performance monitoring

## Troubleshooting

### Audio Distortion/Clipping
- ✅ Verify `maskFloorDb` ≥ -60dB (no razor-thin notches)
- ✅ Increase `outputTrimDb` to -12dB temporarily
- ✅ Check no other processors touching spectrum in effect mode
- ✅ Verify sqrt-Hann windowing + 1/N iFFT scaling

### Performance Issues  
- ✅ Enable Tracy, check p95 hop times
- ✅ Verify no allocations in processBlock (use ASAN build)
- ✅ Check queue drop counters via getPerfStats()
- ✅ Reduce maxEventsPerBlock from 16 to 8

### Build Errors
- ✅ Ensure JUCE at `C:/JUCE` with CMakeLists.txt
- ✅ Use Visual Studio 2022 (not 2019)
- ✅ CMake 3.21+ required for JUCE integration

## Future Optimizations

### Phase 2: PFFFT Integration
- Replace JUCE FFT with PFFFT for 2-4x speedup
- Enable via `-DSC_USE_PFFFT=ON`
- Fixed N=1024 perfect for SIMD acceleration

### Phase 3: xsimd Vectorization  
- Vectorize mask decay/smoothing loops
- Target SSE4/AVX2 for 20-30% additional speedup
- Keep scalar fallback for compatibility

**Remember**: Always benchmark against Tracy baseline before/after optimizations!

---

# 🔑 Critical Reminders for SpectralCanvasLite

### 🚫 No Psychoacoustics in Lite

* **Do not** include `PsychoacousticMaskingEngine.h`
* **Do not** declare psycho members (`magnitudeSpectrum_`, `spectralGains_`, thresholds, etc.)
* **Do not** call psycho functions (`applyPsychoacousticMasking`, `computeThreshold`, etc.)
* **Lite is effect-only**: **FFT → decay → smooth → applyMask → iFFT → OLA**

### ⚙️ Effect-Mode Defaults

* `ProcessingMode = 0` → **Effect** is the default mode
* Synth/Hybrid modes exist in stubs only; they must not be wired in *Lite*

### 🎚️ Safe Parameter Ranges

* **Mask clamp:** attenuation-only → `mask ∈ [maskFloor, 1.0]`
* **Mask floor:** ≥ 0.1 (prevents razor-thin nulls)
* **Output trim:** −6 dB default (protects against clipping on full-scale input)
* **No boosts allowed** in Lite

### ✅ Acceptance Checks

* **Unity path:** with mask=1, dry vs wet nulls within −90 dB
* **COLA test:** impulses reconstruct flat within ±0.1 dB
* **Latency:** exactly hop size (e.g., 256 samples @ 48kHz)
* **RT-safety:** no allocs, no locks, no logging in `processBlock`
* **Performance:** p95 hop < 0.5 ms on reference CPU

### 🔒 Build Flags

* `-DSC_ENABLE_PSYCHO=OFF` must be set for Lite
* Run `pluginval --strictness 10` on every build

---

## Project History & Context

This codebase was established after archiving a larger, more complex, and unstable repository located at `C:\spectralCanvasPro`. That repository contains over 4000 files of valuable, research-grade intellectual property.

The primary breakthrough was **removing psychoacoustic processing** which was causing harsh codec-style filtering artifacts. The psychoacoustic engine is the wrong tool for creative filtering - it's designed for codec compression, not musical effects.

This clean "Lite" implementation uses a **persistent spectral mask with T60 decay** instead, providing smooth, musical filtering that broke a 3-week development loop.
