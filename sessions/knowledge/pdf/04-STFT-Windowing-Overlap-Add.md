# 04 — STFT Windowing & Overlap‑Add

- Window choice: Hann for COLA at common overlaps; Blackman/Hamming for leakage tradeoffs.
- Hop size: `H = N / R` where `N` is FFT size and `R` is overlap ratio (e.g., 4× → H=N/4).
- COLA: ensure chosen window + hop satisfy constant overlap‑add to avoid amplitude ripple.
- Zero padding: pad to FFT size; improves interpolation of peaks but not true resolution.
- Phase handling: for phase vocoder, accumulate phase with unwrapped delta; preserve transients.
- Realtime: precompute windows; avoid allocations; use in‑place FFT; keep circular buffers.

