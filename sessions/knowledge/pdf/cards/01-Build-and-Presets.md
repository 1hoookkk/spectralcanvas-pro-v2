# 01 — Build & Presets

- Configure (generic): `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON`
- Presets (example): `cmake --preset x64-debug` then `cmake --build --preset x64-debug`
- Build (manual): `cmake --build build --config Debug`
- Common outputs (examples):
  - VST3: `build/SpectralCanvasPro_artefacts/Debug/VST3/SpectralCanvasPro.vst3`
  - Standalone: `build/SpectralCanvasPro_artefacts/Debug/Standalone/SpectralCanvasPro.exe`
- Warnings: MSVC `/W4`, Clang/GCC `-Wall -Wextra -Wpedantic`
- Tip: Switch `Debug`/`Release` consistently across configure and build.

