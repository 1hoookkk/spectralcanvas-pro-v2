# Phase 2-3 Validation Test Procedures

## Setup Instructions

### 1. Launch Application
- **Standalone**: `C:\spectralCanvas Pro\build\SpectralCanvasPro_artefacts\RelWithDebInfo\Standalone\spectralCanvas Pro.exe`
- **VST3**: Load `C:\spectralCanvas Pro\build\SpectralCanvasPro_artefacts\RelWithDebInfo\VST3\spectralCanvas Pro.vst3` in DAW

### 2. Enable Debug Overlay
1. Click the **Debug** toggle in the minimal top strip
2. Verify debug overlay shows validation metrics:
   - "Phase 2-3 Validation Metrics" header
   - Latency display (med/p95 ms)  
   - Queue stats (depth/drops)
   - FPS and sample processing info

### 3. Enable Test Mode
1. Click the **START** button to activate test mode
2. Status should show "Test Mode: ACTIVE"
3. Paint gestures will now generate MaskColumn messages for latency measurement

## Test Execution Procedures

### Test 1: Short Taps (Baseline Latency)
**Duration**: 30 seconds  
**Actions**:
1. Single click on canvas at 1-2 Hz (once or twice per second)
2. Click in different areas of the canvas
3. Observe immediate audio feedback (test tone)
4. Monitor debug overlay for latency metrics

**Expected Results**:
- Median latency ≤3ms
- Consistent audio response to each click
- Queue drops = 0
- FPS stable around 60

### Test 2: Long Strokes (Sustained Performance)  
**Duration**: 30 seconds
**Actions**:
1. Hold mouse down and drag continuously for 2-5 seconds
2. Release, pause 1-2 seconds, repeat
3. Vary stroke direction and speed
4. Monitor continuous audio feedback during drags

**Expected Results**:
- Median latency ≤4ms
- Smooth audio during drag operations
- Queue drops = 0 or minimal
- FPS remains stable

### Test 3: Dense Scribble (Stress Test)
**Duration**: 30 seconds
**Actions**:
1. Rapid mouse movement with button held down
2. Create dense scribbling patterns
3. Aim for 15-20 paint events per second
4. Monitor queue depth and drops in real-time

**Expected Results**:
- Median latency ≤5ms  
- Queue drops <5% acceptable under stress
- Audio may become dense but should remain responsive
- FPS may drop slightly but should recover

### Test 4: Standard Use Simulation
**Duration**: 60 seconds
**Actions**:
1. Mix of clicks, drags, and pauses
2. Simulate realistic usage patterns
3. Vary interaction intensity
4. Include idle periods (no interaction)

**Expected Results**:
- Median latency ≤4ms
- Consistent performance across interaction types
- Queue depth returns to 0 during idle
- FPS stable throughout test

### Test 5: Soak Test (Stability)
**Duration**: 5+ minutes
**Actions**:
1. Continuous moderate painting activity
2. Mix of strokes and taps every 2-3 seconds
3. Monitor for performance degradation
4. Watch for memory leaks or instability

**Expected Results**:
- Latency remains stable (no drift)
- No performance degradation over time
- Queue drops remain minimal
- Application stability maintained

## Data Collection

### During Each Test
1. **Start Values**: Record initial latency and FPS
2. **Peak Values**: Note maximum latency observed
3. **End Values**: Record final metrics after test completion
4. **Drops**: Count total queue drops during test
5. **XRuns**: Note any audio dropouts or glitches

### Screenshots Required
1. **Debug overlay active** showing all metrics
2. **Peak latency** moment during stress testing
3. **Queue status** during dense scribble test
4. **Soak test** final results after 5+ minutes

## Configuration Testing

### Sample Rate Testing
Test at multiple sample rates if available:
- 44.1 kHz (standard)
- 48 kHz (professional)  
- 96 kHz (high-resolution)

### Block Size Testing  
Test different audio buffer sizes:
- 64 samples (low latency)
- 128 samples (balanced)
- 256 samples (stable)
- 512 samples (high stability)
- 1024 samples (maximum stability)

## Pass/Fail Criteria

### ✅ PASS Criteria
- Median latency ≤5ms across all configurations
- P95 latency ≤10ms (perceptual threshold)
- FPS sustained 58-62 fps during normal use
- Queue drops = 0 during normal tests
- Queue drops <5% during stress tests only
- Zero audio dropouts (XRuns) detected
- 5+ minute soak test completes without issues

### ❌ FAIL Criteria  
- Median latency >5ms consistently
- P95 latency >10ms (audible delay)
- FPS drops below 55fps regularly
- Queue drops >5% during normal use
- Audio dropouts or glitches detected
- Application crashes or hangs
- Performance degrades during soak test

## Troubleshooting

### High Latency Issues
- Check system audio buffer settings
- Verify exclusive audio device access
- Test with different sample rates/block sizes
- Monitor CPU usage during testing

### Queue Drop Issues  
- Reduce interaction intensity
- Check for UI thread blocking operations
- Verify queue capacity (should be 8 slots)
- Monitor queue depth trends

### FPS Issues
- Check GPU/display driver
- Verify VSync settings
- Monitor debug overlay rendering cost
- Test with overlay disabled

---

**Remember**: Phase 2-3 validation focuses on RT-safety and performance fundamentals. Visual polish and advanced features are out of scope for this validation phase.