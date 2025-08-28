# SpectralCanvas Pro v2 - Development Rules & Standards

## C++20 Standards
- Use RAII for all resource management
- Apply `noexcept` to functions that guarantee no exceptions
- Prefer `auto` for type deduction, explicit types for clarity
- Use smart pointers over raw pointers
- Prefer stack allocation over heap when possible
- Standard library containers over custom implementations

## Real-Time Safety Rules (CRITICAL)
### Audio Thread - ZERO Tolerance
- **NO memory allocations** (new, malloc, vector resize, etc.)
- **NO mutexes, locks, or blocking operations**
- **NO GPU API calls** (D3D11, OpenGL contexts forbidden)
- **NO file I/O, network I/O, or system calls**
- **NO exceptions** (all audio code must be noexcept)
- **NO std::shared_ptr creation** (copying existing ones is OK)
- Use only: atomic operations, lock-free containers, pre-allocated buffers

### RT-Safe Data Structures Only
- `juce::AudioBuffer<float>` - pre-allocated
- `std::array<T, N>` - fixed size
- `std::atomic<T>` - lock-free primitives
- Custom lock-free SPSC queues - single producer/consumer only
- Pre-allocated circular buffers with atomic indices

## GPU Thread Rules
- **ALL D3D11/OpenGL calls must happen on GPU thread**
- Frame dropping is acceptable for performance
- GPU thread may allocate for textures/buffers (not RT)
- Handle device lost scenarios gracefully
- Validate all GPU operations, never assume success

## Message Bus Protocol
### Lock-Free SPSC Queues Only
```cpp
// Audio Thread → UI Thread (spectral data)
SpectralDataQueue audio_to_ui_queue;

// UI Thread → GPU Thread (render commands) 
RenderCommandQueue ui_to_gpu_queue;

// UI Thread → Audio Thread (parameters)
ParameterQueue ui_to_audio_queue;
```

### Data Structure Standards
- POD types only in queues (no destructors)
- Fixed-size messages (no dynamic allocation)
- Atomic sequence numbers for message ordering
- Timeout handling for queue full scenarios

## Shader Conventions
### HLSL (Primary - Windows)
- Use semantic names: POSITION, TEXCOORD0, SV_Target
- Constant buffers for uniform data
- Compute shaders for spectral processing
- Resource binding slots documented in comments

### GLSL (Fallback - Cross-platform)
- Matching functionality to HLSL versions
- Explicit location qualifiers
- Same uniform buffer layouts
- Compatible texture formats

## Build Standards
### MSVC 2022 Configuration
- C++20 standard (/std:c++20)
- Warning level 4 (/W4)
- Permissive mode disabled (/permissive-)
- Runtime checks in Debug (/RTC1)
- Optimization in Release (/O2)

### Target Validation Order
1. **REAPER** - Primary DAW target (most stable plugin host)
2. **Cubase** - Secondary validation
3. **FL Studio** - Tertiary validation
4. Other DAWs - Optional testing

## Commit Format Standards
### Title Format (≤6 words)
```
add stft engine foundation
fix d3d11 swap chain leak  
optimize particle instancing
```

### Performance Impact Summary
```
🚀 Performance: +15% render speed
⚠️  Memory: +2MB texture cache
🔧 Maintenance: refactor reduces complexity
🐛 Bugfix: eliminates audio dropouts
```

## Thread Architecture Enforcement
### Audio Thread Functions
```cpp
void processBlock(AudioBuffer<float>& buffer) noexcept;
void updateSpectralData() noexcept;
void applyZPlaneFilter() noexcept;
```

### UI Thread Functions  
```cpp
void handleMouseGesture(Point<float> position);
void updateParameterValues();
void queueRenderCommands();
```

### GPU Thread Functions
```cpp
void initializeD3D11Context();
void renderSpectralFrame();
void handleDeviceLost();
```

## Error Handling Strategy
- Audio thread: `assert()` for debug, graceful degradation in release
- UI thread: Exception handling with user feedback
- GPU thread: Device validation with fallback paths
- All threads: Comprehensive logging with thread identification

## Memory Management
- Audio buffers: Pre-allocated in constructor, never resize
- GPU resources: RAII wrappers with automatic cleanup
- UI elements: Standard JUCE component lifecycle
- Inter-thread data: POD structs in lock-free queues only

## Performance Targets (Non-Negotiable)
- **Audio Latency**: <5ms gesture-to-sound response
- **Visual Framerate**: 60fps sustained with 1000+ particles
- **Memory Usage**: <100MB total plugin footprint
- **CPU Usage**: <15% on modern quad-core at 48kHz

## Code Review Checklist
- [ ] Audio thread code is 100% RT-safe
- [ ] No blocking operations in performance paths  
- [ ] GPU resources properly managed (no leaks)
- [ ] Message queues handle overflow gracefully
- [ ] All allocations happen at initialization
- [ ] Performance targets validated with profiler