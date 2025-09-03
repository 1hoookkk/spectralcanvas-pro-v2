# Contributing to SpectralCanvas Pro

Thanks for your interest in contributing! This project targets real-time audio and GPU rendering, so please follow these guardrails.

## Getting Started
- Toolchain: CMake ≥ 3.22, Visual Studio 2022 (MSVC), Windows SDK 10+.
- Configure: `cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON`
- Build: `cmake --build build --config Debug`
- Tests: `ctest --test-dir build --output-on-failure`

## Real-Time (RT) Safety
- No allocations, locks, I/O, or logging in audio callbacks.
- Prefer fixed-size buffers, `std::array`, and `std::atomic` (relaxed) where appropriate.
- Use SPSC queues (`Source/Core/MessageBus.h`, `SpscRingBuffer.h`) for cross-thread messaging.
- See: `docs/Developer-Notes.md` for patterns and commands.

## Code Style
- C++20; keep headers and sources paired.
- Warnings: `/W4` (MSVC) or `-Wall -Wextra -Wpedantic`.
- Keep changes minimal and focused; avoid unrelated refactors.

## Tests
- Add targeted tests in `Tests/` mirroring existing patterns.
- Validate RT safety with `RTSafetyTest`, queue behavior with `QueueTest`, and performance with `PerfHarness`.

## Docs
- Update `plan.md` or `docs/` when behavior or architecture changes.
- If a component is planned but not implemented, mark it clearly as planned.

## Pull Requests
- Use clear titles and short descriptions of intent.
- Include build/test notes and any relevant logs.
- Ensure CI (when available) passes tests and warnings checks.

Happy hacking!

