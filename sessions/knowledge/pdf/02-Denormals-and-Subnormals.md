# 02 — Denormals/Subnormals

- Problem: very small floats cause massive slowdowns on some CPUs.
- Mitigation: enable FTZ/DAZ (flush‑to‑zero, denormals‑are‑zero) at init.
- DSP trick: add tiny DC offset or noise dither (~1e‑24f) post‑IIR to avoid denormals.
- Avoid: `powf`, `tanhf` on near‑zero without guards; prefer approximations if needed.
- Validate: perf test near silence; profile release builds (O2/Ofast) with FTZ/DAZ on.

