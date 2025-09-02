# Sample Loading Pipeline Migration Guide

## Overview

The sample loading system has been completely rewritten to provide:
- Centralized async loading service
- Non-blocking UI feedback via toasts
- MP3 support with MediaFoundation
- Comprehensive diagnostics and error handling
- Unified drag-and-drop, button, and menu support

## New Build Options

### MP3 Support (Default: ON)
```bash
# Enable MP3 support (requires MediaFoundation on Windows)
cmake -DSPECTRAL_ENABLE_MP3=ON

# Disable MP3 support (WAV/AIFF/FLAC/OGG only)
cmake -DSPECTRAL_ENABLE_MP3=OFF
```

### Verbose Logging (Debug builds)
```bash
# Enable verbose diagnostics in Release builds
cmake -DENABLE_VERBOSE_LOGS=ON
```

## Supported Audio Formats

### Always Supported
- **WAV** - Uncompressed PCM, most reliable
- **AIFF** - Apple's uncompressed format
- **FLAC** - Lossless compression
- **OGG** - Ogg Vorbis compression

### Conditionally Supported
- **MP3** - Available when `SPECTRAL_ENABLE_MP3=ON` (default)
  - Windows: Requires MediaFoundation (enabled automatically)
  - macOS: Uses CoreAudio (built-in)
  - Linux: Uses system MP3 decoder if available

## API Changes

### AudioProcessor
```cpp
// NEW: Check if renderer is active (for HUD display)
bool hasActiveRenderer() const noexcept;

// NEW: Activate renderer with loaded sample data
bool activateSampleRenderer(const AudioBuffer<float>& sampleData, double sourceSampleRate);

// NEW: Deactivate renderer and reset state
void deactivateRenderer();
```

### Editor
```cpp
// NEW: Load audio file (for drag-and-drop, menu)
void loadAudioFile(const juce::File& file);

// NEW: Access to loading infrastructure
SampleLoaderService* getSampleLoader() const;
ToastManager* getToastManager() const;
```

## User Interface Changes

### Toast Notifications
- **Success**: Green toast with filename and duration
- **Error**: Red toast with specific error message
- **Auto-dismiss**: Toasts fade out automatically
- **Queue**: Multiple toasts can be shown simultaneously

### HUD Status
- **Before**: "GPU: Phase 4 Resilience Ready (No Active Renderer)"
- **After**: "GPU: 14.2ms HW" (when sample loaded and renderer active)

### Drag & Drop
- Drag audio files directly onto the canvas
- Visual feedback during drag operations
- Same error handling as button loading

## Error Messages

| Error | Message | Action |
|-------|---------|--------|
| File not found | "Couldn't open file. Check permissions or path." | Verify file exists and is readable |
| Unsupported format | "Unsupported format. Try WAV, AIFF, FLAC, OGG." | Use supported format or enable MP3 |
| Decode failure | "Decode failed. File may be corrupt..." | Re-export file or try different format |
| Renderer failure | "Sample loaded but renderer activation failed." | Check logs for GPU/DirectX issues |

## Diagnostic Logging

### Categories
- **LOAD**: File selection and loading operations
- **DEC**: Audio decoding and format detection  
- **REND**: Renderer activation and GPU operations
- **UI**: User interface events and feedback

### Example Log Output
```
[14:32:15.123] [UI  ] [INFO] SpectralCanvasProEditor.cpp:151 - Load Sample button clicked
[14:32:15.125] [LOAD] [INFO] SampleLoaderService.cpp:89 - Loading file: C:\samples\kick.wav (size: 441044 bytes)
[14:32:15.127] [DEC ] [INFO] SampleLoaderService.cpp:156 - Sample metadata: 44100 samples, 1 channels, 44.1kHz, 16-bit
[14:32:15.145] [DEC ] [INFO] SampleLoaderService.cpp:201 - Decode completed in 18ms: kick (1.00s, 44.1kHz)
[14:32:15.146] [REND] [INFO] SpectralCanvasProAudioProcessor.cpp:636 - Activating sample renderer: 1 channels, 44100 samples, 44.1kHz
[14:32:15.148] [REND] [INFO] SpectralCanvasProAudioProcessor.cpp:663 - Spectral player prepared: 173 frames, 513 bins
[14:32:15.149] [REND] [INFO] SpectralCanvasProAudioProcessor.cpp:670 - Renderer activation successful
```

## Testing

### Run Sample Loading Tests
```bash
# Build with tests enabled
cmake -B build -DBUILD_TESTING=ON -DSPECTRAL_ENABLE_MP3=ON

# Build project
cmake --build build

# Run sample loading tests specifically
cd build && ctest -R SampleLoading -V

# Run all tests
cd build && ctest -V
```

### Test Coverage
- ✅ Valid WAV/AIFF/FLAC loading
- ✅ MP3 loading (when enabled)
- ✅ Error handling for unsupported formats
- ✅ Error handling for non-existent files
- ✅ Renderer activation after successful load
- ✅ Thread safety of async callbacks

## Troubleshooting

### "No Active Renderer" persists
1. Check that sample loading succeeds (look for success toast)
2. Check logs for renderer activation errors
3. Verify GPU/DirectX drivers are working
4. Try a simple WAV file first

### MP3 files won't load on Windows
1. Verify `SPECTRAL_ENABLE_MP3=ON` in cmake command
2. Check that `JUCE_USE_WINDOWS_MEDIA_FOUNDATION=1` appears in build logs
3. Ensure Windows 10/11 with Media Feature Pack installed

### Slow loading performance
1. Check for VERBOSE logs in Release builds
2. Use smaller files for testing (<10MB recommended)
3. Monitor thread pool utilization in diagnostics

## Migration Steps

1. **Rebuild**: Clean build required due to new CMake options
2. **Test**: Verify sample loading works with your audio files
3. **Logs**: Check console output for diagnostic information
4. **Performance**: Monitor HUD for renderer activation status