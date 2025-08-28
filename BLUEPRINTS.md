# 🏗️ BLUEPRINTS.md - Design Patterns to Reference

> Architectural patterns and design experiments that provide valuable reference material for future development.

## 🎭 UI/UX Interaction Patterns

### Gesture-to-Parameter Mapping
**Pattern:** Mouse position → Normalized parameter space
**Reference:** `Source/SpectralCanvasProEditor.h` (lines 56-61)
```cpp
struct GesturePoint {
    juce::Point<float> position;  // Normalized 0-1
    float intensity;               // Pressure/velocity
    juce::uint32 timestamp;        // For gesture playback
};
```
**Key Insights:**
- Normalize early, process later
- Timestamp everything for automation recording
- Intensity as third dimension beyond X/Y

### Fast Paint Mode Pattern
**Concept:** Dynamic quality adjustment during interaction
**Reference:** `Source/Core/Params.h` (line 31)
```
Normal Mode: FFT 512, Hop 128, Full particles
Fast Mode:   FFT 256, Hop 64, Reduced particles
```
**Trade-offs:**
- Latency vs. Quality
- Responsiveness during gesture capture
- Automatic switching based on interaction state

## 🔄 Thread Communication Patterns

### Triple-Buffer Mailbox Pattern
**Reference:** `setup-spectralcanvas.ps1` (lines 88-104)
```cpp
template <typename T>
struct Mailbox {
    std::atomic<uint32_t> readIdx{0}, writeIdx{1};
    T buffers[3];
    void publish() noexcept { /* atomic swap magic */ }
};
```
**Use Cases:**
- UI → GPU state updates
- Non-critical parameter smoothing
- Debug visualization data

### Command Queue Pattern
**Reference:** `Source/Core/MessageBus.h` (lines 160-187)
```cpp
struct RenderCommand {
    enum Type { UpdateTexture, DrawGesture, SetColormap };
    Type commandType;
    union { /* command-specific data */ };
};
```
**Benefits:**
- Type-safe command dispatch
- Fixed-size for lock-free queues
- Easy to extend with new commands

## 🎨 Visual Design Patterns

### Spectral-to-Visual Mapping Strategies

#### Frequency → Color
**Pattern:** Logarithmic frequency to perceptual color space
```hlsl
// Bass = Purple (270°), Highs = Cyan (180°)
float hue = lerp(0.75, 0.5, log2(frequency) / 14.0);
```

#### Amplitude → Brightness
**Pattern:** Gamma-corrected amplitude mapping
```hlsl
brightness = pow(amplitude, 2.2);  // Perceptual linearization
```

#### Complexity → Texture
**Pattern:** Spectral variance drives visual texture
```
Pure Tones → Smooth, aurora-like
Complex → Sharp, particle-like
Noise → Grainy, dispersed
```

### Particle Behavior Patterns

#### Birth/Death Based on Transients
```
if (spectralEnergy > threshold * 1.5)
    spawnParticles(count = energy * scaleFactor)
```

#### Spectral-Driven Physics
```hlsl
particle.velocity.y += magnitude * liftForce;
particle.size *= (1.0 + magnitude * 2.0);
particle.color = frequencyToColor(bin_frequency);
```

## 🔧 DSP Architecture Patterns

### STFT Processing Pipeline
**Reference:** `Source/DSP/SpectralEngine.h`
```
Input → Window → FFT → Spectral Processing → iFFT → Overlap-Add → Output
         ↓                    ↓
    [Hann Window]      [Magnitude/Phase]
```
**Critical Parameters:**
- FFT Size: 512 (11.6ms @ 44.1kHz)
- Hop Size: 128 (75% overlap)
- Window: Hann (good frequency resolution)

### Filter Morphing Pattern
**Concept:** Smooth interpolation between filter types
```cpp
// Y-axis gesture controls morph position
float morph = gestureY;  // 0.0 to 1.0
if (morph < 0.5) {
    // Lowpass → Bandpass
    coeffs = lerp(lowpass_coeffs, bandpass_coeffs, morph * 2);
} else {
    // Bandpass → Highpass  
    coeffs = lerp(bandpass_coeffs, highpass_coeffs, (morph - 0.5) * 2);
}
```

## 🎮 Parameter Binding Patterns

### Gesture Intensity to Multiple Parameters
```cpp
void applyGestureIntensity(float intensity) {
    saturationDrive = baseDrive + intensity * driveRange;
    filterResonance = baseReso + intensity * resoBoost;
    particleSpawnRate = baseRate * (1 + intensity * 3);
}
```

### Spectral Features as Modulators
```cpp
// Spectral centroid modulates filter cutoff
filterCutoff = baseCutoff * (0.5 + spectralCentroid * 1.5);

// Fundamental frequency locks filter to pitch
if (fundamentalDetected) {
    filterCutoff = fundamentalFreq * harmonicMultiplier;
}
```

## 🏭 Resource Management Patterns

### Pre-allocation Strategy
**Reference:** `Source/DSP/SpectralEngine.cpp` (lines 30-42)
```cpp
void initialize(double sampleRate, int maxBlockSize) {
    // All allocations happen here, never in process()
    forwardFFT_ = std::make_unique<juce::dsp::FFT>(fftOrder);
    buffers_.resize(maxSize);
    // ... etc
}
```

### GPU Resource Pooling
```cpp
class TexturePool {
    std::vector<ID3D11Texture2D*> available_;
    std::vector<ID3D11Texture2D*> inUse_;
    
    ID3D11Texture2D* acquire() { /* recycle or create */ }
    void release(ID3D11Texture2D* tex) { /* return to pool */ }
};
```

## 🔐 Safety Patterns

### RT-Safe Degradation
```cpp
if (!queue.push(data)) {
    // Don't block! Degrade gracefully
    ++droppedFrameCount;
    // Maybe reduce quality temporarily
    enableFastMode();
}
```

### Device Lost Recovery (D3D11)
```cpp
HRESULT hr = device->GetDeviceRemovedReason();
if (hr != S_OK) {
    shutdownD3D11();
    initializeD3D11();  // Full restart
    reloadAllResources();
}
```

## 📊 Performance Monitoring Patterns

### Sliding Window Metrics
```cpp
class PerformanceMonitor {
    CircularBuffer<float, 120> frameTimes;  // 2 seconds @ 60fps
    
    float getAverageFrameTime() { 
        return std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f) / frameTimes.size();
    }
    
    bool isPerformanceDegraded() {
        return getAverageFrameTime() > 20.0f;  // >20ms = problem
    }
};
```

### Adaptive Quality System
```cpp
if (monitor.isPerformanceDegraded()) {
    particleCount *= 0.8;
    fftSize = 256;  // Reduce from 512
    disableExpensiveEffects();
}
```

## 🎯 Canvas Interaction Models

### Paint Accumulation Pattern
**Reference:** Setup script concept
```cpp
// Energy accumulates at gesture points
void paintAt(float x, float y, float intensity) {
    int bin = frequencyFromY(y);
    spectralBins[bin] += intensity;
    spectralBins[bin] *= 0.95f;  // Natural decay
}
```

### Spectral Sculpting Pattern
```cpp
// Gesture shapes spectral content
enum SculptMode { Boost, Cut, Morph };

void sculptSpectrum(GestureData& gesture) {
    for (auto& point : gesture.points) {
        int centerBin = point.y * NUM_BINS;
        float radius = gesture.intensity * 50;
        
        for (int bin = centerBin - radius; bin <= centerBin + radius; ++bin) {
            float distance = abs(bin - centerBin) / radius;
            float envelope = 1.0 - distance;  // Linear falloff
            
            if (mode == Boost)
                spectrum[bin] *= (1.0 + envelope * boostAmount);
            else if (mode == Cut)
                spectrum[bin] *= (1.0 - envelope * cutAmount);
        }
    }
}
```

## 📐 Layout Experiments

### Modular Panel System
```
┌─────────────────────────────┐
│      Spectral Canvas        │  80% height
├─────────┬─────────┬─────────┤
│ Filter  │ Params  │ Visual  │  20% height
└─────────┴─────────┴─────────┘
```

### Overlay Control Pattern
```cpp
class OverlayControls : public Component {
    bool isVisible = false;
    float opacity = 0.0f;
    
    void showOnHover() {
        isVisible = true;
        startAnimation(opacity, 0.0f, 1.0f, 200ms);
    }
};
```

## 🚀 Optimization Patterns

### SIMD-Friendly Data Layout
```cpp
// Structure of Arrays (SoA) for vectorization
struct ParticleSystemSoA {
    alignas(32) float posX[MAX_PARTICLES];
    alignas(32) float posY[MAX_PARTICLES];
    alignas(32) float velX[MAX_PARTICLES];
    alignas(32) float velY[MAX_PARTICLES];
};
```

### Batch Processing Pattern
```cpp
// Process multiple spectral frames together
void processSpectralBatch(SpectralFrame frames[4]) {
    // SIMD process 4 frames at once
    __m128 mag0 = _mm_load_ps(frames[0].magnitude);
    __m128 mag1 = _mm_load_ps(frames[1].magnitude);
    // ... etc
}
```

## 💡 Key Design Principles

1. **Normalize at boundaries** - Convert to 0-1 range early
2. **Timestamp everything** - Enables recording/playback
3. **Degrade gracefully** - Never block, always have fallback
4. **Pre-allocate resources** - No allocations in hot paths
5. **Batch similar operations** - Amortize overhead
6. **Make state transitions explicit** - Clear mode switches
7. **Visual feedback mirrors audio** - Tight correlation
8. **Progressive enhancement** - Start simple, add complexity

These blueprints represent **proven patterns** that balance:
- **Performance** vs **Quality**
- **Responsiveness** vs **Accuracy**  
- **Simplicity** vs **Features**
- **Safety** vs **Flexibility**
