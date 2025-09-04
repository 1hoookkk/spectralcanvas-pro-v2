# 08 — Audio Thread Do's & Don'ts

Do
- Preallocate in `prepareToPlay()`; reuse buffers, tables, state.
- Use atomics and SPSC queues for cross-thread communication.
- Keep processing deterministic, branch- and cache-friendly.

Don't
- No locks, allocations, logging, file I/O, or sleeps in `processBlock()`.
- No GUI or GPU calls from the audio thread.
- No blocking waits, futures, or message thread dependencies.

Validate with `pluginval_rt_safety` and `RTSafetyTest` before merging.

