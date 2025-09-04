# 03 — RT Safety Essentials

- No allocations/locks/logging inside `processBlock()`.
- Pre-allocate in `prepareToPlay()`; reuse buffers and objects.
- Cross-thread messaging: use SPSC queues (`Source/Core/MessageBus.h`).
- Avoid blocking calls (file I/O, sleeps, futures) on the audio thread.
- Prefer atomics and lock-free structures for state shared with audio thread.
- Validate with `pluginval_rt_safety` and `RTSafetyTest` before merge.

