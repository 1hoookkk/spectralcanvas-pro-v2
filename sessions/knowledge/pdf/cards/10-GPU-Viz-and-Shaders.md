# 10 — GPU/Viz & Shaders

- Shaders: enable staging with `-DSCP_INSTALL_SHADERS=ON`.
- Resilience: `-DDISABLE_GPU_RESILIENCE=ON` to bypass fallback paths during perf tests.
- Perf HUD: `-DDISABLE_PERF_HUD=ON` to disable overlay.
- Threading: perform GPU/Viz work on appropriate threads; never call GPU from audio thread.
- GUI updates: use JUCE message thread patterns (`AsyncUpdater`, timers) for safe redraws.

