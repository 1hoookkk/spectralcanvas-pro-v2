# 08 — Oversampling & Anti‑Aliasing

- When: nonlinear DSP (waveshapers, rectifiers) generates harmonics → oversample.
- Factors: 2×/4× common; 8× for aggressive saturation; weigh CPU/latency.
- Filters: use polyphase halfband or efficient linear‑phase FIR; precompute coeffs.
- Latency: report added latency; keep dry path aligned or add compensating delay.
- State: process up/downsampled blocks from preallocated buffers; avoid per‑sample allocs.
- Validation: spectral plots at Nyquist; compare alias energy vs. base rate.

