# 14 — Resampling & Interpolation

Use Cases
- Offline sample-rate conversion for assets; real-time pitch/time tweaks.

Choices
- Linear/Cubic for light RT tasks; polyphase/Windowed-Sinc for quality.
- Band-limit before downsampling; oversample before heavy non-linear steps.

RT Guidance
- Precompute filter kernels (polyphase) in `prepareToPlay()`.
- Keep per-sample inner loops branch-free; use circular buffers.
- Validate with sweeps/impulses; check aliasing and passband ripple.

Integration
- JUCE resamplers as baseline; replace only with measured improvements.

