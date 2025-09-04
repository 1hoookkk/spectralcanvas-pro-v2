# 13 — FFT Performance & Planning

Goals
- Minimize per-block FFT overhead; ensure cache- and SIMD-friendly pipelines.

Guidelines
- Reuse FFT objects/plans across blocks; allocate in `prepareToPlay()`.
- Prefer power-of-two sizes; benchmark zero-padding trade-offs.
- Align buffers to 16/32 bytes for SIMD ops; avoid unaligned access.
- Batch transforms when possible for better locality.

RT Safety
- No plan creation or temporary allocations on the audio thread.
- Use stack or pre-sized work buffers only; avoid std::vector growth.
- Denormal protection around inner loops.

Validation
- Microbenchmarks: N ∈ {256,512,1024,2048,4096}, varying overlaps.
- Compare throughput vs JUCE FFT; verify results match within 1e-6.

