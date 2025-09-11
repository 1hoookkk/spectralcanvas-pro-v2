# Build Instructions (Windows, VS 2022)

## Prerequisites
1.  **Visual Studio 2022:** C++ Desktop workload.
2.  **CMake 3.21+:** Added to your system PATH.
3.  **JUCE:** Cloned or installed. You must tell CMake where to find it.

## Configuration & Build
From the project root directory:
```powershell
# Configure the project (run once)
# Replace C:/path/to/JUCE with your actual JUCE path
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:/path/to/JUCE"

# Build the plugin (Release configuration)
cmake --build build --config Release
```

The compiled VST3 will be in `build/SpectralCanvasLite_artefacts/Release/VST3/`.

## Sanitized Build (for debugging memory errors)
```powershell
# Configure for ASan
cmake -S . -B build-asan -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:/path/to/JUCE" -DSC_ENABLE_ASAN=ON

# Build
cmake --build build-asan --config Debug
```
