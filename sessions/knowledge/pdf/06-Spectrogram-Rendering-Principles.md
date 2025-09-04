# 06 — Spectrogram Rendering Principles

- Log frequency axis: map bins to perceptual scale for readability (mel, log2).
- Dynamic range: compress magnitude to dB with floor; apply gamma for visual contrast.
- Colormaps: perceptually uniform (e.g., viridis) avoid misleading contrast.
- Progressive uploads: stream per‑column/row updates to GPU to keep UI responsive.
- Tiling: use texture atlas/tiles for large canvases; cull off‑screen work.
- Smoothing: optional temporal smoothing for denoise; beware blurring transients.

