# Gem Candidates - Modernization Guide

> Curated reusable components from ARTEFACT_Production archive with modernization checklists

## Curated Top-10 (✓ Verified)

### 1. `Source/dsp/SpscRing.h` — Lock-free SPSC ring buffer ✓
**Tags**: RT-Safe, SPSC  
**Why**: Perfect lock-free single-producer/single-consumer implementation with power-of-2 optimization  
**Risks**: —

**Modernization Checklist**:
- ✅ RT audit: No new/delete/malloc/free, no std::mutex on audio thread
- ✅ Uses proper acquire/release memory ordering  
- ✅ Template-based for any trivially copyable type
- ⚠️ JUCE 8 API pass: Independent of JUCE, pure C++20
- ⚠️ Add CPU feature gates: Not SIMD-dependent but could add alignment hints
- ✅ Licensing: Clean implementation, no external dependencies

**Port Steps**:
1. Copy header directly - zero dependencies
2. Add to `/experiments/rt-safe-collections/`
3. Unit test with audio thread simulation
4. Consider cache-line alignment on capacity

---

### 2. `Source/Core/CommandQueue.h` — RT-safe command queue ✓
**Tags**: RT-Safe, APVTS  
**Why**: JUCE-based thread-safe command queue with overflow protection and statistics  
**Risks**: —

**Modernization Checklist**:
- ✅ RT audit: Uses JUCE::AbstractFifo (RT-safe)
- ✅ Preallocated command buffer, no runtime allocation
- ⚠️ JUCE 8 API pass: Verify AbstractFifo API compatibility
- ✅ Atomics: Uses proper memory fencing
- ⚠️ Unit test parity: Create tests for queue overflow scenarios

**Port Steps**:
1. Extract `Commands.h` dependency first
2. Update JUCE includes for current version
3. Add to `/experiments/rt-communication/`
4. Test with high-frequency command scenarios

---

### 3. `Source/Core/AtomicOscillator.h` — Lock-free oscillator with SIMD ✓
**Tags**: SIMD, RT-Safe, DSP  
**Why**: Lock-free parameter updates with Intel SIMD optimization and denormal suppression  
**Risks**: Windows/Intel-specific SIMD

**Modernization Checklist**:
- ✅ RT audit: Atomic parameter updates, no blocking
- ⚠️ SIMD: Intel-specific (immintrin.h), needs cross-platform abstraction
- ✅ ScopedNoDenormals: Already implemented
- ⚠️ CPU feature gates: Add runtime SIMD detection
- ⚠️ Fallback path: Create scalar version for non-Intel CPUs
- ⚠️ Unit test parity: Test SIMD vs scalar output matching

**Port Steps**:
1. Extract SIMD code to platform abstraction layer
2. Add JUCE_USE_SIMD conditional compilation
3. Create scalar fallback implementation
4. Add to `/experiments/dsp-engines/`
5. Cross-platform testing (ARM, AMD)

---

### 4. `Source/Core/RealtimeMemoryManager.h` — Zero-allocation pools ✓
**Tags**: RT-Safe, Memory  
**Why**: Complete real-time memory management with leak detection  
**Risks**: —

**Modernization Checklist**:
- ✅ RT audit: Pre-allocated pools, lock-free allocation
- ✅ RAII-style management with automatic cleanup
- ✅ Performance monitoring included
- ⚠️ Memory-mapped buffers: Verify large allocation strategy
- ✅ Statistics: Atomic counters for thread safety

**Port Steps**:
1. Independent header, minimal dependencies
2. Add to `/experiments/memory-management/`
3. Stress test with rapid allocate/deallocate cycles
4. Profile memory fragmentation over time

---

### 5. `Source/Spectral/STFTEngine.h` — STFT implementation ✓
**Tags**: DSP, FFT  
**Why**: Clean STFT with configurable parameters, minimal interface  
**Risks**: —

**Modernization Checklist**:
- ✅ RT audit: Simple interface, likely preallocated internally
- ⚠️ JUCE 8 API pass: Uses juce_audio_basics, verify FFT API
- ⚠️ Window functions: Check if windowing is implemented
- ⚠️ Overlap-add: Verify reconstruction quality
- ⚠️ Unit test parity: Add identity transform tests

**Port Steps**:
1. Check STFTEngine.cpp for implementation details  
2. Verify JUCE FFT API usage
3. Add to `/experiments/spectral-processing/`
4. Create benchmark against reference implementation

---

### 6. `Source/Core/ObjectPool.h` — RT-safe pooling template ✓
**Tags**: RT-Safe, Memory  
**Why**: Template-based object pooling with RAII and statistics  
**Risks**: —

**Modernization Checklist**:
- ✅ RT audit: Lock-free atomic indices, pre-allocated storage
- ✅ RAII wrapper for automatic return to pool
- ✅ Thread-safe acquire/release operations
- ✅ Statistics for monitoring pool utilization
- ⚠️ Template specialization: Test with complex object types

**Port Steps**:
1. Header-only template, zero dependencies
2. Add to `/experiments/memory-management/`
3. Test with audio objects (Voice, Filter instances)
4. Profile pool efficiency vs standard allocation

---

### 7. `Source/Core/GPUAccelerationEngine.h` — Multi-API GPU framework ✓
**Tags**: GPU  
**Why**: Comprehensive GPU compute supporting Metal/CUDA/OpenCL/DirectCompute  
**Risks**: Complex API dependencies, device management

**Modernization Checklist**:
- ⚠️ GPU: Verify API versions (Metal 3, CUDA 12, OpenCL 3.0)
- ⚠️ Keep CPU reference path: Ensure fallback implementations
- ⚠️ Hide behind capability flag: Runtime GPU detection
- ⚠️ No blocking on UI thread: Async GPU operations
- ⚠️ Device lost recovery: Robust error handling

**Port Steps**:
1. Extract GPU abstraction layer first
2. Create minimal test with compute shader
3. Add to `/experiments/gpu-acceleration/`
4. Start with single API (Metal or OpenCL)
5. Add device enumeration and capability detection

---

### 8. `Source/Core/CEM3389Filter.h` — Analog-modeled filter ✓
**Tags**: DSP, Analog  
**Why**: E-mu CEM3389 filter emulation with nonlinear characteristics  
**Risks**: —

**Modernization Checklist**:
- ✅ RT audit: Likely stateful but should be RT-safe
- ⚠️ JUCE 8 API pass: Uses JuceHeader.h, update includes
- ⚠️ Parameter smoothing: Add SmoothedValue for cutoff/resonance
- ⚠️ Unit test parity: Frequency response validation
- ⚠️ SIMD: Consider vectorizing filter stages

**Port Steps**:
1. Extract filter coefficients and algorithms
2. Add parameter smoothing with JUCE SmoothedValue
3. Add to `/experiments/dsp-filters/`
4. Create frequency response tests
5. Profile CPU usage vs other filter types

---

### 9. `Source/Core/PluginProcessor.h` — APVTS integration patterns ✓
**Tags**: APVTS, Plugin  
**Why**: Reference implementation for JUCE plugin with parameter management  
**Risks**: RT-Risk flagged (potential unsafe operations)

**Modernization Checklist**:
- ⚠️ RT audit: Review for RT-unsafe operations (flagged as RT-Risk)
- ✅ APVTS: Parameter tree state management
- ⚠️ JUCE 8 API pass: Update deprecated AudioProcessor methods
- ⚠️ Parameter IDs: Ensure stable parameter ID scheme
- ⚠️ SmoothedValue: Use for fast-changing parameters

**Port Steps**:
1. **Audit for RT violations first** (malloc/file I/O on audio thread)
2. Extract parameter management patterns
3. Update to current JUCE AudioProcessor API
4. Add to `/experiments/plugin-architecture/`
5. Create minimal working plugin template

---

### 10. `Source/Core/DoubleBuffer.h` — Double buffering utilities ✓
**Tags**: RT-Safe, DoubleBuffer  
**Why**: Thread-safe double buffering for shared data  
**Risks**: —

**Modernization Checklist**:
- ✅ RT audit: Atomic buffer swapping, no blocking
- ✅ Thread safety: Lock-free buffer switching
- ⚠️ Memory ordering: Verify acquire/release semantics
- ⚠️ Generic implementation: Template for different data types
- ⚠️ Unit test parity: Test concurrent reader/writer scenarios

**Port Steps**:
1. Verify atomic operations and memory barriers
2. Add to `/experiments/rt-safe-collections/`
3. Test with high-frequency data updates
4. Consider integration with JUCE ValueTree

---

## Universal Modernization Checklist

Apply to all gems before integration:

### RT Safety Audit
- [ ] No `new/delete/malloc/free` on audio thread
- [ ] No `std::mutex/std::lock_guard` on audio thread  
- [ ] No file I/O or system calls on audio thread
- [ ] Preallocate in `prepareToPlay`
- [ ] Use `ScopedNoDenormals` in processing loops

### JUCE 8 Compatibility  
- [ ] Remove deprecated calls (`AudioProcessor::getTailLengthSeconds` → `getTailLengthSeconds`)
- [ ] Update include paths (`<JuceHeader.h>` → specific modules)
- [ ] APVTS parameter IDs remain stable across versions
- [ ] Use `SmoothedValue` for continuously changing parameters

### Cross-Platform SIMD
- [ ] Provide CPU feature detection (`JUCE_USE_SSE_INTRINSICS`)
- [ ] Create scalar fallback implementations
- [ ] Unit test SIMD vs scalar output matching
- [ ] Test on ARM (Apple Silicon, Raspberry Pi)

### GPU Robustness
- [ ] Keep CPU reference implementation
- [ ] Hide GPU features behind capability flags
- [ ] No blocking operations on UI thread
- [ ] Graceful degradation on device lost
- [ ] Validate shader compilation at runtime

### Memory Safety
- [ ] Use RAII for all resource management
- [ ] Atomic operations use proper memory ordering
- [ ] Avoid ABA problems (prefer indices over pointers)
- [ ] Profile for memory leaks in long-running tests

## Extraction Recommendations

### High Priority (`/experiments/rt-safe-core/`)
1. **SpscRing.h** - Foundation for all RT communication
2. **ObjectPool.h** - Essential for RT memory management  
3. **DoubleBuffer.h** - Core synchronization primitive

### Medium Priority (`/experiments/dsp-foundation/`)  
4. **STFTEngine.h** - Spectral processing base
5. **CEM3389Filter.h** - Reference analog modeling
6. **AtomicOscillator.h** - RT-safe DSP with SIMD

### Integration Layer (`/experiments/plugin-framework/`)
7. **CommandQueue.h** - UI-to-audio communication
8. **PluginProcessor.h** - JUCE integration patterns (after audit)
9. **RealtimeMemoryManager.h** - Complete memory solution

### Advanced Features (`/experiments/gpu-compute/`)
10. **GPUAccelerationEngine.h** - When GPU acceleration needed

Each experiment directory should include:
- `README.md` with integration notes
- `CMakeLists.txt` for building
- `tests/` directory with unit tests  
- `examples/` with minimal usage examples