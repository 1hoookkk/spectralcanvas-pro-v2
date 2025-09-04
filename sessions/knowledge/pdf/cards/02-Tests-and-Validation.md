# 02 — Tests & Validation

- Unit/Integration: `ctest --test-dir build --output-on-failure`
- Enable tests at configure: `-DENABLE_TESTS=ON`
- Pluginval (quick): `cmake --build build --target pluginval_quick`
- Pluginval (full): `cmake --build build --target pluginval_test`
  - Results: `build/pluginval_results` (review text and XML where applicable)
- RT Safety: run `pluginval_rt_safety` and `RTSafetyTest` targets before merging.
- Perf: use `PerfHarness` targets for audio/DSP regressions.

