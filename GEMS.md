# 💎 GEMS.md - Working Components Worth Preserving

> High-value, battle-tested code that should be preserved and reused in future iterations.

## 🔐 RT-Safe Data Structures

### SPSCQueue - Lock-Free Single Producer/Consumer Queue
**Location:** `Source/Core/MessageBus.h` (lines 9-83)
**Why It's Gold:** 
- Dmitry Vyukov's proven bounded SPSC design
- Cache-line aligned for optimal performance
- Zero allocations after initialization
- Memory ordering carefully tuned for minimal overhead

```cpp
template<typename T, size_t Size>
class SPSCQueue
{
    alignas(64) std::array<T, Size> buffer_;
    alignas(64) std::atomic<size_t> head_;
    alignas(64) std::atomic<size_t> tail_;
};
```

### RTCircularBuffer - Real-Time Safe Circular Buffer
**Location:** `Source/Core/RealtimeSafeTypes.h` (lines 37-112)
**Why It's Gold:**
- Atomic indices for thread safety
- Power-of-2 size for fast modulo operations
- Block read/write support for efficiency
- Clear separation between producer/consumer APIs

### RTWindow - Pre-computed Window Functions
**Location:** `Source/Core/RealtimeSafeTypes.h` (lines 115-184)
**Why It's Gold:**
- Windows computed at initialization, not runtime
- Multiple window types (Hann, Hamming, Blackman)
- Template-based for compile-time optimization
- Zero allocations during audio processing

## 📊 Spectral Processing Components

### SpectralFrame Communication Structure
**Location:** `Source/Core/MessageBus.h` (lines 86-108)
**Why It's Gold:**
- Perfectly sized for 512-point FFT
- Aligned for SIMD operations
- Includes essential metadata (timestamp, sequence number)
- Spectral features (centroid, fundamental) pre-computed

```cpp
struct SpectralFrame
{
    alignas(32) float magnitude[NUM_BINS];
    alignas(32) float phase[NUM_BINS];
    uint32_t sequenceNumber;
    double timestampSamples;
    float fundamentalFreq;
    float spectralCentroid;
};
```

### SpectralEngine Architecture
**Location:** `Source/DSP/SpectralEngine.h` & `.cpp`
**Why It's Gold:**
- Clean STFT/iFFT implementation
- Overlap-add properly implemented
- RT-safe with pre-allocated buffers
- JUCE FFT wrapper for portability

## 🎨 GPU Rendering Pipeline

### Shader Colormap Implementations
**Location:** `Source/Viz/shaders/SpectralVisualization.hlsl`
**Why It's Gold:**
- **Viridis** (lines 29-43): Perceptually uniform, GPU-optimized polynomial
- **Magma** (lines 46-60): High contrast for particle effects
- **SpectralColormap** (lines 63-82): Custom frequency-to-hue mapping

### Particle System Shader
**Location:** `Source/Viz/shaders/ParticleSystem.hlsl`
**Why It's Gold:**
- GPU instancing for 1000+ particles
- Spectral-driven animation (lines 106-120)
- Complexity-based shape morphing (lines 239-263)
- Billboard generation in geometry shader

## 🔧 Parameter Management

### Parameter Layout Factory
**Location:** `Source/Core/Params.h` (lines 74-129)
**Why It's Gold:**
- Clean separation of IDs, ranges, and defaults
- Type-safe parameter creation
- Works seamlessly with JUCE ValueTreeState
- Sample-accurate automation ready

## 📐 Thread Architecture Pattern

### Three-Thread Message Bus Design
**Location:** `Source/Core/MessageBus.h`
**Why It's Gold:**
```
Audio Thread ←→ UI Thread ←→ GPU Thread
     ↑              ↑            ↑
SpectralDataQueue  │   RenderCommandQueue
     └─ParameterQueue
```
- Clear separation of concerns
- Lock-free communication throughout
- Each thread has specific, non-overlapping responsibilities

## 🛡️ Safety Utilities

### RT-Safe Assertions
**Location:** `Source/Core/RealtimeSafeTypes.h` (lines 12-19)
**Why It's Gold:**
- Debug-only checks that compile out in release
- Clear naming convention (RT_SAFE_ASSERT)
- Logging that identifies RT violations

### AlignedStorage Template
**Location:** `Source/Core/RealtimeSafeTypes.h` (lines 22-34)
**Why It's Gold:**
- Ensures cache-line alignment automatically
- Zero-overhead abstraction
- Works with any trivially-copyable type

## 🚀 Performance Patterns

### Gesture Command Union Pattern
**Location:** `Source/Core/MessageBus.h` (lines 124-158)
**Why It's Gold:**
- Efficient command dispatch without virtual functions
- Fixed-size messages for queue compatibility
- Type safety with enum discrimination

### Sequence Number Generation
**Location:** `Source/Core/RealtimeSafeTypes.h` (lines 217-240)
**Why It's Gold:**
- Lock-free atomic counter
- Monotonic ordering guarantee
- Reset capability for testing

## 🏗️ Build Configuration

### MSVC Compiler Settings
**Location:** `CMakeLists.txt` (lines 80-88)
**Why It's Gold:**
- C++20 standard enforcement
- Strict warning levels (/W4)
- Runtime checks in debug (/RTC1)
- Optimal release settings (/O2)

### JUCE Configuration
**Location:** `CMakeLists.txt` (lines 14-23)
**Why It's Gold:**
- Clean plugin definition
- Minimal dependencies
- VST3 + Standalone formats
- No unnecessary features enabled

## 📝 Documentation Standards

### Development Rules Document
**Location:** `CLAUDE.md`
**Why It's Gold:**
- Comprehensive RT-safety rules
- Clear thread responsibility matrix
- Performance targets clearly stated
- Code review checklist

### Technical Architecture Blueprint
**Location:** `docs/BLUEPRINT.md`
**Why It's Gold:**
- Visual thread architecture diagram
- Data flow specifications
- Risk analysis with mitigation strategies
- Phased implementation plan

## 🎯 Key Takeaways

These components represent **battle-tested patterns** that solve real problems:
1. **Lock-free communication** that actually works
2. **RT-safe structures** with zero allocation guarantees  
3. **GPU shaders** optimized for visual quality and performance
4. **Clear architectural boundaries** preventing thread conflicts
5. **Documentation** that explains the "why" not just the "what"

**Preserve these gems** - they're the foundation of a robust real-time audio/visual system.
