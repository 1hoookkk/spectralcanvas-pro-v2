# SpectralCanvasLite Development Roadmap

## Vision
SpectralCanvasLite serves as the clean, minimal foundation for building a production-ready spectral audio processor. We strategically mine the ResearchVault (4000+ files of high-value IP) by porting components incrementally while maintaining RT-safety, clean architecture, and preventing technical debt accumulation.

## Core Philosophy
- **Start minimal, add complexity behind flags**
- **Explicit is better than implicit** (no GLOB patterns)  
- **Every file must have a purpose and owner**
- **Archive don't delete** (ResearchVault stays external)
- **Test early, test often**

---

## Validation Gates (Required for All Phases)

These gates must pass for every PR and release:

### 🏗️ Build Quality
- ✅ Builds cleanly with zero warnings (`-Werror -Wall -Wextra`)
- ✅ No orphan files (`tools/check_orphans.py` passes)
- ✅ Feature flags work (builds with all OFF and all ON configs)

### ⚡ Real-Time Safety
- ✅ No allocations/locks in `processBlock()` (audio thread)
- ✅ Bounded work per block (≤16 queue drains maximum)
- ✅ Hard clamp output to ±1.0f at end of each block

### 🎯 Plugin Validation
- ✅ **pluginval strictness ≥8 passes** (level 10 preferred)
- ✅ DAW smoke tests (load, process, paint events)
- ✅ Latency reporting: Centered STFT (FFT_SIZE/2)

### 🔬 Code Quality
- ✅ Explicit CMake source lists (no wildcards)
- ✅ Per-module documentation (README per directory)
- ✅ RT-safety audit (no heap operations detected)

---

## Development Phases

### Phase 1: Clean Architecture Foundation
**Status**: In Progress  
**Goal**: Establish organized structure and ghost-code prevention

#### Infrastructure
- [x] Create task scaffolding (`sessions/tasks/h-implement-spectral-canvas-foundation/`)
- [ ] Organize `Source/` into logical subdirectories:
  - `Source/Core/` - Plugin infrastructure (processor, parameters)
  - `Source/DSP/` - Audio processing (STFT, engines, RT-safe code)
  - `Source/UI/` - Interface components (canvas, editor, HUD)  
  - `Source/Utils/` - Lock-free queues, utilities, helpers
- [ ] Add ghost-code prevention tools:
  - `tools/check_orphans.py` - Detect unlisted source files
  - `.githooks/pre-commit` - Run checks before commits
  - Per-directory `README.md` with ownership/purpose

#### CMake Modernization  
- [ ] Fix naming inconsistency (SpectralCanvasLite vs SpectralCanvasPro)
- [ ] Convert to explicit source lists (eliminate GLOB patterns)
- [ ] Add feature flags (default OFF):
  - `SC_ENABLE_STUB` - Enable SpectralEngineStub path
  - `SC_ENABLE_PSYCHO` - Enable PsychoacousticMaskingEngine
- [ ] Add strict compiler warnings

**Deliverables**: Clean directory structure, build system guardrails

### Phase 2: RT-Safe Event Transport
**Status**: Pending  
**Goal**: Wire paint events to audio thread without DSP changes

#### Core Components
- [ ] `Utils/SpscQueueLite.h` - Single-producer, single-consumer lock-free queue
- [ ] `Core/MaskColumn.h` - Paint event structure (column, intensity, timestamp)
- [ ] `Core/Snapshot.h` - RT-safe telemetry counters

#### Audio Thread Integration
- [ ] Add `SpscQueueLite<MaskColumn, 1024> maskQueue_` to processor
- [ ] Bounded drain ≤16 events per `processBlock()` call
- [ ] Count queue rejections (full queue handling)
- [ ] Publish per-block counters: `processedBlocks`, `queueDepth`, `drainsThisBlock`

#### UI Integration  
- [ ] Minimal HUD overlay showing live counters
- [ ] Wire paint events from editor to processor queue
- [ ] No allocations on audio thread (atomics + lock-free queues only)

**Deliverables**: Working paint→queue→counters pipeline, silent audio passthrough

### Phase 3: Minimal Spectral Processing
**Status**: Pending  
**Goal**: Add audible spectral masking behind feature flag

#### Spectral Engine Stub
- [ ] `DSP/SpectralEngineStub.h/.cpp` - Minimal STFT processor behind `SC_ENABLE_STUB`
- [ ] Basic FFT→apply masks→iFFT pipeline
- [ ] Use existing `PsychoacousticMaskingEngine` (already ported!)
- [ ] Latency compensation: `setLatencySamples(FFT_SIZE/2)`

#### Audio Path Routing
- [ ] Route `processEnabled=true` to SpectralEngineStub when flag enabled
- [ ] Apply mask-derived gain shaping (simple RT-safe multiplication)
- [ ] Maintain backward compatibility (flag OFF = current ducking behavior)

**Deliverables**: Audible paint→spectral masking, configurable via flag

### Phase 4: Advanced Features (Competitive Differentiators)
**Status**: Future  
**Goal**: Port high-value IP from ResearchVault behind flags

#### Priority Components (Mine from ResearchVault)
- [ ] **AdvancedPsychoacousticEngine** - AI-powered masking with neural enhancement
- [ ] **SpectralSynthEngine** - Musical additive synthesis with harmonic quantization
- [ ] **Z-Plane Spectral Masking** - Advanced frequency domain processing
- [ ] **Voice Pooling System** - Polyphonic synthesis management

#### UI Enhancements
- [ ] **CanvasComponent** - Real-time spectral painting with gesture tracking
- [ ] **SpectrogramComponent** - GPU-accelerated visualization
- [ ] **Performance HUD** - Live telemetry and RT-safety monitoring

**Deliverables**: Feature-complete spectral processor with unique innovations

### Phase 5: Production Polish & Validation  
**Status**: Future  
**Goal**: Shipping-ready plugin with full validation

#### Testing & Validation
- [ ] **pluginval level 10** passes consistently
- [ ] **DAW compatibility** testing (Pro Tools, Logic, Ableton, etc.)
- [ ] **RT-safety audit** - Verify zero heap operations in audio thread
- [ ] **Performance profiling** - CPU usage <15% @ 48kHz/64 samples

#### Documentation & Distribution
- [ ] Complete user documentation
- [ ] Developer API documentation  
- [ ] Installation/deployment guides
- [ ] Performance benchmarks

**Deliverables**: Shipping-ready SpectralCanvasLite v1.0

---

## Mining Playbook: ResearchVault → SpectralCanvasLite

When porting components from the ResearchVault:

1. **Identify minimal class** - Find the core functionality without dependencies
2. **Port into isolated namespace/module** - Keep it contained and testable
3. **Add compile flag** - Default OFF, enable incrementally  
4. **Unit smoke + pluginval** - Validate before enabling by default
5. **Document integration** - Update module README with usage patterns

### Available ResearchVault Treasures
Based on agent analysis of 237+ source files:

#### Essential (Phase 3-4)
- `STFTProcessor.h` - Complete STFT with psychoacoustic integration
- `SpectralEngine.h` - Production-ready spectral processing
- `SpscQueueLite.h` - Lock-free RT-safe communication

#### High-Value IP (Phase 4)  
- `AdvancedPsychoacousticEngine.h` - The "secret sauce" with AI masking
- `SpectralSynthEngine.h` - Musical synthesis with voice pooling
- `CanvasComponent.h` - Complete spectral painting interface

#### Innovative Features (Phase 4-5)
- Z-Plane spectral masking algorithms
- GPU acceleration framework (D3D11 + fallbacks)
- AI Creative Assistant with pattern recognition
- Neural enhancement and scene analysis

---

## Success Metrics

### Technical KPIs
- **Build time**: <3 minutes clean build
- **Plugin load time**: <500ms in DAW
- **Audio latency**: <5ms paint→audio feedback  
- **CPU usage**: <15% @ 48kHz/64 samples
- **Memory**: Zero allocations in `processBlock()`

### Quality KPIs
- **pluginval**: Level 10 passes (no exceptions)
- **DAW compatibility**: 95%+ success rate across major DAWs
- **RT-safety**: Zero heap operations detected in audio thread
- **Code coverage**: >80% for RT-critical paths

### Development KPIs
- **Ghost files**: Zero orphaned source files
- **Build warnings**: Zero warnings with strict flags
- **Feature flags**: All combinations build successfully
- **Documentation**: Every module has README with API examples

---

## Current Status: Phase 1 In Progress

✅ **Completed**
- Task structure created
- Roadmap documented
- ResearchVault cataloged (237+ source files)
- PsychoacousticMaskingEngine already ported

🚧 **In Progress**  
- Source directory organization
- CMake modernization with explicit lists
- Ghost-code prevention tools

📋 **Next Steps**
- Complete Phase 1 infrastructure
- Implement SPSC queue + MaskColumn structures  
- Wire paint events to bounded audio thread processing
- Begin Phase 2: RT-safe event transport