# Denormal Handling in Spectral Processing

## Core Concepts

- **Denormal Numbers**: IEEE 754 floating-point values smaller than the minimum normalized value (~1.18e-38 for float32)
- **Performance Impact**: Denormal arithmetic can be 100-1000x slower than normal floating-point operations
- **STFT Vulnerability**: Spectral processing creates many near-zero values prone to becoming denormals
- **FTZ (Flush-To-Zero)**: CPU mode that treats denormals as zero for faster processing
- **DAZ (Denormals-Are-Zero)**: CPU mode that treats denormal inputs as zero before operations
- **Hardware Detection**: Modern CPUs can automatically flush denormals, but must be explicitly enabled
- **Cascade Effect**: One denormal value can contaminate an entire processing chain

## Key Algorithms/Patterns

```cpp
// JUCE ScopedNoDenormals - Automatic RAII denormal protection
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override {
    juce::ScopedNoDenormals noDenormals;  // Enables FTZ/DAZ for this scope
    
    // All floating-point operations protected until function exit
    // Computational overhead: ~2-3 CPU cycles setup, massive savings during denormals
    // Cite: JUCE Documentation - ScopedNoDenormals class reference
}

// Manual Denormal Clamping (when ScopedNoDenormals insufficient)
inline float clampDenormal(float value) noexcept {
    constexpr float DENORMAL_THRESHOLD = 1e-15f;  // Well above denormal range
    return (std::abs(value) < DENORMAL_THRESHOLD) ? 0.0f : value;
    // Alternative: return std::fpclassify(value) == FP_SUBNORMAL ? 0.0f : value;
}

// SIMD-Optimized Denormal Clamping (for tight loops)
inline void clampDenormalsSSE(float* buffer, int numSamples) noexcept {
    const __m128 threshold = _mm_set1_ps(1e-15f);
    const __m128 negThreshold = _mm_set1_ps(-1e-15f);
    
    for (int i = 0; i < numSamples; i += 4) {
        __m128 values = _mm_load_ps(&buffer[i]);
        __m128 mask = _mm_and_ps(_mm_cmpgt_ps(values, threshold), 
                                _mm_cmplt_ps(values, negThreshold));
        values = _mm_and_ps(values, mask);  // Zero values within denormal range
        _mm_store_ps(&buffer[i], values);
    }
    // Processes 4 samples simultaneously - 4x speedup over scalar version
}
```

## RT-Audio Specific Considerations

### ✅ DO:
- **Always wrap processBlock()** with juce::ScopedNoDenormals - zero overhead when no denormals
- **Enable FTZ/DAZ globally** at plugin initialization for maximum protection
- **Monitor CPU usage spikes** during development - sudden increases often indicate denormal issues
- **Test with near-silent input** - expose denormal generation in decay/reverb tails
- **Use profiling tools** to identify denormal hotspots in spectral processing chains
- **Clamp filter feedback** values that can accumulate denormals over time

### ❌ DON'T:
- **Ignore sudden CPU spikes** - often first sign of denormal performance issues  
- **Use std::fpclassify()** in tight processing loops - too expensive for RT use
- **Forget denormal protection** in filter implementations and delay lines
- **Apply excessive clamping** - can introduce audible artifacts if threshold too high
- **Assume hardware handles everything** - some older CPUs don't support FTZ/DAZ
- **Use denormal protection** as excuse for poor algorithm design

## SpectralCanvasPro Integration

### RealtimeSpectral Denormal Protection:
```cpp
void RealtimeSpectral::processBlock(const float* in, float* out, int numSamples, 
                                   const float* maskCol) noexcept {
    juce::ScopedNoDenormals noDenormals;  // Essential for spectral processing
    
    // FFT operations create many near-zero spectral bins
    forwardFFT_->performFrequencyOnlyForwardTransform(fftBuffer_.getWritePointer(0));
    
    // Spectral masking can create denormals from attenuation
    for (int bin = 0; bin < AtlasConfig::NUM_BINS; ++bin) {
        const float maskValue = maskCol[bin];  // 0.0-1.0 range
        const float realPart = fftBuffer_.getSample(0, bin * 2);
        const float imagPart = fftBuffer_.getSample(0, bin * 2 + 1);
        
        // Masking multiplication can produce denormals
        fftBuffer_.setSample(0, bin * 2, realPart * maskValue);
        fftBuffer_.setSample(0, bin * 2 + 1, imagPart * maskValue);
        // ScopedNoDenormals handles any denormals produced here
    }
    
    // iFFT and overlap-add also vulnerable to denormal accumulation
    reverseFFT_->performRealOnlyInverseTransform(fftBuffer_.getWritePointer(0));
}
```

### HopScheduler Denormal Management:
```cpp
class HopScheduler {
    void drainAndApply(uint32_t maxDeltas = 16) noexcept {
        juce::ScopedNoDenormals noDenormals;  // Protect mask value operations
        
        MaskColumnDelta delta;
        uint32_t processed = 0;
        
        while (processed < maxDeltas && maskDeltaQueue_.tryPop(delta)) {
            // Mask values can be very small (near denormal range)
            for (int bin = 0; bin < AtlasConfig::NUM_BINS; ++bin) {
                float maskValue = delta.values[bin];
                // ScopedNoDenormals prevents denormal arithmetic here
                mask_.writeStagingColumn(delta.position, bin, maskValue);
            }
            ++processed;
        }
    }
};
```

### Global Denormal Protection Setup:
```cpp
// In plugin constructor or initialization
void SpectralCanvasProAudioProcessor::enableGlobalDenormalProtection() {
    // Enable FTZ (Flush-To-Zero) and DAZ (Denormals-Are-Zero) CPU modes
    #if JUCE_INTEL
        _mm_setcsr(_mm_getcsr() | 0x8040);  // Enable FTZ and DAZ bits
    #elif JUCE_ARM
        // ARM processors typically handle denormals more efficiently
        // iOS/Android usually have denormal protection enabled by default
    #endif
    
    // JUCE provides cross-platform denormal protection
    juce::FloatVectorOperations::enableFlushToZeroMode(true);
}
```

## When Denormals Occur in STFT

### Common Denormal Sources:
- **Spectral Bin Attenuation**: Masking values near zero create denormal FFT coefficients
- **Window Function Tails**: Hann/Blackman windows have very small edge values
- **Filter Decay**: IIR filter states can decay into denormal range
- **Overlap-Add Accumulation**: Repeated small additions can produce denormals
- **Phase Vocoder Processing**: Phase unwrapping math can generate tiny intermediate values

### Detection Strategies:
```cpp
// Development-time denormal detection (remove in release builds)
#ifdef JUCE_DEBUG
void detectDenormals(const juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        const float* data = buffer.getReadPointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            if (std::fpclassify(data[i]) == FP_SUBNORMAL) {
                DBG("Denormal detected at channel " << ch << ", sample " << i);
                jassertfalse;  // Break in debugger
            }
        }
    }
}
#endif
```

## Performance Characteristics

### Denormal Performance Impact:
- **Normal Operation**: 1x processing time (baseline)
- **Occasional Denormals**: 2-5x processing time (moderate impact)
- **Denormal Cascade**: 10-100x processing time (severe performance degradation)
- **Complete Denormal Saturation**: 100-1000x processing time (unusable)

### Protection Overhead:
- **ScopedNoDenormals**: ~2-3 CPU cycles setup cost, massive savings during denormals
- **Manual Clamping**: ~1-2 cycles per sample (constant overhead)
- **SIMD Clamping**: ~0.25-0.5 cycles per sample (amortized over vector width)
- **Hardware FTZ/DAZ**: Nearly zero overhead, automatic protection

## Frequency Domain Considerations

### Spectral Bin Masking:
- **Deep Attenuation**: Mask values below 0.001 risk creating denormals in FFT bins
- **Smooth Transitions**: Use exponential or logarithmic scaling to avoid sudden jumps to zero
- **Frequency-Dependent Thresholds**: Human hearing thresholds can guide minimum mask values

### Phase Processing:
- **Phase Unwrapping**: Can generate very small phase differences prone to denormals
- **Phase Vocoder Synthesis**: Accumulated phase errors can decay into denormal range
- **Complex Arithmetic**: Real/imaginary parts can independently become denormal

## Common Pitfalls and Solutions

### Filter State Accumulation:
```cpp
// PROBLEM: IIR filter states accumulate denormals over time
class IIRFilter {
    float state1_, state2_;  // Can slowly decay to denormal range
    
    float process(float input) {
        // Without protection, states become denormals during silence
        float output = input + state1_ * 0.99f + state2_ * 0.01f;
        state2_ = state1_;
        state1_ = output;
        return output;
    }
};

// SOLUTION: Periodic state clamping or ScopedNoDenormals
float process(float input) {
    juce::ScopedNoDenormals noDenormals;  // Automatic protection
    // OR manual clamping every N samples:
    // if (++sampleCounter % 64 == 0) {
    //     state1_ = clampDenormal(state1_);
    //     state2_ = clampDenormal(state2_);
    // }
}
```

### Spectral Magnitude Calculations:
```cpp
// PROBLEM: Magnitude calculation can produce denormals
float magnitude = std::sqrt(real*real + imag*imag);  // Can become denormal

// SOLUTION: Use JUCE's optimized functions with denormal protection
float magnitude = juce::dsp::FastMathApproximations::sqrt(real*real + imag*imag);
```

## Testing and Validation

### Denormal Stress Testing:
1. **Silent Input Test**: Process complete silence for extended periods
2. **Decay Test**: Process exponentially decaying signals to expose filter state issues  
3. **Deep Masking Test**: Apply extreme spectral attenuation (>60dB) to expose bin denormals
4. **CPU Monitoring**: Watch for sudden CPU usage spikes during development

### Performance Benchmarking:
```cpp
// Measure processing time with and without denormal protection
auto start = std::chrono::high_resolution_clock::now();
processBlock(buffer);  // With ScopedNoDenormals
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
```

## Sources & References

- **IEEE 754 Standard**: Floating-point arithmetic specification - denormal number definition
- **Intel Software Developer's Manual**: Volume 1, Section 4.8.1.5 - FTZ and DAZ control bits
- **JUCE Documentation**: [ScopedNoDenormals class reference](https://docs.juce.com/master/classjuce_1_1ScopedNoDenormals.html)
- **Ross Bencina**: "Denormals in Audio DSP" - Practical denormal handling strategies
- **Designing Audio Effect Plug-Ins in C++ (Pirkle)**: Chapter 6 - Real-time performance optimization
- **ARM Developer Documentation**: Floating-point behavior on ARM processors

## Cross-References
- **Related Cards**: card-rt-safe-memory-patterns.md, card-stft-windowing-cola.md, card-performance-optimization.md
- **SpectralCanvasPro Files**: Source/DSP/RealtimeSpectral.h, Source/SpectralCanvasProAudioProcessor.cpp
- **JUCE Classes**: juce::ScopedNoDenormals, juce::FloatVectorOperations, juce::dsp::FastMathApproximations