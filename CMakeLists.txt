cmake_minimum_required(VERSION 3.22)
project(SpectralCanvasPro VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.8
)
FetchContent_MakeAvailable(juce)

juce_add_plugin(SpectralCanvasPro
    COMPANY_NAME "SpectralCanvas"
    VERSION "0.1.0"
    IS_SYNTH TRUE
    PLUGIN_CODE Scp1
    FORMATS VST3 Standalone
    PRODUCT_NAME "spectralCanvas Pro"
    MICROPHONE_PERMISSION_ENABLED FALSE
    BUNDLE_ID com.spectralcanvas.pro
)

target_sources(SpectralCanvasPro PRIVATE
    # Main plugin files
    Source/SpectralCanvasProAudioProcessor.h
    Source/SpectralCanvasProAudioProcessor.cpp
    Source/SpectralCanvasProEditor.h
    Source/SpectralCanvasProEditor.cpp
    Source/SpectralPaintProcessor.h
    Source/SpectralPaintProcessor.cpp
    
    # Core system
    Source/Core/Params.h
    Source/Core/MessageBus.h
    Source/Core/ParameterSnapshot.h
    Source/Core/ReconfigManager.h
    Source/Core/SpectralCoreEngine.h
    Source/Core/ContinuousVerification.h
    Source/Core/RealtimeSafeTypes.h
    Source/Core/PaintQueue.h
    Source/Core/LatencyTracker.h
    Source/Core/GPUStatus.h
    
    # RT Infrastructure
    Source/rt/MaskColumnLite.h
    Source/rt/SpscQueueLite.h
    
    # Modern DSP Nodes
    Source/nodes/SpectralPaintNode.h
    Source/nodes/SpectralPaintNode.cpp
    
    # DSP processing
    Source/DSP/SpectralEngine.h
    Source/DSP/SpectralEngine.cpp
    Source/DSP/SampleLoader.h
    Source/DSP/SampleLoader.cpp
    Source/DSP/MaskTestFeeder.h
    Source/DSP/MaskTestFeeder.cpp
    Source/DSP/KeyFilter.h
    Source/DSP/KeyFilter.cpp
    Source/DSP/SpectralEngineStub.h
    Source/DSP/SpectralEngineStub.cpp
    
    # GUI components
    Source/GUI/CanvasComponent.h
    Source/GUI/CanvasComponent.cpp
    Source/GUI/TopStrip.h
    Source/GUI/TopStrip.cpp
    Source/GUI/BottomBar.h
    Source/GUI/BottomBar.cpp
    Source/GUI/MinimalTopStrip.h
    Source/GUI/MinimalTopStrip.cpp
    Source/GUI/MinimalBottomStrip.h
    Source/GUI/MinimalBottomStrip.cpp
    Source/GUI/PerfHUD.h
    Source/GUI/PerfHUD.cpp
    Source/GUI/SpectrogramComponent.h
    Source/GUI/SpectrogramComponent.cpp
    Source/GUI/HudFormatter.h
    
    # Spectral Pipeline (MVP)
    Source/Audio/SampleManager.h
    Source/Audio/SampleManager.cpp
    Source/DSP/SpectralModel.h
    Source/DSP/SpectralModel.cpp
    Source/DSP/SpectralMask.h
    Source/DSP/SpectralPlayer.h
    Source/DSP/SpectralPlayer.cpp
    
    # GPU rendering
    Source/Viz/GpuRenderer.h
    Source/Viz/GpuRenderer.cpp
    Source/Viz/RenderThread.h
    Source/Viz/DeviceLostHandler.h
    Source/Viz/backends/D3D11Renderer.h
    Source/Viz/backends/D3D11Renderer.cpp
    
    # Shader resources (HLSL files - temporarily excluded from build)
    # Source/Viz/shaders/FullscreenQuad.hlsl
    # Source/Viz/shaders/ParticleSystem.hlsl
    # Source/Viz/shaders/SpectralVisualization.hlsl
    # Source/Viz/shaders/ShaderCommon.hlsl
    # Source/Viz/shaders/ShaderConfig.hlsl
)

target_compile_definitions(SpectralCanvasPro
    PRIVATE
      JUCE_WEB_BROWSER=0
      JUCE_USE_CURL=0
      JUCE_DISPLAY_SPLASH_SCREEN=0
      JUCE_VST3_CAN_REPLACE_VST2=0
      JUCE_STRICT_REFCOUNTEDPOINTER=1
      PHASE4_EXPERIMENT=1
)

# GPU resilience feature flag
option(DISABLE_GPU_RESILIENCE "Disable GPU device-lost recovery and WARP fallback" OFF)
if(DISABLE_GPU_RESILIENCE)
    target_compile_definitions(SpectralCanvasPro PRIVATE DISABLE_GPU_RESILIENCE=1)
endif()

# Performance HUD feature flag  
option(DISABLE_PERF_HUD "Disable Performance HUD overlay" OFF)
if(DISABLE_PERF_HUD)
    target_compile_definitions(SpectralCanvasPro PRIVATE DISABLE_PERF_HUD=1)
endif()

# Windows-specific defines
if(WIN32)
    target_compile_definitions(SpectralCanvasPro PRIVATE
        NOMINMAX 
        WIN32_LEAN_AND_MEAN
    )
endif()

target_link_libraries(SpectralCanvasPro
    PRIVATE
      juce::juce_audio_utils
      juce::juce_audio_formats
      juce::juce_dsp
      juce::juce_gui_extra
)

# Windows-specific D3D11 libraries
if(WIN32)
    target_link_libraries(SpectralCanvasPro PRIVATE
        d3d11.lib
        dxgi.lib
        d3dcompiler.lib
    )
endif()

if (MSVC)
  target_compile_options(SpectralCanvasPro PRIVATE /W4 /permissive- /std:c++20)
  # Debug configuration
  target_compile_options(SpectralCanvasPro PRIVATE $<$<CONFIG:Debug>:/RTC1>)
  # Release optimization
  target_compile_options(SpectralCanvasPro PRIVATE $<$<CONFIG:Release>:/O2>)
else()
  target_compile_options(SpectralCanvasPro PRIVATE -Wall -Wextra -Wpedantic -std:c++20)
endif()

# Pattern #7: Include pluginval integration for continuous verification
include(pluginval.cmake)

# Enable testing for CTest integration
option(BUILD_TESTING "Enable testing with CTest" ON)
if(BUILD_TESTING)
    enable_testing()
    
    # Stricter warnings in tests for catching issues early
    if(MSVC)
        add_compile_options($<$<CONFIG:Debug>:/W4> $<$<CONFIG:RelWithDebInfo>:/W4> $<$<CONFIG:Debug>:/WX>)
    else()
        add_compile_options($<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic -Werror>)
    endif()
    
    add_subdirectory(Tests)
endif()
