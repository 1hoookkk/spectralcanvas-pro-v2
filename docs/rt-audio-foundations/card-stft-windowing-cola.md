# STFT Windowing & COLA Compliance

## Core Concepts

- **COLA (Constant Overlap-Add)**: Mathematical requirement that windowed segments sum to unity for perfect reconstruction
- **Spectral Leakage**: Frequency domain artifacts caused by finite-length windowing; minimized by proper window selection
- **75% Overlap Requirement**: SpectralCanvasPro's FFT_SIZE=512, HOP_SIZE=128 configuration requires COLA-compliant windows
- **Hann Window Optimality**: Best balance of main lobe width (4 bins) vs side lobe suppression (-31.5 dB) for most applications
- **Window Function Trade-offs**: Blackman (better side lobe suppression) vs Hann (narrower main lobe) vs Kaiser (adjustable parameters)
- **Reconstruction Accuracy**: COLA compliance ensures bit-perfect reconstruction when no spectral modifications applied
- **Frequency Resolution**: Effective frequency resolution determined by main lobe width, not FFT bin spacing

## Key Algorithms/Equations

```cpp
// COLA Verification for 75% overlap (4x oversampling)
// Window function w[n] must satisfy: Σ w[n + rR] = constant
// where R = HOP_SIZE = 128, r = 0,1,2,3 for 4 overlapped frames

// Hann Window (COLA-compliant for 75% overlap)
float hannWindow(int n, int N) {
    return 0.5f * (1.0f - std::cos(2.0f * M_PI * n / (N - 1)));
    // Computational complexity: O(1) per sample
    // Cite: Julius O. Smith, "Spectral Audio Signal Processing", Ch. 3
}

// Blackman Window (COLA-compliant alternative)  
float blackmanWindow(int n, int N) {
    const float a0 = 0.42f, a1 = 0.5f, a2 = 0.08f;
    return a0 - a1 * std::cos(2.0f * M_PI * n / (N-1)) + a2 * std::cos(4.0f * M_PI * n / (N-1));
    // Better side lobe suppression (-58 dB) but wider main lobe (6 bins)
    // Cite: Understanding DSP (Lyons), p. 445-448
}
```

## RT-Audio Specific Considerations

### ✅ DO:
- **Precompute window coefficients** in `prepareToPlay()` and store in preallocated buffer
- **Verify COLA compliance** mathematically before using any window function
- **Apply ScopedNoDenormals** before windowing operations to prevent denormal slowdowns
- **Use float32 precision** for window coefficients (sufficient for 24-bit audio)
- **Implement window application** as simple multiply (no conditionals in inner loop)

### ❌ DON'T:
- **Compute window functions** inside processBlock() - precompute and store instead
- **Use arbitrary window functions** without COLA verification for your overlap ratio
- **Apply different windows** to analysis and synthesis (breaks perfect reconstruction)
- **Ignore main lobe width** when designing spectral processing algorithms
- **Use double precision** for window coefficients in RT contexts (unnecessary overhead)

## SpectralCanvasPro Integration

### RealtimeSpectral Class Integration:
```cpp
// In RealtimeSpectral.h - precomputed window storage
class RealtimeSpectral {
    alignas(16) float windowCoeffs_[AtlasConfig::FFT_SIZE];  // SIMD-aligned
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        // Precompute Hann window for 512 samples
        for (int i = 0; i < AtlasConfig::FFT_SIZE; ++i) {
            windowCoeffs_[i] = 0.5f * (1.0f - std::cos(2.0f * M_PI * i / 511.0f));
        }
    }
};
```

### COLA Compliance for 75% Overlap:
- **Analysis Window**: Apply window function to input before FFT
- **Synthesis Window**: Same window function applied to iFFT output
- **Overlap Factor**: 4x overlap (512/128) ensures COLA compliance with Hann window
- **Amplitude Correction**: None required - COLA property handles reconstruction scaling

### Integration with MaskAtlas Pipeline:
- **Pre-windowing**: Input audio windowed before FFT in HopScheduler
- **Spectral Masking**: Applied to windowed FFT coefficients
- **Post-windowing**: iFFT output windowed before overlap-add reconstruction
- **Phase Preservation**: Window function preserves phase relationships for spectral masking

## When to Use Different Windows

### Hann Window (Default Choice):
- **General spectral processing** with good balance of resolution vs leakage
- **Spectral masking applications** where moderate side lobe suppression sufficient
- **Real-time applications** where computational efficiency important

### Blackman Window (Higher Quality):
- **Critical listening applications** requiring maximum side lobe suppression
- **Spectral analysis** where leakage between bins must be minimized
- **Non-real-time processing** where CPU overhead acceptable

### Kaiser Window (Research/Specialized):
- **Custom applications** requiring specific main lobe width vs side lobe trade-offs
- **Experimental spectral processing** with adjustable window parameters
- **Academic research** comparing different windowing strategies

## Common Artifacts and Solutions

### Amplitude Modulation (AM) Effects:
- **Cause**: Non-COLA-compliant windows or incorrect overlap ratio
- **Solution**: Verify mathematical COLA property for your specific hop size
- **Test**: Process sine wave through STFT pipeline - should be bit-perfect

### Spectral Smearing:
- **Cause**: Window main lobe too wide for spectral content
- **Solution**: Consider narrower main lobe window (Hann vs Blackman trade-off)
- **SpectralCanvasPro Impact**: Affects precision of spectral masking

### Transient Pre-echo:
- **Cause**: Window length too long for transient material
- **Solution**: Fixed by 512-sample window length (10.7ms @ 48kHz) - good compromise
- **RT Constraint**: Cannot adaptively change window length in real-time

## Sources & References

- **Julius O. Smith CCRMA**: [Spectral Audio Signal Processing, Chapter 3-4](https://ccrma.stanford.edu/~jos/sasp/Windowing.html) - Mathematical foundations and COLA theory
- **Understanding DSP (Lyons)**: pages 445-465 - Practical window function comparisons
- **Audio Effects: Theory, Implementation and Application**: Chapter 8 - STFT implementation patterns
- **JUCE Documentation**: WindowingFunction class reference - Implementation details

## Cross-References
- **Related Cards**: card-phase-vocoder.md, card-rt-safe-memory-patterns.md
- **SpectralCanvasPro Files**: Source/DSP/RealtimeSpectral.h, Source/Core/AtlasIds.h
- **JUCE Classes**: juce::dsp::WindowingFunction, juce::ScopedNoDenormals