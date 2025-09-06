# spectralCanvas Pro: Paint Sound, Sculpt Music

SpectralCanvas Pro is a revolutionary spectral audio editor and synthesizer that transforms sound into a living visual canvas. Inspired by pioneering visual music tools like MetaSynth, it allows you to paint sound directly onto a spectrogram, turning intuitive gestures into complex and beautiful music.

Where other tools present knobs and sliders, SpectralCanvas Pro offers a brush and a canvas. It’s an instrument designed for artists, sound designers, and producers who want to break free from traditional workflows and interact with audio in a completely new way.

## Core Features

-   **Visual Spectral Painting:** Directly edit and create audio by painting on a spectrogram. Your gestures become the music. Attenuate, amplify, or sculpt existing audio, or paint entirely new sounds from a blank canvas.
-   **Dynamic Timbre Engine:** At the heart of SpectralCanvas lies a powerful synthesis core that allows you to morph between radically different sonic textures in real-time. Create sounds that evolve from sharp and metallic to warm and vocal with a single, fluid brushstroke.
-   **Integrated Character Core:** A built-in, analog-inspired signal path adds rich harmonics and vintage warmth to your creations. Go from pristine digital clarity to saturated, tube-style character with a single, expressive control.
-   **Generative Pattern Engine:** Create intricate melodic and rhythmic patterns effortlessly. Let your gestures guide the creation of complex sequences that are always locked to your project's tempo.

## Project Status

This project is in active development. We are currently building out the core engine and creative interface. For a detailed look at our implementation plan, please see our public [**ROADMAP.md**](ROADMAP.md).

## Build Instructions

#### Prerequisites
- CMake 3.22+
- Visual Studio 2022 (Windows) or an equivalent modern C++20 compiler.
- Git

#### Build Steps
```bash
# 1. Clone the repository with its submodules
git clone --recursive https://github.com/1hoookkk/spectralcanvas-pro-v2.git
cd spectralcanvas-pro-v2

# 2. Configure the project using a CMake preset
# (This is the simplest way to get started)
cmake --preset x64-debug

# 3. Build the project
cmake --build --preset x64-debug

# 4. Run the tests
ctest --preset x64-debug
```

The built plugins and standalone application can be found in the `build/SpectralCanvasPro_artefacts/` directory. For more advanced build options and configurations, please consult the `CMakePresets.json` file.
