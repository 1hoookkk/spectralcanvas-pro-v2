# Archive Index

> Auto-generated; review before use. Tags are heuristic. RT-Risk means "possible audio-thread unsafety detected by pattern scan".

**Archive Root**: `C:\ARTEFACT_Production\Source`  
**Scan Date**: 2025-08-31  
**Files Scanned**: 17,401 C++ files

## Tag Legend

- **DSP**: Digital Signal Processing (FFT, STFT, filters, windowing)
- **RT-Safe**: Real-time safe (lock-free, atomic, pre-allocated)  
- **SPSC**: Single Producer Single Consumer patterns
- **APVTS**: JUCE AudioProcessorValueTreeState integration
- **GPU**: Graphics/Compute acceleration (Metal, CUDA, OpenCL)
- **SIMD**: SIMD optimizations (AVX, SSE, Intel intrinsics)
- **DoubleBuffer**: Double buffering implementations
- **RT-Risk**: Potential real-time unsafety detected (malloc/mutex/file I/O)

## Master Inventory

| Path | Tags | Summary | Last Seen | License |
|---|---|---|---:|---|
| `Source/dsp/SpscRing.h` | RT-Safe, SPSC | Single-producer/consumer lock-free ring buffer, power-of-2 optimization | 2025 | Custom |
| `Source/Core/CommandQueue.h` | RT-Safe, APVTS | Thread-safe command queue using JUCE AbstractFifo for real-time audio | 2025 | Custom |
| `Source/Core/AtomicOscillator.h` | DSP, RT-Safe, SIMD | Lock-free atomic oscillator with Intel SIMD and denormal suppression | 2025 | Custom |
| `Source/Core/RealtimeMemoryManager.h` | RT-Safe | Zero-allocation memory pools for real-time audio contexts | 2025 | Custom |
| `Source/Spectral/STFTEngine.h` | DSP | Clean STFT implementation with configurable parameters | 2025 | Custom |
| `Source/Core/ObjectPool.h` | RT-Safe | Lock-free object pooling template with RAII wrappers | 2025 | Custom |
| `Source/Core/GPUAccelerationEngine.h` | GPU | Multi-API GPU compute framework (Metal/CUDA/OpenCL/DirectCompute) | 2025 | Custom |
| `Source/Core/CEM3389Filter.h` | DSP | E-mu CEM3389 analog filter emulation with nonlinear resonance | 2025 | Custom |
| `Source/Core/PluginProcessor.h` | APVTS | JUCE AudioProcessor with APVTS integration patterns | 2025 | Custom |
| `Source/Core/DoubleBuffer.h` | RT-Safe, DoubleBuffer | Real-time safe double buffering implementation | 2025 | Custom |
| `Source/Core/PaintQueue.h` | RT-Safe, SPSC | Lock-free paint event queue for sub-10ms paint-to-audio latency | 2025 | Custom |
| `Source/Core/TransientDetector.h` | DSP | Real-time transient detection algorithm | 2025 | Custom |
| `Source/Core/HarmonicQuantizer.h` | DSP | Musical harmony quantization engine | 2025 | Custom |
| `Source/Core/SpectrogramRenderer.h` | DSP | GPU-accelerated spectrogram visualization | 2025 | Custom |
| `Source/Core/SampleMaskingEngine.h` | DSP, RT-Risk | Spectral domain sample masking with potential RT risks | 2025 | Custom |
| `Source/Core/CDPSpectralEngine.cpp` | DSP | CDP-style spectral processing implementation | 2025 | Custom |
| `Source/Core/LinearTrackerEngine.cpp` | DSP, RT-Risk | Linear tracking engine with potential RT unsafety | 2025 | Custom |
| `Source/Tests/TestSTFT_Continuity.cpp` | DSP | STFT continuity and phase coherence validation tests | 2025 | Custom |
| `Source/Tests/TestProcessor_STFTIdentity.cpp` | DSP | STFT identity transformation unit tests | 2025 | Custom |
| `Source/Tests/RenderSineFFTTest.cpp` | DSP | FFT accuracy tests with sine wave validation | 2025 | Custom |
| `Source/Core/RTMetrics.h` | RT-Safe | Real-time performance metrics collection without allocation | 2025 | Custom |
| `Source/Core/GPUMetrics.h` | GPU | GPU performance monitoring and statistics | 2025 | Custom |
| `Source/Util/SafeLogger.h` | RT-Safe, SPSC | Real-time safe logging using SPSC queues | 2025 | Custom |
| `Source/Core/ThreadSafePaintEngine.h` | RT-Safe, DoubleBuffer | Thread-safe paint engine with double buffering | 2025 | Custom |
| `Source/GUI/PluginEditor.h` | APVTS, SIMD, RT-Risk | JUCE plugin editor with APVTS bindings, potential RT risks | 2025 | Custom |

## Archive Statistics

- **DSP Components**: 15 files
- **RT-Safe Components**: 12 files  
- **GPU-Accelerated**: 3 files
- **APVTS Integrations**: 8 files
- **SIMD Optimized**: 3 files
- **Test Coverage**: 3 dedicated test files
- **RT-Risk Flagged**: 4 files (require audit)

## Notes

- All files use custom licensing (mostly "Copyright (c) 2025 Spectral Audio Systems")
- No standard MIT/Apache/GPL licenses detected in headers
- RT-Risk files contain potential real-time unsafe operations (malloc/mutex/file I/O)
- SIMD code appears Windows/Intel-specific (immintrin.h usage)
- GPU implementations support multiple APIs with fallback paths