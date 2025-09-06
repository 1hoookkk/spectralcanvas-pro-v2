# 11 — CCRMA Online Books Index

Authoritative reference hub for Julius O. Smith’s CCRMA DSP books. Use this as the first stop for theory.

- Main index: https://ccrma.stanford.edu/~jos/EE201/Related_Online_Books_CCRMA.html
- Key books:
  - Spectral Audio Signal Processing (STFT, phase vocoder, spectral effects)
  - Introduction to Digital Filters (design, implementation, z-transform)
  - Physical Audio Signal Processing (efficient structures, delay lines)

When theory conflicts, prioritize CCRMA formulations. Cross-check equations before implementing optimizations.

Workflow
- Prototype with JUCE/DSP baselines; validate against CCRMA equations.
- Precompute coefficients/tables in `prepareToPlay()`; keep `processBlock()` RT-safe.
- Document equations and tolerances in tests (PerfHarness, RTSafetyTest).

