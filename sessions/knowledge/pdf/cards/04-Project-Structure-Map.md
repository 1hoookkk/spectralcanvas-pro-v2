# 04 — Project Structure Map

- `Source/` — main code
  - `Core/` — threading, queues, params, atlas
  - `DSP/` — engines, loaders, STFT, schedulers
  - `GUI/` — editor, HUD, canvas components
  - `Audio/` — sample I/O
  - `Viz/` — GPU backends
  - `nodes/` — DSP nodes
  - `rt/` — RT-lite helpers
- `Tests/` — CTest + some GoogleTest targets
- `docs/`, `CMakeLists.txt`, `CMakePresets.json`, `pluginval.cmake`

