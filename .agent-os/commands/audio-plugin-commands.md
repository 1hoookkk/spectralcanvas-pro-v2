# Audio Plugin Development Commands
## SpectralCanvas Pro - JUCE Development Workflow

**Focus Areas:** Build System, Testing, Performance Validation, Development Tools  
**Target Platform:** Windows with CMake + JUCE

---

## 🏗️ BUILD SYSTEM COMMANDS

### Standard Build Commands
```bash
# Development build with all tests
cmake -S . -B build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --config RelWithDebInfo --target ALL_BUILD

# Release build for distribution
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --config Release --target ALL_BUILD

# Debug build for development
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug --config Debug --target ALL_BUILD

# Clean build (remove all build artifacts)
rm -rf build build-release build-debug
```

### JUCE-Specific Build Commands
```bash
# Build VST3 plugin
cmake --build build --config RelWithDebInfo --target SpectralCanvasPro_VST3

# Build AU plugin (macOS only)
cmake --build build --config RelWithDebInfo --target SpectralCanvasPro_AU

# Build Standalone application
cmake --build build --config RelWithDebInfo --target SpectralCanvasPro_Standalone

# Build all plugin formats
cmake --build build --config RelWithDebInfo --target SpectralCanvasPro
```

### CMake Configuration Commands
```bash
# Configure with specific options
cmake -S . -B build \
    -DBUILD_TESTS=ON \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DJUCE_BUILD_EXAMPLES=OFF \
    -DJUCE_BUILD_EXTRAS=OFF

# Configure for specific plugin formats
cmake -S . -B build \
    -DJUCE_BUILD_VST3=ON \
    -DJUCE_BUILD_AU=OFF \
    -DJUCE_BUILD_STANDALONE=ON

# Configure with custom JUCE path
cmake -S . -B build \
    -DJUCE_DIR=/path/to/juce
```

---

## 🧪 TESTING COMMANDS

### Unit Testing
```bash
# Run all tests
ctest --test-dir build -C RelWithDebInfo --output-on-failure

# Run specific test categories
ctest --test-dir build -C RelWithDebInfo --output-on-failure -R "RTSafety"
ctest --test-dir build -C RelWithDebInfo --output-on-failure -R "Performance"
ctest --test-dir build -C RelWithDebInfo --output-on-failure -R "Unit"

# Run tests with verbose output
ctest --test-dir build -C RelWithDebInfo --output-on-failure --verbose

# Run tests in parallel
ctest --test-dir build -C RelWithDebInfo --output-on-failure -j4
```

### Real-Time Safety Testing
```bash
# Run RT-safety validation
./build/SpectralCanvasProTests --gtest_filter="RTSafety*"

# Run performance benchmarks
./build/SpectralCanvasProTests --gtest_filter="Performance*"

# Run specific RT-safety test
./build/SpectralCanvasProTests --gtest_filter="RTSafetyTest.expectNoDynamicAllocations"
```

### Continuous Testing
```bash
# Run TDD guard (continuous testing)
./tdd-guard.bat

# Quick validation script
./tools/test/phase7-quick-validation.bat

# Performance stress test
./tools/test/performance-stress-test.bat
```

---

## 🎵 AUDIO PLUGIN TESTING

### Plugin Validation
```bash
# Test VST3 plugin with validator
vst3validator /path/to/SpectralCanvasPro.vst3

# Test AU plugin with validator (macOS)
auval -v aumu Spcp YourC SpectralCanvasPro

# Test plugin in DAW (manual testing)
# Load plugin in Reaper, Logic, Ableton, etc.
# Test all parameters, automation, preset loading
```

### Performance Testing
```bash
# CPU usage test
./tools/test/cpu-usage-test.bat

# Memory usage test
./tools/test/memory-usage-test.bat

# Latency test
./tools/test/latency-test.bat

# Polyphony test
./tools/test/polyphony-test.bat
```

### Audio Quality Testing
```bash
# Frequency response test
./tools/test/frequency-response-test.bat

# Distortion test
./tools/test/distortion-test.bat

# Noise floor test
./tools/test/noise-floor-test.bat

# Phase response test
./tools/test/phase-response-test.bat
```

---

## 🔧 DEVELOPMENT TOOLS

### Debugging Commands
```bash
# Enable debug logging
export JUCE_LOG_LEVEL=DEBUG

# Run with debugger
gdb --args ./build/SpectralCanvasPro_Standalone

# Run with Valgrind (memory checking)
valgrind --tool=memcheck --leak-check=full ./build/SpectralCanvasPro_Standalone

# Run with AddressSanitizer
ASAN_OPTIONS=detect_leaks=1 ./build/SpectralCanvasPro_Standalone
```

### Profiling Commands
```bash
# CPU profiling with gprof
cmake -S . -B build-profile -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_FLAGS="-pg"
cmake --build build-profile --config RelWithDebInfo
./build-profile/SpectralCanvasPro_Standalone
gprof ./build-profile/SpectralCanvasPro_Standalone gmon.out > profile.txt

# CPU profiling with perf
perf record ./build/SpectralCanvasPro_Standalone
perf report

# Memory profiling with massif
valgrind --tool=massif ./build/SpectralCanvasPro_Standalone
ms_print massif.out.* > memory_profile.txt
```

### Code Analysis
```bash
# Static analysis with clang-tidy
cmake -S . -B build-analyze -DCMAKE_BUILD_TYPE=Debug
cmake --build build-analyze --config Debug
clang-tidy -p build-analyze Source/**/*.cpp

# Code coverage
cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build build-coverage --config Debug
ctest --test-dir build-coverage -C Debug
lcov --capture --directory build-coverage --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
```

---

## 📦 PACKAGING & DISTRIBUTION

### Plugin Packaging
```bash
# Create VST3 package
mkdir -p SpectralCanvasPro_VST3
cp build/SpectralCanvasPro_artefacts/RelWithDebInfo/VST3/SpectralCanvasPro.vst3 SpectralCanvasPro_VST3/
cp docs/README.md SpectralCanvasPro_VST3/
cp docs/CHANGELOG.md SpectralCanvasPro_VST3/
zip -r SpectralCanvasPro_VST3_Windows.zip SpectralCanvasPro_VST3/

# Create Standalone package
mkdir -p SpectralCanvasPro_Standalone
cp build/SpectralCanvasPro_artefacts/RelWithDebInfo/Standalone/SpectralCanvasPro.exe SpectralCanvasPro_Standalone/
cp -r resources/ SpectralCanvasPro_Standalone/
zip -r SpectralCanvasPro_Standalone_Windows.zip SpectralCanvasPro_Standalone/
```

### Installer Creation
```bash
# Create NSIS installer
makensis installer/SpectralCanvasPro.nsi

# Create Inno Setup installer
iscc installer/SpectralCanvasPro.iss

# Create MSI installer
wix build installer/SpectralCanvasPro.wxs
```

---

## 🔍 EMULATOR X3 ANALYSIS COMMANDS

### Archive Analysis
```bash
# Analyze Emulator X3 preset files
./tools/analysis/analyze-presets.bat /path/to/emulator-x3/presets

# Extract parameter data
./tools/analysis/extract-parameters.bat /path/to/emulator-x3

# Generate behavioral documentation
./tools/analysis/generate-behavioral-docs.bat /path/to/emulator-x3

# Validate clean-room compliance
./tools/analysis/validate-clean-room.bat
```

### Behavioral Modeling
```bash
# Generate filter behavior models
./tools/modeling/generate-filter-models.bat

# Create arpeggiator pattern analysis
./tools/modeling/analyze-arpeggiator-patterns.bat

# Generate modulation matrix documentation
./tools/modeling/document-modulation-matrix.bat

# Create sample library compatibility analysis
./tools/modeling/analyze-sample-compatibility.bat
```

---

## 🚀 DEPLOYMENT COMMANDS

### Release Preparation
```bash
# Update version numbers
./tools/release/update-version.bat 1.0.0

# Generate changelog
./tools/release/generate-changelog.bat

# Create release notes
./tools/release/create-release-notes.bat

# Validate release build
./tools/release/validate-release.bat
```

### Distribution
```bash
# Upload to plugin stores
./tools/distribution/upload-vst-store.bat
./tools/distribution/upload-au-store.bat

# Create GitHub release
gh release create v1.0.0 SpectralCanvasPro_VST3_Windows.zip --title "SpectralCanvas Pro v1.0.0" --notes "Initial release with EMU Audity 2000 hybrid engine"

# Deploy to website
./tools/distribution/deploy-website.bat
```

---

## 🛠️ MAINTENANCE COMMANDS

### Code Quality
```bash
# Format code with clang-format
find Source/ -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# Check code formatting
find Source/ -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run --Werror

# Run linter
./tools/quality/run-linter.bat

# Check for TODOs and FIXMEs
grep -r "TODO\|FIXME" Source/ --include="*.cpp" --include="*.h"
```

### Documentation
```bash
# Generate API documentation
doxygen Doxyfile

# Update project documentation
./tools/docs/update-project-docs.bat

# Generate technical specifications
./tools/docs/generate-tech-specs.bat

# Create user manual
./tools/docs/create-user-manual.bat
```

### Dependencies
```bash
# Update JUCE
git submodule update --remote JUCE

# Update other dependencies
./tools/deps/update-dependencies.bat

# Check for security vulnerabilities
./tools/deps/security-scan.bat

# Validate dependency licenses
./tools/deps/validate-licenses.bat
```

---

## 🆘 EMERGENCY COMMANDS

### Crash Recovery
```bash
# Emergency test tone toggle (in plugin editor)
# Press 'T' key → toggles emergency test tone for audio debugging

# Force safe mode
export SPECTRALCANVAS_SAFE_MODE=1

# Disable problematic features
export DISABLE_Z_PLANE_FILTERS=1
export DISABLE_ARPEggiATOR=1
export DISABLE_PAINT_GESTURES=1

# Reset to factory settings
./tools/emergency/reset-to-factory.bat
```

### Performance Emergency
```bash
# Force low CPU mode
export SPECTRALCANVAS_LOW_CPU_MODE=1

# Disable real-time processing
export SPECTRALCANVAS_OFFLINE_MODE=1

# Force single-threaded processing
export SPECTRALCANVAS_SINGLE_THREADED=1

# Emergency performance monitoring
./tools/emergency/performance-monitor.bat
```

---

## 📊 MONITORING COMMANDS

### Performance Monitoring
```bash
# Monitor CPU usage
./tools/monitoring/monitor-cpu.bat

# Monitor memory usage
./tools/monitoring/monitor-memory.bat

# Monitor audio latency
./tools/monitoring/monitor-latency.bat

# Monitor plugin stability
./tools/monitoring/monitor-stability.bat
```

### Log Analysis
```bash
# Analyze debug logs
./tools/logs/analyze-debug-logs.bat

# Search for errors
./tools/logs/search-errors.bat

# Generate performance report
./tools/logs/generate-performance-report.bat

# Monitor real-time logs
./tools/logs/monitor-logs.bat
```

---

**Document Status:** ACTIVE - Development Ready  
**Last Updated:** August 26, 2025  
**Next Review:** Phase 1 Milestone Completion
