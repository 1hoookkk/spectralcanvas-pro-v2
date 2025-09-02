# Migration Guide: Build System & API Updates

## Build System Improvements

### Faster Builds
- **Problem**: Building ALL_BUILD target compiles 62+ projects (5+ minutes)
- **Solution**: Build specific targets only
```bash
# Fast VST3 build (recommended for development)
cmake --build build --config Debug --target SpectralCanvasPro_VST3

# Fast Standalone build  
cmake --build build --config Debug --target SpectralCanvasPro_Standalone
```

### Test Build Control
- **New**: `ENABLE_TESTS` CMake option (default OFF)
- **Usage**: 
```bash
# Development builds (no tests, faster)
cmake -B build -DENABLE_TESTS=OFF

# Full builds with tests
cmake -B build -DENABLE_TESTS=ON
```
- **Impact**: Eliminates GTest dependency and test compilation for normal development

## API Changes

### Time Utils Migration
- **New Header**: `Source/Core/TimeUtils.h`
- **Replaces**: Direct `getCurrentTimeUs()` calls on processor
```cpp
// OLD (undefined reference)
auto time = processor.getCurrentTimeUs();

// NEW (use utility header)
#include "Core/TimeUtils.h"
auto time = TimeUtils::getCurrentTimeUs();
```

### Platform-Specific Time Functions
- **New Header**: `Source/Core/PlatformTimeUtils.h` 
- **Replaces**: Direct `localtime()` calls (deprecated on Windows)
```cpp
// OLD (CRT warning on Windows)
std::tm* tm = std::localtime(&timer);

// NEW (cross-platform safe)
#include "Core/PlatformTimeUtils.h"
std::tm tm;
if (PlatformTimeUtils::getLocalTime(&tm, &timer)) {
    // use tm safely
}
```

### AudioFormat Writer API
- **Change**: JUCE updated writer method names
```cpp
// OLD (deprecated)
writer->writeFromAudioBuffer(buffer, startSample, numSamples);

// NEW (current JUCE 8.x)
writer->writeFromAudioSampleBuffer(buffer, startSample, numSamples);
```

### Font Constructor Updates  
- **Preferred**: FontOptions-based construction
```cpp
// OLD (deprecated but works)
juce::Font font(name, size, juce::Font::plain);

// NEW (recommended)
juce::Font font{juce::FontOptions().withHeight(size).withTypefaceStyle("Regular")};
```

### Plugin Name Macros
- **Issue**: `JucePlugin_Name` only available in plugin targets
- **Solution**: Conditional usage
```cpp
#ifdef JucePlugin_Name
    auto name = JucePlugin_Name;
#else
    auto name = ProjectInfo::projectName;
#endif
```

## Type Safety Improvements

### Conversion Warnings Fixed
All implicit narrowing conversions addressed:
- `double` → `float`: Explicit casts added
- `double` → `uint64_t`: Proper rounding with `std::llround()`  
- `size_t` → `int`: Safe clamping for JUCE APIs

### Unused Parameter Cleanup
- Added `juce::ignoreUnused()` for legitimate unused parameters
- Removed dead code where appropriate

## Build Performance Results

**Before**: 5+ minutes (ALL_BUILD, 62 projects)
**After**: ~30 seconds (single target, tests optional)

**Commands for fast development**:
```bash
# Quick iteration cycle
cmake -B build -DENABLE_TESTS=OFF
cmake --build build --target SpectralCanvasPro_VST3

# Full validation (when needed)  
cmake -B build -DENABLE_TESTS=ON
cmake --build build
ctest --test-dir build
```