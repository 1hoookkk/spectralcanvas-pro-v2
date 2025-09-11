# SpectralCanvas Pro v2 - Technical Architecture Blueprint

Note on scope and status: This blueprint includes planned components. The current implementation focuses on JUCE + Direct3D 11 (no OpenGL renderer yet). Shader sources exist under `Source/Viz/shaders/` but are not compiled/packaged via CMake. Some DSP modules (e.g., Z-plane filter, tube saturation) are planned and not yet implemented. See `docs/DOC_CODE_AUDIT.md` for a living list of discrepancies and next steps.

## Project Vision
Revolutionary spectral manipulation synthesizer combining real-time STFT processing with EMU Audity-style Z-plane filter morphing. Users reshape a "living nebula of sound" using liquid-light painting gestures while GPU-accelerated particles provide 60fps visual feedback.

## Thread Architecture

```
┌─────────────────┐    Lock-Free SPSC    ┌────────────────┐    Lock-Free SPSC    ┌─────────────────┐
│   Audio Thread  │◄─────────────────────►│   UI Thread    │◄─────────────────────►│   GPU Thread    │
│                 │   SpectralDataQueue   │                │  RenderCommandQueue  │                 │
│ • STFT/iFFT     │                       │ • Mouse/Touch  │                      │ • D3D11/OpenGL  │
│ • Z-Plane Filt  │   ParameterQueue      │ • MIDI Input   │   DeviceEventQueue   │ • Compute Shader│
│ • Tube Satur    │◄─────────────────────►│ • UI Updates   │◄─────────────────────►│ • Particle Sys  │
│ • <5ms Latency  │                       │ • 60fps Timer  │                      │ • Texture Stream│
│ • ZERO Allocs   │                       │ • Preset Mgmt  │                      │ • Frame Dropping│
└─────────────────┘                       └────────────────┘                      └─────────────────┘
```

## Core Components Architecture

### 1. Audio Processing Chain
```cpp
AudioInput → WindowedSTFT → SpectralManipulation → iFFT → ZPlaneFilter → TubeSaturation → AudioOutput
              ↓                      ↑                                      ↓
          SpectralData        UI Gestures/Parameters              FilterCurveData
              ↓                      ↑                                      ↓
      Lock-Free Queue         Lock-Free Queue                    Lock-Free Queue
```

### 2. GPU Rendering Pipeline
```cpp
SpectralTexture → ComputeShader(Colormap) → ParticleUpdate → GeometryShader → FragmentShader → SwapChain
      ↑                                           ↑                                  ↓
SpectralDataQueue                        GestureInput                      60fps Display
```

## File Structure & Responsibilities

### Core Engine (`/Source/Core/`)
- **MessageBus.h** - Lock-free SPSC queue templates and communication protocols
- **RealtimeSafeTypes.h** - RT-safe containers, assertions, and memory management  
- **Params.h** - Parameter definitions, ranges, and automation mapping

### DSP Processing (`/Source/DSP/`)
- **SpectralEngine.{h,cpp}** - STFT/iFFT with windowing, RT-safe spectral buffers
- **ZPlaneFilter.{h,cpp}** - EMU-style morphing filters (LP↔BP↔HP)
- **TubeSaturation.{h,cpp}** - Analog warmth modeling with 2x oversampling

### GPU Rendering (`/Source/Viz/`)
- **GpuRenderer.h** - Abstract renderer interface for cross-platform support
- **D3D11Renderer.{h,cpp}** - Windows-optimized Direct3D 11 implementation
- **OpenGLRenderer.{h,cpp}** - Cross-platform OpenGL fallback
- **ParticleSystem.{h,cpp}** - GPU-instanced particle management
- **Shaders/** - HLSL compute/vertex/fragment shaders

### Plugin Framework (`/Source/`)
- **PluginProcessor.{h,cpp}** - JUCE audio processor, parameter management
- **PluginEditor.{h,cpp}** - Main UI, gesture handling, D3D11 integration

## Data Flow Specifications

### SpectralDataQueue (Audio → UI)
```cpp
struct SpectralFrame {
    alignas(64) float magnitude[1024];    // FFT bin magnitudes
    alignas(64) float phase[1024];        // FFT bin phases  
    uint32_t sequence_number;             // Atomic ordering
    double timestamp_samples;             // Sample-accurate timing
};
```

### RenderCommandQueue (UI → GPU)
```cpp
struct RenderCommand {
    enum Type { UpdateTexture, DrawGesture, SetColormap, ClearParticles };
    Type command_type;
    union {
        SpectralFrame spectral_data;
        GestureData gesture_info;
        ColormapSettings colormap_params;
    };
};
```

### ParameterQueue (UI → Audio)
```cpp
struct ParameterUpdate {
    uint32_t parameter_id;               // Parameter identifier
    float normalized_value;              // 0.0 to 1.0 range
    uint32_t sample_offset;              // Buffer-relative timing
};
```

## Performance Specifications

### Audio Thread Requirements (Non-Negotiable)
- **Latency**: <5ms total processing (STFT + filters + saturation)
- **Memory**: Zero allocations during processBlock()  
- **CPU**: <15% single core usage at 48kHz/64 samples (tightened target)
- **Stability**: Zero audio dropouts under normal operating conditions
- **Paint→Audio**: <5ms (p50), <10ms (p99) from brush stroke to audio output

### GPU Thread Requirements
- **Framerate**: 60fps sustained with 1000+ particles  
- **Frame Budget**: <16ms per frame @ 1080p
- **Resolution**: Up to 4K display support (3840x2160)
- **Memory**: <64MB VRAM for all textures and buffers
- **Compatibility**: DirectX 11.0+ with WARP fallback
- **Device Recovery**: <100ms recovery time after device-lost events

### UI Thread Requirements  
- **Responsiveness**: <16ms gesture-to-visual feedback
- **Precision**: Sub-pixel gesture accuracy for fine control
- **Stability**: Graceful handling of MIDI controller floods

## GPU Resilience Architecture (Phase 4)

### Device-Lost Recovery State Machine
```
States: OK → REMOVED → RECREATING → OK (success path)
                    ↓
                WARP_FALLBACK (after 3 failed hardware attempts)
```

### Detection Points
- After Present() calls (DXGI_ERROR_DEVICE_REMOVED)
- Before texture updates (GetDeviceRemovedReason)
- In beginFrame() for early detection

### Recovery Choreography
1. **Detection**: GetDeviceRemovedReason() != S_OK
2. **Resource Release**: Clean up all device-dependent resources
3. **Recreation**: Try hardware device first, fallback to WARP after 3 failures  
4. **Resource Rebuild**: Recreate swapchain, textures, shaders, buffers
5. **Resume**: Continue rendering with <100ms recovery time

### Staging Buffer Pattern (RT-Safe)
```cpp
// RT-safe texture updates - no mapping of default GPU resources
CPU → Staging Texture (Map/Unmap)
Staging → GPU Texture (CopySubresourceRegion)
```

### Telemetry (Atomic Counters)
- `device_state`: Current device state (OK/REMOVED/RECREATING/WARP_FALLBACK)
- `frame_time_us`: Rolling average frame time
- `recovery_count`: Number of successful recoveries
- `dropped_frame_count`: Frames lost during device issues
- `peak_frame_time_us`: Peak frame time in current window

## Observability & Performance Monitoring

### Performance HUD (Phase 5)
Real-time metrics overlay providing zero-impact monitoring of critical performance indicators:

```
┌─ Performance HUD Layout ──────────┐
│ Lat: 3.2/8.1ms    CPU: 8.5%      │  ← Paint→audio latency p50/p99, CPU usage
│ GPU: 14.2ms HW     Rec: 0ms       │  ← GPU frame p95, device mode, recovery time
│ Q: 7/8  D: 0  XR: 0               │  ← Queue depth/cap, drops, xruns  
│ 48kHz/512          [H]ide         │  ← Audio config, toggle hint
└────────────────────────────────────┘
```

**Implementation Strategy:**
- UI thread sampling at 30Hz (no RT impact)
- Atomic reads from existing telemetry counters
- Toggle via parameter automation or 'H' key
- Render budget: ≤1ms per frame
- Rollback: DISABLE_PERF_HUD compile flag

### Metrics Data Flow
```
┌─────────────────┐    Atomic Reads   ┌──────────────┐    JSON Export   ┌──────────────┐
│   Audio Thread  │─ ─ ─ ─ ─ ─ ─ ─ ─ ►│   PerfHUD    │─ ─ ─ ─ ─ ─ ─ ─ ►│   CI System   │
│                 │   (RTLatencyTracker)│              │  (30Hz sampling) │              │
│ • processBlock  │                    │ • UI overlay │                  │ • Regression  │
│ • Queue depths  │                    │ • 180x100px  │                  │   Detection   │
│ • Xrun counters │                    │ • Parameter  │                  │ • Thresholds  │
│                 │                    │   controlled │                  │   Alerting    │
└─────────────────┘                    └──────────────┘                  └──────────────┘
        │                                       ↑
        │            Atomic Reads               │
        ▼                                       │
┌─────────────────┐                            │
│   GPU Thread    │ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┘
│                 │   (GPUStatus struct)
│ • Frame timing  │
│ • Device state  │
│ • Recovery cnt  │
│ • WARP mode     │
└─────────────────┘
```

### CI Integration
- PerfHarness exports JSON with `--emit-json` flag
- Soft gates: WARN if latency_p99 > 15ms or CPU > 20%
- Future hard gates after baseline stabilization
- Artifact upload: `spectralcanvas-perf-${BUILD}.json`

## Risk Analysis & Mitigation Strategies

### 🔴 Critical Risks

**1. Real-Time Safety Violations**
- *Risk*: Accidental memory allocation in audio thread causing dropouts
- *Detection*: Custom allocator hooks + ThreadSanitizer in debug builds
- *Mitigation*: Comprehensive code review checklist + automated testing

**2. GPU Driver Compatibility Issues**
- *Risk*: Inconsistent D3D11 feature support across hardware vendors + device removal
- *Detection*: Feature capability queries + device-lost monitoring
- *Mitigation*: WARP fallback + device recovery choreography + staging buffers

**3. DAW Integration Problems**
- *Risk*: Plugin state/threading model conflicts with host DAWs
- *Detection*: Extensive testing in REAPER, Cubase, FL Studio
- *Mitigation*: Early DAW validation + gradual feature rollout

### 🟡 Medium Risks

**4. Complex State Synchronization**
- *Risk*: Audio/UI/GPU thread coordination bugs causing inconsistent state
- *Mitigation*: Simple message protocols + extensive integration testing

**5. Performance Regression Under Load**
- *Risk*: Missing <5ms latency or 60fps targets with complex content
- *Mitigation*: Continuous performance monitoring + Fast Paint Mode fallback

## Phase 1 Implementation Priority

### Week 1: Foundation
1. Update CMakeLists.txt to JUCE 8.0.8
2. Create basic PluginProcessor/Editor skeletons
3. Implement core MessageBus system with unit tests
4. Basic D3D11 context creation + swap chain

### Week 2: Spectral Pipeline  
1. RT-safe STFT engine with windowing
2. Fake spectral data generation for testing
3. GPU texture upload pipeline
4. Basic fullscreen quad rendering

### Week 3: Integration & Testing
1. Complete Audio→UI→GPU data flow
2. Mouse gesture capture + visual feedback
3. Performance profiling + RT-safety validation
4. REAPER plugin loading + basic functionality test

## Success Criteria
- Live scrolling spectrogram displaying at 60fps
- Mouse gestures affect visual content in real-time  
- <5ms measured latency from input to audio output
- Zero RT-safety violations detected in debug builds
- Stable plugin loading in REAPER without crashes
