# RT-Safe Memory Patterns for DSP

## Core Concepts

- **Zero-Allocation Principle**: No heap allocations (new, malloc, vector::resize) allowed in processBlock() or any audio callback
- **Preallocation Strategy**: All audio buffers, FFT workspaces, and processing objects allocated during prepareToPlay()
- **Lock-Free Communication**: UI↔Audio thread communication via atomic variables and SPSC (Single Producer Single Consumer) queues
- **Stack Allocation Preference**: Use stack-allocated arrays and juce::AudioBuffer for temporary processing
- **Object Pooling**: Reuse expensive objects (FFT instances, filter states) rather than recreating
- **Deterministic Memory Access**: Predictable memory access patterns prevent cache misses and page faults
- **Double Buffering**: Separate read/write buffers for lock-free data exchange between threads

## Key Algorithms/Patterns

```cpp
// RT-Safe Buffer Preallocation Pattern
class RealtimeSpectral {
private:
    // Preallocated in prepareToPlay() - never resized in processBlock()
    juce::AudioBuffer<float> fftBuffer_;           // FFT_SIZE samples
    juce::AudioBuffer<float> windowBuffer_;        // FFT_SIZE window coefficients  
    juce::AudioBuffer<float> overlapBuffer_;       // (FFT_SIZE - HOP_SIZE) overlap storage
    std::unique_ptr<juce::dsp::FFT> forwardFFT_;   // FFT processor object
    std::unique_ptr<juce::dsp::FFT> reverseFFT_;   // iFFT processor object
    
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        // ONE-TIME allocation - RT-safe zone ends here
        const int channels = 2;  // Max channels for plugin
        fftBuffer_.setSize(channels, AtlasConfig::FFT_SIZE);
        windowBuffer_.setSize(1, AtlasConfig::FFT_SIZE);
        overlapBuffer_.setSize(channels, AtlasConfig::FFT_SIZE - AtlasConfig::HOP_SIZE);
        
        // FFT objects allocated once
        forwardFFT_ = std::make_unique<juce::dsp::FFT>(juce::dsp::FFT::Order::order512);
        reverseFFT_ = std::make_unique<juce::dsp::FFT>(juce::dsp::FFT::Order::order512);
        
        // Clear all buffers to prevent artifacts
        fftBuffer_.clear();
        overlapBuffer_.clear();
    }
    
    // processBlock() - RT-SAFE ZONE: No allocations allowed
    void processBlock(const float* in, float* out, int numSamples) noexcept {
        // Only use preallocated buffers and stack variables
        // Computational complexity: O(n log n) for FFT, O(n) for windowing
    }
};
```

## RT-Audio Specific Considerations

### ✅ DO:
- **Preallocate ALL buffers** in prepareToPlay() based on worst-case scenarios
- **Use juce::AudioBuffer<float>** for audio data - optimized for audio processing
- **Implement RAII patterns** - automatic cleanup prevents resource leaks
- **Size buffers generously** - slight over-allocation better than reallocation
- **Use alignas(16) for SIMD** buffers when manual memory management required
- **Clear buffers after allocation** to prevent audio artifacts from uninitialized memory

### ❌ DON'T:
- **Call std::vector::resize()** in processBlock() - triggers heap allocation
- **Use std::unique_ptr::reset()** in audio thread - can allocate
- **Create temporary objects** with dynamic allocation in hot paths
- **Call any OS functions** (malloc, free, mmap) from audio callbacks
- **Use std::string operations** in processBlock() - hidden allocations
- **Resize any container** (std::vector, std::deque, juce::Array) during processing

## SpectralCanvasPro Integration

### MaskDeltaQueue (Lock-Free UI→Audio Communication):
```cpp
// Lock-free SPSC queue for UI paint commands
class MaskDeltaQueue {
private:
    // Preallocated circular buffer - fixed size
    static constexpr size_t QUEUE_SIZE = 1024;  // Power of 2 for efficient modulo
    alignas(64) MaskColumnDelta buffer_[QUEUE_SIZE];  // Cache line aligned
    
    std::atomic<size_t> writePos_{0};   // UI thread writes here
    std::atomic<size_t> readPos_{0};    // Audio thread reads here
    
public:
    // RT-Safe: Only atomic reads, no allocations
    bool tryPop(MaskColumnDelta& delta) noexcept {
        const auto currentRead = readPos_.load(std::memory_order_relaxed);
        const auto currentWrite = writePos_.load(std::memory_order_acquire);
        
        if (currentRead == currentWrite) return false;  // Queue empty
        
        delta = buffer_[currentRead & (QUEUE_SIZE - 1)];  // Efficient modulo
        readPos_.store(currentRead + 1, std::memory_order_release);
        return true;
    }
};
```

### Double-Buffered MaskAtlas Pattern:
```cpp
class MaskAtlas {
private:
    // Two complete mask buffers - alternate between read/write
    alignas(16) float frontBuffer_[AtlasConfig::TOTAL_MASK_SIZE];
    alignas(16) float backBuffer_[AtlasConfig::TOTAL_MASK_SIZE];
    
    std::atomic<bool> bufferSelector_{false};  // Atomic flip between buffers
    
public:
    // RT-Safe: Audio thread always reads from stable buffer
    const float* getReadBuffer() const noexcept {
        return bufferSelector_.load(std::memory_order_acquire) ? 
               frontBuffer_ : backBuffer_;
    }
    
    // UI thread writes to opposite buffer, then atomically flips
    void commitUpdates() noexcept {
        bufferSelector_.store(!bufferSelector_.load(std::memory_order_relaxed), 
                             std::memory_order_release);
    }
};
```

### HopScheduler Integration:
- **Bounded Queue Drains**: Maximum 16 deltas per processBlock() call - prevents unbounded work
- **Preallocated Delta Storage**: MaskColumnDelta objects stored in fixed arrays
- **Atomic Counters**: Track queue depth without locks using std::atomic<uint32_t>

## When to Use Different Patterns

### Fixed-Size Circular Buffers:
- **UI→Audio communication** where data rate is predictable
- **Delay lines and filter histories** with known maximum length
- **Performance critical paths** where allocation overhead unacceptable

### Object Pooling:
- **Expensive objects** like FFT instances, filter coefficients arrays
- **Variable-size processing** where objects can be reused
- **Memory-constrained environments** where fragmentation problematic

### Double Buffering:
- **Large data structures** updated infrequently (spectral masks, filter banks)
- **Cross-thread data sharing** where atomic updates required
- **Rendering pipelines** where consistent data snapshots needed

## Memory Layout Optimization

### Cache-Friendly Access Patterns:
```cpp
// BAD: Random access pattern - cache misses
for (int bin = 0; bin < NUM_BINS; ++bin) {
    for (int time = 0; time < WINDOW_LENGTH; ++time) {
        process(spectrogramData[time][bin]);  // Non-sequential access
    }
}

// GOOD: Sequential access pattern - cache efficient
for (int time = 0; time < WINDOW_LENGTH; ++time) {
    for (int bin = 0; bin < NUM_BINS; ++bin) {
        process(spectrogramData[time][bin]);  // Sequential access
    }
}
```

### SIMD Alignment:
- **16-byte alignment** for SSE operations on float arrays
- **32-byte alignment** for AVX operations where supported
- **Cache line alignment** (64 bytes) for frequently accessed atomic variables

## Common RT-Violations and Fixes

### Vector Resize in processBlock():
```cpp
// VIOLATION: std::vector resize triggers allocation
std::vector<float> tempBuffer;
tempBuffer.resize(newSize);  // FORBIDDEN in processBlock()

// FIX: Preallocate maximum size
std::vector<float> tempBuffer_;  // Member variable
// In prepareToPlay():
tempBuffer_.resize(MAX_BUFFER_SIZE);
// In processBlock():
// Use only first 'neededSize' elements
```

### String Operations:
```cpp
// VIOLATION: String concatenation allocates
std::string debugMsg = "Processing " + std::to_string(sampleCount);  // FORBIDDEN

// FIX: Use compile-time constants or preallocated char arrays
static const char* debugMsg = "Processing block";
```

### Temporary Object Creation:
```cpp
// VIOLATION: Temporary objects may allocate
auto result = complexMath(ComplexNumber(real, imag));  // Potential allocation

// FIX: Use stack objects or member variables
ComplexNumber temp{real, imag};  // Stack allocation OK
auto result = complexMath(temp);
```

## Performance Characteristics

### Memory Access Timing:
- **L1 Cache**: ~1-3 cycles (ideal for hot processing loops)
- **L2 Cache**: ~10-20 cycles (acceptable for moderate access frequency)
- **Main RAM**: ~100-300 cycles (avoid in inner processing loops)
- **Heap Allocation**: ~1000+ cycles (absolutely forbidden in processBlock())

### Buffer Sizing Guidelines:
- **FFT Buffers**: Exact power-of-2 sizes for optimal FFT performance
- **Circular Buffers**: Power-of-2 sizes enable efficient modulo operations
- **SIMD Buffers**: Multiple of vector size (4 floats for SSE, 8 for AVX)

## Sources & References

- **JUCE Documentation**: [Real-time audio programming best practices](https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html)
- **Ross Bencina**: [Real-time audio programming 101](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing) - Foundational RT principles
- **Timur Doumler**: [Want fast C++? Know your hardware!](https://www.youtube.com/watch?v=BP6NxVxDQIs) - Cache optimization patterns
- **Understanding DSP (Lyons)**: pages 765-789 - Real-time implementation considerations
- **JUCE Source Code**: modules/juce_audio_basics/buffers/juce_AudioBuffer.h - Optimized buffer implementation

## Cross-References
- **Related Cards**: card-denormal-handling.md, card-stft-windowing-cola.md
- **SpectralCanvasPro Files**: Source/Core/MessageBus.h, Source/DSP/HopScheduler.h, Source/Core/MaskAtlas.h
- **JUCE Classes**: juce::AudioBuffer, juce::AbstractFifo, juce::SpinLock