# 05 — Coding Style & Naming

- Language: C++20; Indentation: 4 spaces.
- Warnings: MSVC `/W4`; Clang/GCC `-Wall -Wextra -Wpedantic`.
- Types/classes/files: PascalCase (e.g., `SpectralCanvasProAudioProcessor`).
- Methods/functions: camelCase; Constants/macros: UPPER_SNAKE_CASE.
- Keep headers/sources paired; minimal headers in `Source/` with matching `.cpp`.
- Absolutely no allocations/locks/logging in `processBlock()`.

