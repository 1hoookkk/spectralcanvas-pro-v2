# 06 — CMake Options Cheatsheet

- `-DSCP_INSTALL_SHADERS=ON` — stage HLSL shaders for GPU paths.
- `-DENABLE_TESTS=ON` — build test targets and CTest integration.
- `-DDISABLE_GPU_RESILIENCE=ON` — skip resilience paths (for perf testing).
- `-DSPECTRAL_ENABLE_MP3=OFF` — disable MP3 to slim deps/footprint.
- `-DDISABLE_PERF_HUD=ON` — remove perf HUD overlay in GUI builds.

Usage example: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DSCP_INSTALL_SHADERS=ON`.

