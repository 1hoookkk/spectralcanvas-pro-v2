# Validation Instructions

## 1. pluginval (Highly Recommended)
This is the gold standard for checking plugin stability.

1.  [Install `pluginval`](https://github.com/Tracktion/pluginval/releases) and add it to your PATH.
2.  Build the project first.
3.  Run the custom CMake target from your build directory:
    ```powershell
    cmake --build . --config Release --target pluginval_strict
    ```

A successful run will report no errors or warnings.

## 2. Sanitizers
These tools find deep memory and threading bugs.

1.  Configure a sanitized build as described in `BUILD.md` (`-DSC_ENABLE_ASAN=ON`).
2.  Build the sanitized version.
3.  Run the Standalone application or load the VST3 in your DAW. The plugin will crash and print a detailed report to the console if it detects an error.
