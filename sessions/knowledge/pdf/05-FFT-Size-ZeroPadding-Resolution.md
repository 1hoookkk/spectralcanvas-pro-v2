# 05 — FFT Size, Zero‑Padding, Resolution

- Frequency bin resolution: `df = Fs / N`; larger `N` → finer `df`, higher latency.
- Time resolution: inversely related to window length; choose N for task (analysis vs UI).
- Zero padding: interpolates spectrum; helps peak picking but not true resolution increase.
- Power vs magnitude: use `|X|^2` for power; dBFS display needs reference scaling.
- Window normalization: match energy across windows for accurate magnitude in dB.
- SIMD & plans: build FFT plans once; reuse; align data; prefer power‑of‑two sizes.

