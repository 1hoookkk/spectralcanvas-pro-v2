# RT Safety Test Suite for SpectralCanvas Pro
# Note: This file is included from the main CMakeLists.txt via add_subdirectory()
# JUCE is already available from the parent scope

# Test executable: RT Safety validation
add_executable(RTSafetyTest 
    RTSafetyTest.cpp
    TestHarness.cpp
    AllocationDetector.cpp
)

# Test executable: Spectral Paint Node validation
add_executable(SpectralPaintNodeTest
    SpectralPaintNodeTest.cpp
    ../Source/nodes/SpectralPaintNode.cpp
)

# Test executable: Sample Loading Pipeline validation
add_executable(SampleLoadTest
    SampleLoadTest.cpp
    ../Source/Core/SampleLoaderService.cpp
    ../Source/Core/DiagnosticLogger.cpp
    ../Source/SpectralCanvasProAudioProcessor.cpp
    ../Source/Audio/SampleManager.cpp
    ../Source/DSP/SpectralModel.cpp
    ../Source/DSP/SpectralPlayer.cpp
    ../Source/DSP/SampleLoader.cpp
    ../Source/Core/ColorToSpectralMapper.cpp
    ../Source/Core/AtomicOscillator.cpp
)

target_compile_features(RTSafetyTest PRIVATE cxx_std_20)
target_compile_features(SpectralPaintNodeTest PRIVATE cxx_std_20)
target_compile_features(SampleLoadTest PRIVATE cxx_std_20)

target_link_libraries(RTSafetyTest PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

target_link_libraries(SpectralPaintNodeTest PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    gtest
    gtest_main
)

target_link_libraries(SampleLoadTest PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_formats
    juce::juce_dsp
    juce::juce_gui_basics
)

# Test executable: ProcessBlock validation  
add_executable(ProcessBlockTest
    ProcessBlockTest.cpp
    TestHarness.cpp
    AllocationDetector.cpp
)

target_compile_features(ProcessBlockTest PRIVATE cxx_std_20)

target_link_libraries(ProcessBlockTest PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

# Test executable: Queue validation
add_executable(QueueTest
    QueueTest.cpp
    TestHarness.cpp
    AllocationDetector.cpp
)

target_compile_features(QueueTest PRIVATE cxx_std_20)

target_link_libraries(QueueTest PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

# Test executable: Performance harness
add_executable(PerfHarness
    PerfHarness.cpp
    TestHarness.cpp
    AllocationDetector.cpp
)

target_compile_features(PerfHarness PRIVATE cxx_std_20)

target_link_libraries(PerfHarness PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

# Test executable: GPU Resilience validation
add_executable(GPUResilienceTest
    GPUResilienceTest.cpp
    TestHarness.cpp
)

# Test executable: Windowing FFT Benchmark
add_executable(BenchWindowFFT
    BenchWindowFft.cpp
)

target_compile_features(GPUResilienceTest PRIVATE cxx_std_20)
target_compile_features(BenchWindowFFT PRIVATE cxx_std_20)

target_link_libraries(GPUResilienceTest PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

target_link_libraries(BenchWindowFFT PRIVATE
    juce::juce_core
    juce::juce_dsp
)

# Windows-specific D3D11 libraries for GPU tests
if(WIN32)
    target_link_libraries(GPUResilienceTest PRIVATE
        d3d11.lib
        dxgi.lib
        d3dcompiler.lib
    )
endif()

# Include source directories for testing
target_include_directories(RTSafetyTest PRIVATE "../Source")
target_include_directories(SpectralPaintNodeTest PRIVATE "../Source")
target_include_directories(SampleLoadTest PRIVATE "../Source")
target_include_directories(ProcessBlockTest PRIVATE "../Source")  
target_include_directories(QueueTest PRIVATE "../Source")
target_include_directories(PerfHarness PRIVATE "../Source")
target_include_directories(GPUResilienceTest PRIVATE "../Source")
target_include_directories(BenchWindowFFT PRIVATE "../Source")

# Compiler flags for testing
if(MSVC)
    target_compile_options(RTSafetyTest PRIVATE /W4)
    target_compile_options(SpectralPaintNodeTest PRIVATE /W4)
    target_compile_options(SampleLoadTest PRIVATE /W4)
    target_compile_options(ProcessBlockTest PRIVATE /W4)
    target_compile_options(QueueTest PRIVATE /W4)
    target_compile_options(PerfHarness PRIVATE /W4)
    target_compile_options(GPUResilienceTest PRIVATE /W4)
else()
    target_compile_options(RTSafetyTest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(SpectralPaintNodeTest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(SampleLoadTest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(ProcessBlockTest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(QueueTest PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(PerfHarness PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(GPUResilienceTest PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Define tests for CTest
add_test(NAME RTSafety COMMAND RTSafetyTest)
add_test(NAME SampleLoading COMMAND SampleLoadTest)
add_test(NAME ProcessBlockValidation COMMAND ProcessBlockTest --latency-target 5.0 --cpu-target 15.0)
add_test(NAME QueueValidation COMMAND QueueTest)
add_test(NAME PerformanceBaseline COMMAND PerfHarness --baseline)
add_test(NAME PerformanceJSON COMMAND PerfHarness --baseline --emit-json ${CMAKE_CURRENT_BINARY_DIR}/perf_metrics.json)
add_test(NAME GPUResilience COMMAND GPUResilienceTest)

# Set test properties
set_tests_properties(RTSafety PROPERTIES
    TIMEOUT 30
    LABELS "rt-safety;validation"
)

set_tests_properties(SampleLoading PROPERTIES
    TIMEOUT 30
    LABELS "sample-loading;validation"
)

set_tests_properties(ProcessBlockValidation PROPERTIES  
    TIMEOUT 60
    LABELS "performance;rt-safety"
)

set_tests_properties(QueueValidation PROPERTIES
    TIMEOUT 30
    LABELS "concurrency;rt-safety"
)

set_tests_properties(PerformanceBaseline PROPERTIES
    TIMEOUT 120
    LABELS "performance;baseline"
)

set_tests_properties(PerformanceJSON PROPERTIES
    TIMEOUT 120
    LABELS "performance;export;json"
)

set_tests_properties(GPUResilience PROPERTIES
    TIMEOUT 60
    LABELS "gpu;resilience;d3d11"
)

# RT safety compile-time option
option(ENABLE_RT_SAFETY_CHECKS "Enable RT safety checking" ON)
if(ENABLE_RT_SAFETY_CHECKS)
    target_compile_definitions(RTSafetyTest PRIVATE ENABLE_RT_SAFETY_CHECKS=1)
    target_compile_definitions(ProcessBlockTest PRIVATE ENABLE_RT_SAFETY_CHECKS=1)
    target_compile_definitions(QueueTest PRIVATE ENABLE_RT_SAFETY_CHECKS=1)
    target_compile_definitions(PerfHarness PRIVATE ENABLE_RT_SAFETY_CHECKS=1)
    target_compile_definitions(GPUResilienceTest PRIVATE ENABLE_RT_SAFETY_CHECKS=1)
endif()

# Debug builds get additional safety checks
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(RTSafetyTest PRIVATE RT_SAFETY_DEBUG=1)
    target_compile_definitions(ProcessBlockTest PRIVATE RT_SAFETY_DEBUG=1)
    target_compile_definitions(QueueTest PRIVATE RT_SAFETY_DEBUG=1)
    target_compile_definitions(PerfHarness PRIVATE RT_SAFETY_DEBUG=1)
    target_compile_definitions(GPUResilienceTest PRIVATE RT_SAFETY_DEBUG=1)
endif()