# SpectralCanvasLite - Session Handoff Document
*Generated: 2025-09-11*

## Current Status: Phase 4 Complete ✅

**Achievement**: RT-safe audio plugin infrastructure is fully working with comprehensive UI, telemetry, and paint canvas. Paint events are successfully processed (1794+ events seen in telemetry).

**Key Success**: Clean build system with zero warnings, working VST3/Standalone builds, and real paint→audio ducking effect.

**Critical Issue Identified**: Current ducking effect is too simple/lame for real use. Next phase needs to implement actual spectral processing.

---

## Architecture Status

### ✅ **Completed Infrastructure**

**Clean Source Organization**:
- `Source/Core/` - Audio processor and data structures
- `Source/DSP/` - Signal processing modules (ready for expansion)
- `Source/UI/` - Editor and paint canvas
- `Source/Utils/` - RT-safe communication utilities

**RT-Safe Communication Pipeline**:
- Lock-free SPSC queue working perfectly (`SpscQueueLite.h`)
- `MaskColumn` data structure refined with field-based design
- Paint events from UI thread → Audio thread with bounded processing (≤16/block)
- Zero allocations in `processBlock()` confirmed

**Working Plugin Infrastructure**:
- JUCE 8.0.8 based VST3 + Standalone builds
- Clean CMake system with feature flags
- Comprehensive telemetry system with atomic counters
- 30fps UI updates with performance monitoring

**Paint Canvas Working**:
- Mouse painting with real-time visual feedback
- Paint intensity, position, and timing captured correctly
- 1794+ paint events successfully processed (confirmed in telemetry)
- Brush visualization with dynamic sizing and trails

### 🔄 **Ready for Integration**

**PsychoacousticMaskingEngine**:
- Already ported from SpectralCanvasPro research
- Located at `Source/DSP/PsychoacousticMaskingEngine.h`
- RT-safe with locked mathematical formulas
- Feature flag available: `SC_ENABLE_PSYCHO=OFF` (ready to enable)
- Implements Bark/ERB band analysis with JND clamping

**JUCE DSP Integration Ready**:
- JUCE audio_utils already linked
- STFT/FFT classes available for spectral analysis
- Need to add `juce_dsp` module for FFT pipeline

---

## Current Implementation Details

### **Audio Processing Chain** (`SpectralCanvasProProcessor.cpp`)
```cpp
// Current Phase 4 (Simple Ducking)
processBlock() {
    processPaintEvents();     // Drain ≤16 events from UI
    updatePaintGain();        // Smooth gain transitions
    buffer.applyGain(gain);   // Apply simple ducking
    updateTelemetry();        // Track performance
}
```

**Paint Event Processing**:
- Bounded work: Maximum 16 events per audio block
- Intensity mapping: `targetGain = 1.0f - (intensity * 0.95f)`
- Attack/release smoothing with different coefficients
- Queue depth monitoring and rejection counting

### **MaskColumn Interface** (`MaskColumn.h`)
```cpp
struct MaskColumn {
    float intensity;        // [0.0, 1.0] paint strength
    int32_t columnIndex;    // Time/column position
    int32_t startBin;       // Frequency range start
    int32_t endBin;         // Frequency range end
    uint64_t timestamp;     // For latency tracking
    uint32_t sequenceNumber; // Debug sequence
};
```

### **Telemetry System** (`Snapshot.h`)
Comprehensive performance monitoring with atomic counters:
- Processing time per block (microseconds)
- Queue depth and drain counts
- Paint event totals and rejections
- RT-safety verification (zero allocations confirmed)

---

## Next Session Implementation Plan

### **Priority 1: Replace Simple Ducking with Real Spectral Processing**

**1. Enable JUCE DSP FFT Pipeline**
```cmake
# Add to CMakeLists.txt
target_link_libraries(SpectralCanvasLite PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp)  # Add this for FFT
```

**2. Implement STFT Processing in Audio Thread**
```cpp
// Replace simple ducking with:
processBlock() {
    // Forward FFT
    stft.performFFT(buffer);
    
    // Process paint events → frequency masks
    processPaintEvents();  // Convert to spectral masks
    
    // Apply masks to frequency bins
    applySpectralMasks(fftData);
    
    // Inverse FFT
    stft.performIFFT(buffer);
}
```

**3. Integrate PsychoacousticMaskingEngine**
```bash
# Enable the psychoacoustic engine
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSC_ENABLE_PSYCHO=ON
```

### **Priority 2: Transform Paint Coordinates to Frequency Domain**

**Current Issue**: Paint Y-coordinates are arbitrary. Need proper frequency mapping.

**Solution**: 
- Map canvas Y-position to Bark/ERB frequency bands
- Use `PsychoacousticMaskingEngine` band structure
- Transform `MaskColumn.startBin/endBin` to actual FFT bins

**Implementation**:
```cpp
// In SpectralCanvasProEditor.cpp
MaskColumn createMaskColumnFromMouse(MouseEvent& event) {
    float normalizedY = 1.0f - (relativePos.y / canvasHeight); // Invert Y
    
    // Map to psychoacoustic bands (24 Bark bands default)
    int bandIndex = static_cast<int>(normalizedY * 24);
    
    // Convert band to FFT bin range
    auto [startBin, endBin] = getBinRangeForBand(bandIndex);
    
    column.startBin = startBin;
    column.endBin = endBin;
    // ...
}
```

### **Priority 3: Implement Intelligent Spectral Manipulation**

**Beyond Simple Ducking**:
- Psychoacoustic masking-aware attenuation
- Frequency-selective processing based on paint position
- Preserve important spectral content while following paint gestures

**Algorithm**:
```cpp
void applySpectralMasks(std::span<float> fftMagnitudes) {
    // 1. Compute psychoacoustic masking thresholds
    psychoEngine.computeThreshold(fftMagnitudes, maskingThresholds);
    
    // 2. Apply paint-derived gains with JND clamping
    psychoEngine.jndClampBins(fftMagnitudes, maskingThresholds, paintGains);
    
    // 3. Intelligently attenuate while preserving perceptual content
    for (int bin = 0; bin < numBins; ++bin) {
        fftMagnitudes[bin] *= paintGains[bin];
    }
}
```

---

## Critical Files for Next Session

### **Primary Implementation Files**
- **`SpectralCanvasProProcessor.cpp`** - Replace `processBlock()` ducking with STFT
- **`SpectralCanvasProEditor.cpp`** - Fix paint coordinate→frequency mapping
- **`CMakeLists.txt`** - Enable `juce_dsp` and `SC_ENABLE_PSYCHO=ON`

### **Ready-to-Use Resources**
- **`PsychoacousticMaskingEngine.h`** - Complete psychoacoustic processor
- **`MaskColumn.h`** - Data structure (may need frequency mapping refinement)
- **`SpscQueueLite.h`** - RT-safe communication (working perfectly)

### **Integration Points**
- JUCE DSP FFT classes: `juce::dsp::FFT`, `juce::dsp::WindowingFunction`
- STFT overlap-add processing for real-time spectral manipulation
- Convert paint events to frequency-domain masks in real-time

---

## Build Status

**Current Configuration**:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --config RelWithDebInfo
```

**Zero Warnings Achieved**: Strict compiler settings with warnings-as-errors enabled

**Feature Flags Available**:
- `SC_ENABLE_PSYCHO=OFF` → Set to `ON` for next phase
- `SC_ENABLE_STUB=OFF` → Keep OFF (not needed for spectral path)

**Validated**:
- VST3 loads correctly in DAWs
- Standalone runs without issues
- RT-safety confirmed (no allocations in audio thread)
- Paint events flow correctly from UI to audio

---

## Measurement & Validation

**Performance Confirmed**:
- Processing time: <100μs per block typical
- Queue management: 0 rejected pushes under normal use
- Paint latency: Immediate visual feedback, audio response within 1-2 blocks
- Memory: Zero allocations during `processBlock()` processing

**Next Session Should Measure**:
- STFT processing overhead
- Psychoacoustic engine CPU usage
- Spectral processing quality (before/after analysis)
- Real-time performance with full spectral pipeline

---

## Success Criteria for Next Phase

### **Minimum Viable Spectral Processing**
1. ✅ Replace simple ducking with STFT-based processing
2. ✅ Enable and integrate `PsychoacousticMaskingEngine`
3. ✅ Paint gestures affect actual frequency content
4. ✅ Maintain RT-safety (no audio dropouts)
5. ✅ Keep telemetry working for performance monitoring

### **Quality Targets**
- Spectral processing sounds "intelligent" (not just ducking)
- Paint position clearly corresponds to frequency content changes
- Performance remains under 5ms processing per block
- No artifacts from STFT overlap-add processing

### **Validation Steps**
1. Load VST3 in DAW with complex audio content
2. Paint gestures create clearly audible spectral changes
3. High frequencies affected by top painting, low frequencies by bottom
4. Psychoacoustic engine preserves important content while following paint
5. Telemetry shows reasonable processing times with new pipeline

---

## Getting Started Commands

```bash
# Configure with psychoacoustic engine enabled
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSC_ENABLE_PSYCHO=ON

# Build
cmake --build build --config RelWithDebInfo

# Test VST3
# Load: build/SpectralCanvasLite_artefacts/RelWithDebInfo/VST3/SpectralCanvasLite.vst3

# Key files to start editing:
vim Source/Core/SpectralCanvasProProcessor.cpp  # Replace processBlock()
vim Source/UI/SpectralCanvasProEditor.cpp        # Fix frequency mapping
vim CMakeLists.txt                               # Add juce_dsp
```

**This foundation is solid. Time to make it sound amazing.**