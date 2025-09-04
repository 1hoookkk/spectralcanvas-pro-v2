# 09 — Messaging & Concurrency

- Cross-thread messaging: use SPSC queues via `Source/Core/MessageBus.h`.
- Audio → GUI: push minimal, POD-like messages; process on message thread.
- GUI → Audio: update atomics or enqueue param-change events; no locks.
- Avoid shared mutable state without atomics; never block the audio thread.
- For periodic GUI updates, poll queues on a timer or `AsyncUpdater`.

