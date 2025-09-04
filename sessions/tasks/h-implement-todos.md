---
task: h-implement-todos
branch: none
status: in-progress
created: 2025-09-03
modules: [Core, DSP, Audio]
---

# Implement Concrete TODOs from Codebase

## Problem/Goal
Address the specific TODO items found in the actual codebase to improve functionality and completeness. Focus on grounded implementation without hallucinations - only work on what actually exists.

## Success Criteria
- [x] ✅ Identify concrete TODOs from existing code
- [x] ✅ Fix STFT latency configuration in SpectralCanvasProAudioProcessor (line 725)
- [x] ✅ Wire respeed parameter to spectral player (line 756) 
- [x] ✅ Implement XRun detection infrastructure (line 1020)
- [x] ✅ Verify all changes build successfully
- [x] ✅ Ensure RT-safety compliance for all modifications

## Context Files
- @Source/SpectralCanvasProAudioProcessor.cpp:725  # STFT latency TODO
- @Source/SpectralCanvasProAudioProcessor.cpp:756  # Respeed parameter TODO  
- @Source/SpectralCanvasProAudioProcessor.cpp:1020 # XRun detection TODO
- @Source/DSP/SpectralEngine.h                     # FFT constants
- @docs/Developer-Notes.md                         # RT-safety rules
- @CMakeLists.txt                                  # Build configuration

## User Notes
- Focus on streamlined workflow with no AI hallucinations
- Only modify files that actually exist
- Follow existing patterns from implemented components
- Use actual constants and infrastructure already in place

## Context Manifest

### How RT DSP Audio Processing Currently Works: SpectralCanvasPro STFT Masking Pipeline

The SpectralCanvasPro audio processor implements a sophisticated real-time Short-Time Fourier Transform (STFT) masking system that transforms user paint gestures into immediate spectral modifications. The entire pipeline is designed around strict RT-safety requirements - no heap allocations, no locks, and no logging in processBlock().

When audio enters the system via processBlock(), it follows this critical path:

**Phase 4 STFT Masking (Primary RT Path)**: The main processing happens in lines 340-364 of SpectralCanvasProAudioProcessor.cpp. First, the system converts stereo input to mono processing using channel 0 as the reference, then zeros all output channels to prepare for mono-to-stereo replication. The HopScheduler drains up to 16 mask column deltas from the UI thread's MaskDeltaQueue (bounded to prevent RT violations), applying these paint-based mask values to the staging mask page via mask_.writeStagingColumn() and mask_.flip() operations.

The RealtimeSpectral engine processes each audio block using a sliding window approach: input samples accumulate in a hop-sized FIFO until a complete hop (128 samples) is available. Once ready, the engine assembles a complete FFT frame by shifting the previous (FFT_SIZE - HOP_SIZE) samples and appending the new hop. The forward FFT transforms this windowed time-domain data into frequency bins, where mask values are applied by scaling the complex bin magnitudes while preserving phase information. The modified spectrum undergoes inverse FFT and overlap-add reconstruction to produce the final audio output.

**Latency Architecture**: The system maintains strict latency consistency at exactly (FFT_SIZE - HOP_SIZE) = 384 samples across all audio paths. For the RT STFT path, this latency is inherent to the overlap-add reconstruction process. For non-STFT paths (TestFeeder, Phase4Synth, ModernPaint), the applyLatencyDelayIfNeeded() function applies a circular buffer delay using latencyLine_ to match the reported latency. This ensures seamless switching between audio paths without latency jumps that would cause audio artifacts.

**Thread Safety and RT Compliance**: The implementation uses atomic variables for all cross-thread communication (wroteAudioFlag_, currentPath_, latencySamples_), preallocated buffers sized during prepareToPlay(), and lock-free SPSC queues for UI-to-audio data flow. The JUCE ScopedNoDenormals wrapper prevents denormal float operations that could cause CPU spikes. Parameter smoothers handle control value changes without discontinuities. The system tracks performance metrics (totalBlocksProcessed_, totalSamplesProcessed_, xrunCount_) using atomic operations to avoid locks while providing diagnostic information.

**Memory Management**: All audio processing buffers are preallocated during prepareToPlay(): the RealtimeSpectral engine initializes FFT buffers, windowing tables, and FIFO structures; the latencyLine_ buffer is sized to exactly 384 samples; smoothing parameters are prepared with their target values. The HopScheduler's MaskAtlas uses double-buffered pages with atomic flips, ensuring the audio thread always reads from a stable mask while the UI thread can update the staging page.

**Error Handling and Diagnostics**: The system implements RT-safe XRun detection by monitoring for abnormally small buffer sizes (< 16 samples when expecting > 64). Early exit conditions handle zero-length buffers gracefully. Atomic performance counters provide debugging information without compromising RT safety. The Phase4DebugTap records queue addresses for integrity checking during development.

### Recent RT DSP Reactivation Implementation

The recent work reactivated the full audio processing pipeline by removing a debug early-return that was clearing buffers and exiting processBlock() prematurely. The main DSP logic block (lines 284-658) was uncommented, restoring the complete audio path selection and processing flow.

Key implementation details for the reactivation:

**applyLatencyDelayIfNeeded() Function**: This RT-safe function implements a circular buffer delay to align non-STFT audio paths with the reported 384-sample latency. It processes each channel independently, reading delayed samples from the circular buffer at latencyWritePos_, writing new input samples to that same position, and advancing the write position with modulo arithmetic. The function includes safety checks for buffer validity and channel count alignment.

**Latency Initialization**: The latencyLine_ buffer is initialized in prepareToPlay() with exactly (FFT_SIZE - HOP_SIZE) samples and cleared to prevent artifacts. The latencyWritePos_ atomic tracks the current write position for thread-safe access. The updateReportedLatency() function ensures consistent reporting via AudioProcessor::setLatencySamples() and atomic storage for cross-thread access.

**RT-Safety Validation**: The reactivated code maintains all RT-safety guarantees: no heap allocations in processBlock(), no mutex locks, no logging or I/O operations. All operations use preallocated buffers and atomic variables. The circular buffer implementation uses simple modulo arithmetic without dynamic memory allocation.

### Technical Reference Details

#### Core Constants (AtlasIds.h)
```cpp
static constexpr size_t FFT_SIZE = 512;
static constexpr size_t NUM_BINS = FFT_SIZE / 2 + 1;  // 257
static constexpr size_t HOP_SIZE = 128;
// Latency = FFT_SIZE - HOP_SIZE = 384 samples
```

#### Key Function Signatures

**Main Processing Entry Point**:
```cpp
void SpectralCanvasProAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, 
                                                   juce::MidiBuffer& midiMessages) noexcept
```

**RT-Safe Latency Compensation**:
```cpp
void SpectralCanvasProAudioProcessor::applyLatencyDelayIfNeeded(juce::AudioBuffer<float>& buffer) noexcept
```

**RealtimeSpectral Processing Core**:
```cpp
void RealtimeSpectral::processBlock(const float* in, float* out, int numSamples, const float* maskCol)
```

**HopScheduler Delta Processing**:
```cpp
inline void HopScheduler::drainAndApply(uint32_t maxDeltas = 16) noexcept
```

#### Data Structures and Buffer Management

**Latency Delay Line**:
```cpp
juce::AudioBuffer<float> latencyLine_;  // Size: NUM_CHANNELS x 384 samples
std::atomic<int> latencyWritePos_{0};   // Circular buffer write position
```

**RT Performance Tracking**:
```cpp
std::atomic<uint64_t> totalBlocksProcessed_{0};
std::atomic<uint64_t> totalSamplesProcessed_{0};
std::atomic<uint32_t> xrunCount_{0};
std::atomic<bool> wroteAudioFlag_{false};
```

**Audio Path Selection**:
```cpp
enum class AudioPath : uint8_t {
    Silent = 0,
    TestFeeder,       // Direct tone generation with latency compensation
    Phase4Synth,      // Spectral synthesis with latency compensation  
    ModernPaint,      // JUCE DSP spectral painting with latency compensation
    SpectralResynthesis  // File playback (no extra latency - handled by SpectralPlayer)
};
```

#### RT-Safety Constraints

**Forbidden in processBlock()**:
- Heap allocations (new, malloc, vector resize)
- Mutex locks or any blocking operations
- File I/O or network operations
- DBG() or console output
- Parameter tree lookups (APVTS access)
- Exception throwing

**Required Patterns**:
- Preallocate all buffers in prepareToPlay()
- Use atomic variables for cross-thread communication
- Wrap with juce::ScopedNoDenormals
- Bound queue operations (max 16 deltas per block)
- Use memory_order annotations for atomic operations

#### File Locations

**Core Implementation**:
- Main processor: `Source/SpectralCanvasProAudioProcessor.cpp` (lines 263-658)
- Header declarations: `Source/SpectralCanvasProAudioProcessor.h`
- STFT engine: `Source/DSP/RealtimeSpectral.h`
- Hop scheduling: `Source/DSP/HopScheduler.h`
- Atlas configuration: `Source/Core/AtlasIds.h`

**Related Components**:
- Message bus: `Source/Core/MessageBus.h` (MaskDeltaQueue)
- Mask atlas: `Source/Core/MaskAtlas.h` (double-buffered masks)
- Windowing compatibility: `Source/Compat/WindowingCompat.h`

## Work Log
- [2025-09-03] Started task, analyzed codebase for concrete TODOs
- [2025-09-03] Found 3 specific TODO items in audio processor
- [2025-09-03] Created task tracking and todo list
- [2025-09-03] Fixed STFT latency configuration: uncommented setLatencySamples(384) with proper constants
- [2025-09-03] Wired respeed parameter: added setRespeed() method to SpectralPlayer, implemented fractional frame advance
- [2025-09-03] Implemented XRun detection: added atomic counter and buffer size monitoring in processBlock()
- [2025-09-03] Fixed compilation errors: replaced canPush() with hasSpaceAvailable(), fixed syntax error in BottomBar.cpp
- [2025-09-03] Build successful: VST3 and Standalone targets built with only warnings (no errors)
- [2025-09-03] Task completed - all TODOs addressed with RT-safe implementations
- [2025-09-04] **RT DSP REACTIVATION**: Removed debug early-return from processBlock(), uncommented full DSP logic (lines 284-658), implemented RT-safe applyLatencyDelayIfNeeded() with circular buffer delay, initialized latencyLine_ buffer with 384 samples, ensured latency consistency throughout system