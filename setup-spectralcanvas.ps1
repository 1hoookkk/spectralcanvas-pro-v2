# === Setup folders ===
$root = "C:\spectralCanvas Pro"
$src  = "$root\Source"

if (!(Test-Path $root)) { mkdir $root }
if (!(Test-Path $src))  { mkdir $src }

# Helper function to write file content
function Write-File($path, $content) {
    Set-Content -Path $path -Value $content -Encoding UTF8
    Write-Host "Created $path"
}

# === Top-level CMakeLists.txt ===
Write-File "$root\CMakeLists.txt" @'
cmake_minimum_required(VERSION 3.22)
project(SpectralCanvasPro VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.4
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
    Source/SpectralCanvasProAudioProcessor.h
    Source/SpectralCanvasProAudioProcessor.cpp
    Source/SpectralCanvasProEditor.h
    Source/SpectralCanvasProEditor.cpp
    Source/AudioEngine.h
    Source/AudioEngine.cpp
    Source/AdditiveBank.h
    Source/AdditiveBank.cpp
    Source/CanvasField.h
    Source/Mailbox.h
    Source/EventQueue.h
    Source/PaintModel.h
    Source/PaintModel.cpp
)

target_compile_definitions(SpectralCanvasPro
    PRIVATE
      JUCE_WEB_BROWSER=0
      JUCE_USE_CURL=0
      JUCE_DISPLAY_SPLASH_SCREEN=0
      JUCE_VST3_CAN_REPLACE_VST2=0
      JUCE_STRICT_REFCOUNTEDPOINTER=1
)

target_link_libraries(SpectralCanvasPro
    PRIVATE
      juce::juce_audio_utils
      juce::juce_dsp
      juce::juce_gui_extra
)

if (MSVC)
  target_compile_options(SpectralCanvasPro PRIVATE /W4 /permissive-)
else()
  target_compile_options(SpectralCanvasPro PRIVATE -Wall -Wextra -Wpedantic)
endif()
'@

# === Now the Source files ===

# Example: Mailbox.h
Write-File "$src\Mailbox.h" @'
#pragma once
#include <atomic>
#include <cstdint>

template <typename T>
struct Mailbox
{
    std::atomic<uint32_t> readIdx { 0 }, writeIdx { 1 };
    T buffers[3];

    T&       writable() noexcept { return buffers[writeIdx.load(std::memory_order_relaxed)]; }
    const T& readable() const noexcept { return buffers[readIdx.load(std::memory_order_acquire)]; }

    void publish() noexcept
    {
        const auto w = writeIdx.load(std::memory_order_relaxed);
        const auto r = readIdx.exchange(w, std::memory_order_acq_rel);
        writeIdx.store(3u - w - r, std::memory_order_release);
    }

    void clearAll() noexcept { buffers[0] = T{}; buffers[1] = T{}; buffers[2] = T{}; }
};
'@

# === Repeat for each file (CanvasField.h, EventQueue.h, AdditiveBank.* etc.) ===
# (Paste the code I provided earlier for each file here, wrapped in the same Write-File block.)

# Example for CanvasField.h
Write-File "$src\CanvasField.h" @'
#pragma once
#include <algorithm>

struct CanvasField
{
    static constexpr int numBins = 1024;
    alignas(64) float energy[numBins]{};

    void clear() noexcept { std::fill(std::begin(energy), std::end(energy), 0.0f); }

    void blendFrom (const CanvasField& src, float alpha = 1.0f) noexcept
    {
        for (int i = 0; i < numBins; ++i)
            energy[i] = energy[i] * (1.0f - alpha) + src.energy[i] * alpha;
    }
};
'@

# --- you’d continue like this for each file (EventQueue.h, AdditiveBank.h/.cpp,
#     AudioEngine.h/.cpp, PaintModel.h/.cpp, SpectralCanvasProAudioProcessor.*,
#     SpectralCanvasProEditor.*) pasting the code from our previous step ---
