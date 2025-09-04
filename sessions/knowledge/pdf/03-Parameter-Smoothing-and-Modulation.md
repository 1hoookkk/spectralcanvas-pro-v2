# 03 — Parameter Smoothing & Modulation

- Avoid zipper noise: smooth GUI/automation changes with 1‑pole or TPT smoothing.
- Time constants: express in ms; convert to per‑sample alpha using sample rate.
- Mod routing: combine base param + LFO/env with headroom; clamp bounds early.
- Exponential params: smooth in log domain for perceptual linearity (e.g., frequency, gain dB).
- Discrete changes: crossfade or dual‑state ramping for filter topology changes.

