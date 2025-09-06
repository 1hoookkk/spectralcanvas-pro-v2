# 12 — Phase Vocoder: Accuracy & Phase Handling

Targets
- Preserve phase coherence across frames; minimize smearing and phasiness.
- Stable frequency tracking for peaks; robust synthesis with OLA.

Key Practices
- Use consistent analysis/synthesis windows; match overlap and scaling.
- Phase unwrapping: track instantaneous frequency via expected phase advance.
- Peak locking (optional): bind magnitude neighborhoods to dominant peaks.
- Avoid time-varying window shapes during processing.

Implementation Notes
- Store previous-frame phase; compute delta phase and correct by hop size.
- Use float32, `ScopedNoDenormals`, and avoid branches in hot loops.
- Pre-allocate all buffers; reuse FFT plans; avoid allocations in `processBlock()`.

Validation
- Compare against CCRMA SASP examples for tone and chirp tests.
- Check reconstruction error (RMS, max) under various hop/overlap settings.
- Listen tests: steady tones, transients, percussive material.

