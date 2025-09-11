cmake_minimum_required(VERSION 3.21)
project(SpectralCanvasLite VERSION 1.0.0)

# ===== Project Options =====
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(SC_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(SC_ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(SC_ENABLE_TRACY "Enable Tracy profiler" OFF)

# ===== JUCE Dependency =====
# Add JUCE via subdirectory
add_subdirectory(C:/JUCE ${CMAKE_BINARY_DIR}/JUCE)

# ===== Tracy Profiler =====
if (SC_ENABLE_TRACY)
    message(STATUS "Tracy profiler enabled")
    include(FetchContent)
    FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG v0.12.2
    )
    FetchContent_MakeAvailable(tracy)
endif()

# ===== Sanitizer Configuration =====
if (SC_ENABLE_ASAN)
  message(STATUS "AddressSanitizer enabled")
  if (MSVC)
    add_compile_options(/fsanitize=address /Zi)
    add_link_options(/fsanitize=address)
  else()
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
  endif()
endif()

if (SC_ENABLE_TSAN)
  if (MSVC)
    message(WARNING "ThreadSanitizer is not supported on MSVC.")
  else()
    message(STATUS "ThreadSanitizer enabled")
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=thread)
  endif()
endif()

# ===== Source File Lists (Explicit is better than GLOB) =====
set(SCL_CORE_SOURCES
  Source/Core/SpectralCanvasLiteProcessor.h
  Source/Core/SpectralCanvasLiteProcessor.cpp
  Source/Core/Params.h
  Source/Core/MaskColumn.h
)

set(SCL_UI_SOURCES
  Source/UI/SpectralCanvasLiteEditor.h
  Source/UI/SpectralCanvasLiteEditor.cpp
)

set(SCL_DSP_SOURCES
  Source/DSP/SpectralProcessor.h
  Source/DSP/SpectralProcessor.cpp
  Source/DSP/SynthEngineStub.h
  Source/DSP/SynthEngineStub.cpp
)

set(SCL_UTILS_SOURCES
  Source/Utils/SpscQueueLite.h
  Source/Utils/TracyProfiler.h
)

# ===== JUCE Plugin Target =====
juce_add_plugin(SpectralCanvasLite
    COMPANY_NAME "YourCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    FORMATS VST3 Standalone
    PRODUCT_NAME "SpectralCanvasLite"
    VST3_CATEGORIES "Fx"
    PLUGIN_MANUFACTURER_CODE "YrCo"
    PLUGIN_CODE "ScLt"
)

target_sources(SpectralCanvasLite PRIVATE
  ${SCL_CORE_SOURCES}
  ${SCL_UI_SOURCES}
  ${SCL_DSP_SOURCES}
  ${SCL_UTILS_SOURCES}
)

target_link_libraries(SpectralCanvasLite PRIVATE
    juce::juce_dsp
    juce::juce_audio_utils
)

# Link Tracy if enabled
if (SC_ENABLE_TRACY)
    target_link_libraries(SpectralCanvasLite PRIVATE TracyClient)
    target_compile_definitions(SpectralCanvasLite PRIVATE TRACY_ENABLE)
endif()

# VST3-only plugin (no VST2 conflicts)
target_compile_definitions(SpectralCanvasLite PRIVATE JUCE_VST3_CAN_REPLACE_VST2=0)

# ===== Validation Target: pluginval =====
find_program(PLUGINVAL_EXECUTABLE NAMES pluginval)
if (PLUGINVAL_EXECUTABLE)
  # Note: The exact path to the built VST3 can vary. This is a robust way to find it.
  set(PLUGIN_BINARY "$<TARGET_FILE_DIR:SpectralCanvasLite>/$<TARGET_FILE_NAME:SpectralCanvasLite>.vst3")
  if(WIN32)
      set(PLUGIN_BINARY "$<TARGET_FILE_DIR:SpectralCanvasLite>/SpectralCanvasLite_artefacts/$<CONFIG>/VST3/SpectralCanvasLite.vst3")
  endif()

  add_custom_target(pluginval_strict
    COMMAND "${PLUGINVAL_EXECUTABLE}" --strictness-level 10 --validate "${PLUGIN_BINARY}"
    DEPENDS SpectralCanvasLite
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Running pluginval (strict) on SpectralCanvasLite..."
    VERBATIM
  )
else()
  message(STATUS "pluginval not found. The 'pluginval_strict' target will be unavailable.")
endif()