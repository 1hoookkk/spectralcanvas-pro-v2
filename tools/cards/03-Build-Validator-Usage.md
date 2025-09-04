# 03 — Build Validator Usage

Purpose
- Ensure plugin builds cleanly across configurations and runs validation.

Commands
- Windows: `powershell -ExecutionPolicy Bypass -File tools/build-validator.ps1`
- Bash: `./tools/run-build-validator.sh` (or `tools/run-build-validator.bat` on Windows)

What It Does
- Configures with `CMakePresets.json` where available.
- Builds targets and runs `ctest` and `pluginval` quick/full.

Tips
- Use `-DENABLE_TESTS=ON` during configure.
- Inspect `build/pluginval_results` for issues.

