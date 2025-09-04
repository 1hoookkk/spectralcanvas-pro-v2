# CLAUDE_HEADERS — spectralcanvas-pro-v2

This document records machine-readable headers and specs for AI-assisted patches
so future contributors understand intent, invariants, and testing expectations.

## Commit Context

- **Thread-safety work**: atomic latency reporting, atomic snapshot GUI handover.
- **Compatibility work**: JUCE windowing API differences across versions:
  `fillWindowingTable(...)` (older) vs `fillWindowingTables(...)` (newer).

## New Files

### `Source/Compat/WindowingCompat.h`

```text
Purpose: Provide a stable, version-agnostic wrapper around JUCE's windowing helpers.
Exports: compat::fillWindow(dest, size, WindowingMethod)
Behavior:
  - Prefer WindowingFunction<T>::fillWindowingTables if available.
  - Else fall back to WindowingFunction<T>::fillWindowingTable if available.
  - Else compute Hann/Hamming/Blackman windows manually.
Thread-safety: Not stateful; pure functions only.
Dependencies: <juce_dsp/juce_dsp.h>, <cmath>.
```

## Modified Call Sites

Replace all direct usages of `juce::dsp::WindowingFunction<Sample>::fillWindowingTable(s)` with:

```cpp
compat::fillWindow(buffer, size, juce::dsp::WindowingFunction<Sample>::hann /* or desired */);
```

## Invariants

* Latency must be reported via `AudioProcessor::getLatencySamples()` overridden to return the atomic value.
* `updateReportedLatency(newLatency)` must be called from `prepareToPlay()` (and on FFT/HOP changes) and also call `AudioProcessor::setLatencySamples(newLatency)` to keep hosts in sync.
* GUI must read **only** from immutable snapshots (`std::shared_ptr<const Snapshot>` via atomic load), never from live audio-thread data.

## Tests / Validation

* Build on your JUCE version (both older/newer) — WindowingCompat should compile either way.
* Run `pluginval --strict`:

  * Expect `Reported latency: 384` unless FFT/HOP differs.
  * Pass "Open editor whilst processing" test.
* Run minimal reproducer (safe mode) — no crash.
* (Optional) ASan build of host + plugin if any issues.

## Header to paste into changed files

```cpp
/* ==========================================================================
   spectralcanvas-pro-v2 — Compatibility + Thread-Safety Hardening
   WindowingCompat shim + std::isfinite portability

   Summary:
   - Introduce compat::fillWindow(...) to abstract JUCE's windowing API
     differences (fillWindowingTable vs fillWindowingTables).
   - Provide math fallback for Hann/Hamming/Blackman if neither helper exists.
   - Use <cmath> std::isfinite for portability.

   Invariants:
   - Atomic latency reporting: getLatencySamples() returns atomic value.
   - GUI reads immutable snapshots via atomic shared_ptr load (no live audio ptrs).
   - No placement-new of WindowingFunction on audio thread.

   Testing:
   - Build on target JUCE version(s)
   - Run pluginval (expect stable latency & no editor-open crash)
   - Optional: ASan/UBSan run if issues persist
   ========================================================================== */
```