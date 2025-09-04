# FFT Optimization vs JUCE

## Problem Statement
- Current implementation: Using `juce::dsp::FFT` for frequency-only forward transforms in STFT and analysis widgets.
- Performance issue: UI stutters at large N (e.g., 4096–8192) and dense updates; CPU spikes > budget.
- Constraint: RT-audio must not starve; GUI updates must remain smooth.
- Goal: ≥20% faster than JUCE baseline for target sizes without accuracy loss.

## JUCE Baseline Analysis
### What JUCE Already Offers:
- Relevant Classes: `juce::dsp::FFT`, `juce::dsp::WindowingFunction`.
- Performance Baseline: Measure with `SpectralCanvasPro::Validation::DSPBenchmark`.
- Pros: Stable, portable, easy to use.
- Cons: Not tailored to batched, cache-friendly streaming; no plan reuse across threads.

### When to Use JUCE vs Custom:
- Use JUCE when: small FFT sizes (<1024), sporadic calls, simplicity matters.
- Go custom when: repeated, batched transforms; large sizes; strict CPU budget.
- Never worth optimizing: low-rate, non-critical analysis paths.

## Core Concepts (Theory)
- Key Principle 1: Plan reuse and memory alignment increase throughput.
- Key Principle 2: Batched transforms improve cache locality and SIMD utilization.
- Key Principle 3: Window normalization and COLA preserve amplitude accuracy.

## Implementation Strategy
1. Measure Current Performance: Use `DSPBenchmark::measurePerformance` on JUCE and custom.
2. Implement Textbook Solution: Cooley–Tukey radix-2 with in-place, aligned buffers.
3. Optimize for RT Constraints: Preallocate work buffers in `prepareToPlay()`; batch per column.
4. Validate Correctness: Compare magnitude bins via `AccuracyValidator::compareBuffers`.
5. Profile Performance: Require ≥20% improvement averaged across iterations.

## Code Template
```cpp
#include "tools/dsp_validation_framework.h"
using namespace SpectralCanvasPro::Validation;

void validateCustomFFT(int fftOrder) {
    FFTValidationTest validator(fftOrder);

    auto customFFT = [](float* buffer) noexcept {
        // TODO: call your custom FFT (frequency-only forward)
    };

    jassert(validator.validateAccuracy(customFFT, 1e-6f));
    jassert(validator.validatePerformance(customFFT, 20.0f));
}
```

## RT-Audio Reality Check
- Latency Target: No added audio path latency; GUI throughput ≥ 60 FPS.
- CPU Budget: Keep per-frame FFT work < X% on target machine.
- Memory Constraint: All buffers preallocated; no allocs during processing.
- Checklist: Accuracy verified; ≥20% perf win; RT-safety maintained; edge cases covered.

## Decision Framework
- Use Custom When: ≥20% faster; stable across N={1024..8192}; maintains accuracy.
- Stick with JUCE When: Gains <20% or increased complexity risks maintenance.

## Validation Protocol
- Correctness: `FFTValidationTest::validateAccuracy` on sine/noise/sweep/impulse.
- Performance: `validatePerformance` for 10k iters; report average µs and stddev.
- Stress: Vary window/hop sizes; random inputs near denormal range.

## Sources & References
- Primary Source: Cooley–Tukey radix-2 FFT; JUCE docs for `juce::dsp::FFT`.
- Validation Reference: `tools/dsp_validation_framework.h`.

## Lessons Learned
- Fill after implementation.

