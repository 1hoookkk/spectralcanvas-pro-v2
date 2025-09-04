# 01 — DSP Block Processing & Latency

- Host block size: design to handle variable `numSamples`; avoid assumptions.
- Internal block size: choose power‑of‑two for SIMD/FFT; handle tail processing.
- Latency sources: linear‑phase filters, lookahead, oversampling, STFT analysis windows.
- Report latency: if applicable, report via processor latency API to keep DAW aligned.
- State continuity: carry buffers across calls (delay lines, overlap) without reallocating.
- Dry/wet paths: align with identical latency; compensate before mixing to avoid combing.

