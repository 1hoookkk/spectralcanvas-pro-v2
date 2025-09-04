# Task: Fix FFT Windowing Performance Benchmark

**Priority:** Medium  
**Complexity:** Low  
**Owner:** System  
**Status:** Active  

## Problem Statement

The current BenchWindowFFT benchmark incorrectly measures performance by recalculating expensive Hann window coefficients (`cosf()`) on every frame, which dominates timing and masks the actual optimization benefits. This produces misleading results showing a performance regression instead of the expected improvement.

## Root Cause Analysis

### Current Benchmark Issues
1. **Recalculates window coefficients**: Each frame computes `0.5f * (1.0f - cos(2π*i/(N-1)))` for every sample
2. **Hides real optimization**: cos() calculation (expensive) dominates vs memory bandwidth savings (cheap)  
3. **Wrong performance expectations**: Claims 20-30μs improvement on a 14μs total operation
4. **Not testing real implementation**: Actual `RealtimeSpectral.h` uses precomputed `anaWinTable_[]`

### Expected Real Performance
For 512-point FFT, the true optimization saves:
- 1 extra `memcpy` (2KB)  
- Separate windowing loop iteration
- **Expected improvement: 2-10%** (0.3-1.5μs), not tens of microseconds

## Technical Solution

### Fix the Benchmark
Replace the current approach with three proper test cases:

1. **Naive (baseline)**: Recompute Hann + copy + FFT (reproduces current regression)
2. **Precomputed Separate**: Copy + multiply with cached coefficients + FFT  
3. **Precomputed Fused**: Single-pass `dest[i] = src[i] * win[i]` + FFT

### Implementation Details
- Precompute Hann window once at initialization
- Use `juce::FloatVectorOperations::multiply()` for vectorized operations
- Add thread pinning for stable measurements (`SetThreadAffinityMask`)
- Test with compiler optimizations (`/arch:AVX2`, `/O2`, `/fp:fast`)

## Files to Modify

### Primary Changes
- `Tests/BenchWindowFft.cpp` - Replace with corrected benchmark implementation
- `artifacts/perf/bench-window-fft.md` - Update with accurate analysis

### Optional Enhancements  
- `Tests/CMakeLists.txt` - Add standalone benchmark target
- `artifacts/perf/bench-window-fft-standalone.cpp` - Separate micro-benchmark

## Acceptance Criteria

### Must Have
- [ ] Benchmark uses precomputed window tables (no cos() per frame)
- [ ] Tests three distinct approaches with clear performance comparison
- [ ] Performance documentation reflects realistic expectations (2-10% improvement)
- [ ] Results show actual optimization benefit vs memory bandwidth tradeoff

### Should Have  
- [ ] Thread pinning for measurement stability
- [ ] SIMD-optimized paths where applicable
- [ ] Multiple FFT sizes (512, 1024, 2048) to show scaling

### Could Have
- [ ] AVX2/AVX512 performance comparison
- [ ] Standalone CMake target for isolated benchmarking
- [ ] Integration with CI for performance regression detection

## Dependencies
- JUCE DSP module (already available)
- Windows thread affinity APIs (for measurement stability)
- Existing `RealtimeSpectral.h` optimization as reference

## Timeline
**Estimated effort:** 2-3 hours
- Benchmark code fix: 1 hour
- Documentation update: 30 minutes  
- Testing and validation: 1 hour
- CMake integration: 30 minutes

## Success Metrics
- Benchmark shows 2-10% improvement for fused approach
- Performance claims match measured reality
- No performance regressions in other test variants
- Documentation accurately reflects optimization benefits

## Notes
This task directly validates the fused windowing+FFT optimization merged in commit 554d37f. Accurate benchmarking is essential for understanding the real-world performance impact of our RT-safe optimizations.