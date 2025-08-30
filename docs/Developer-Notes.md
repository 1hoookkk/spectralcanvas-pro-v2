# SpectralCanvas Pro Developer Quick Reference

## RT-Safety Checklist ⚡

### In `processBlock()` and all RT paths:
- ❌ **NO** `new`, `delete`, `malloc`, `free`
- ❌ **NO** `std::vector<>.push_back()`, `.resize()`, `.reserve()`
- ❌ **NO** `juce::Logger::writeToLog()`, `printf`, `std::cout`
- ❌ **NO** `std::mutex`, `juce::CriticalSection`, `std::lock_guard`
- ❌ **NO** File I/O, network calls, or blocking system calls
- ❌ **NO** Exception throwing (`throw`, dynamic allocations)

### RT-Safe Alternatives:
- ✅ Pre-allocated `std::array<T, N>`, fixed-size buffers
- ✅ Lock-free atomics: `std::atomic<T>` with `memory_order_relaxed`
- ✅ SPSC queues: `Core/MessageBus.h` templates
- ✅ Ring buffers for debugging counters (no immediate logging)
- ✅ Stack-allocated variables, pass by reference/value
- ✅ `constexpr` functions and compile-time constants

### RT Validation Commands:
```bash
# Build with RT safety checks
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_RT_SAFETY=ON
cmake --build build

# Run allocation/lock detector
build/Tests/RTSafetyTest --detect-allocs --detect-locks

# Profile processBlock latency
build/Tests/ProcessBlockTest --samples 48000 --buffer-size 64
```

## GPU Safety Checklist 🎨

### Device-Lost Recovery:
- ✅ Always check `HRESULT` from D3D11 calls
- ✅ Handle `DXGI_ERROR_DEVICE_REMOVED`, `DXGI_ERROR_DEVICE_RESET`
- ✅ Fallback to WARP renderer: `D3D_DRIVER_TYPE_WARP`
- ✅ Graceful degradation: disable particles, keep audio working
- ✅ Recovery time: <100ms to restore rendering

### Frame Budget Management:
- ✅ Target: <16ms per frame @ 1080p (60fps)
- ✅ Use staging buffers for large texture uploads
- ✅ Batch similar draw calls, minimize state changes
- ✅ Drop frames rather than block the render thread
- ✅ Profile with GPU timers: `ID3D11Query::D3D11_QUERY_TIMESTAMP`

### GPU Validation Commands:
```bash
# Test device-lost recovery
build/Tests/GPUResilienceTest --simulate-device-lost

# Profile GPU frame times  
build/Tests/GPUProfilerTest --resolution 1920x1080 --duration 30s

# Validate WARP fallback
build/Tests/WarpFallbackTest --force-software-rendering
```

## Performance Targets 🎯

### Audio Thread:
- **Latency**: <5ms (p50), <10ms (p99) paint→audio
- **CPU Usage**: <15% @ 48kHz/64 samples
- **Memory**: 0 allocations in `processBlock()`
- **Dropouts**: 0 under normal load

### GPU Thread:
- **Frame Rate**: 60fps sustained
- **Frame Budget**: <16ms @ 1080p
- **VRAM Usage**: <64MB total
- **Device Recovery**: <100ms

### Queue Health:
- **Drops**: 0 under normal load
- **Latency**: <2ms queue traversal time
- **Capacity**: 95% max utilization before backpressure

## Common Patterns 📋

### RT-Safe Parameter Updates:
```cpp
// Message thread writes
oscGain_.store(newValue, std::memory_order_relaxed);

// Audio thread reads  
float gain = oscGain_.load(std::memory_order_relaxed);
```

### Lock-Free Queue Usage:
```cpp
// Producer (UI thread)
if (maskColumnQueue.canPush()) {
    maskColumnQueue.push(maskData);
} else {
    queueDrops_.fetch_add(1, std::memory_order_relaxed);
}

// Consumer (Audio thread)  
MaskColumn column;
while (maskColumnQueue.pop(column)) {
    // Process column - guaranteed RT-safe
    spectralEngine.applyMask(column);
}
```

### GPU Device-Lost Recovery:
```cpp
HRESULT hr = device->Present(0, 0);
if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
    // Release all D3D resources
    cleanupD3DResources();
    
    // Recreate with WARP fallback
    if (!recreateDevice(D3D_DRIVER_TYPE_WARP)) {
        // Disable rendering, continue audio-only
        gpuEnabled_.store(false, std::memory_order_relaxed);
    }
}
```

### Static Safety Assertions:
```cpp
// In header files
static_assert(std::is_trivially_copyable_v<MaskColumn>);
static_assert(sizeof(SpectralFrame) % 64 == 0, "Cache line alignment");
static_assert(std::is_lock_free<std::atomic<float>>::value);

// RT path markers
void processBlock(...) noexcept {
    RT_SAFE_ASSERT(std::this_thread::get_id() == audioThreadId_);
    // ... RT-safe code only
}
```

## Debug Helpers 🔍

### Allocation Detection:
```cpp
#ifdef DEBUG
    // Custom allocator that fails in RT context
    class RTSafeAllocator {
        static bool isAudioThread() { /* check thread ID */ }
    public:
        void* allocate(size_t) {
            RT_SAFE_ASSERT(!isAudioThread() && "Allocation in RT path!");
            return ::malloc(size);
        }
    };
#endif
```

### Performance Counters:
```cpp
// RT-safe counters (no immediate logging)
std::atomic<uint64_t> processBlockCalls_{0};
std::atomic<uint64_t> queueDrops_{0};
std::atomic<float> avgLatencyMs_{0.0f};

// UI thread reads for HUD display
void updatePerfHUD() {
    uint64_t calls = processBlockCalls_.load();
    uint64_t drops = queueDrops_.load();
    float latency = avgLatencyMs_.load();
    
    // Update HUD display (non-RT thread)
    perfHUD_->setValues(calls, drops, latency);
}
```

## File Boundaries 🚧

### ✅ Safe to Edit:
- `Source/Core/` - Message bus, RT-safe types, parameters
- `Source/DSP/` - Spectral engine, audio processing
- `Source/GUI/` - UI components, canvas painting
- `Source/Viz/` - GPU rendering, shaders

### ❌ Never Touch:
- `build*/` - Generated build files  
- `*_deps/` - External dependencies
- `JUCE modules` - Framework internals

### ⚠️ Ask Before Modifying:
- `CMakeLists.txt` - Build configuration
- `pluginval.cmake` - Validation setup
- `.github/workflows/` - CI/CD pipeline

## Testing Workflow 🧪

```bash
# Full validation pipeline
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build

# RT safety validation
ctest --test-dir build -R RT

# GPU resilience testing  
ctest --test-dir build -R GPU

# Plugin validation (strictness level 8)
cmake --build build --target pluginval_rt_safety

# Performance regression check
build/Tests/PerfHarness --baseline perf_baseline.json

# Export performance metrics to JSON
build/Tests/PerfHarness --baseline --emit-json perf_metrics.json
```

## Performance HUD 📊

### Overview
The Performance HUD provides real-time monitoring of critical audio/visual metrics without impacting RT performance. It samples atomic counters at 30Hz from the UI thread only.

### Displayed Metrics:
```
┌─────────────────────────┐
│ Lat:3.2/8.1ms CPU:8.5% │  (p50/p99 latency, CPU usage)
│ GPU:14.2ms HW Rec:0ms   │  (GPU frame time, device mode, last recovery)
│ Q:7/8 D:0 XR:0          │  (queue depth/cap, drops, xruns)
│ 48kHz/512 [H]ide        │  (sample rate/block, toggle hint)
└─────────────────────────┘
```

### Usage:
```bash
# Toggle via parameter (automatable)
showPerfHud = 1.0  # Show HUD
showPerfHud = 0.0  # Hide HUD

# Keyboard shortcut
Press 'H' key to toggle visibility

# Access from code
auto metrics = perfHUD.getLastMetrics();
float latencyP50 = metrics.latencyP50Ms;
```

### Implementation Details:
- **Thread Safety**: UI thread sampling only, atomic loads with memory_order_acquire
- **Update Rate**: 30Hz timer (33ms intervals) to avoid UI overhead
- **Render Budget**: ≤1ms per frame, typically <0.5ms
- **RT Impact**: Zero - no audio thread polling or locks
- **Rollback**: Compile flag DISABLE_PERF_HUD=ON to disable completely

### Metrics Sources:
- **Latency**: RTLatencyTracker (paint→audio latency in µs)
- **CPU**: ContinuousVerification (block processing time estimation)
- **GPU**: GPUStatus atomic struct (frame time, device state, recovery)
- **Queues**: SPSC queue atomic counters (depth, drops)
- **XRuns**: Host-provided underrun detection when available

## Emergency Procedures 🚨

### Audio Dropouts:
1. Check `processBlock()` for new allocations
2. Verify queue capacities aren't exceeded  
3. Profile CPU usage with audio-thread tools
4. Disable non-essential features temporarily

### GPU Crashes:
1. Force WARP fallback: `D3D_DRIVER_TYPE_WARP`
2. Disable particle system: `particlesEnabled_ = false`
3. Check for resource leaks with debug layer
4. Validate shader compilation on target hardware

### Build Failures:
1. Clean build: `rm -rf build && cmake -B build`
2. Check JUCE version: should be 8.0.8
3. Verify C++20 support: MSVC 2019+ or GCC 10+
4. Update dependencies: `git submodule update --recursive`