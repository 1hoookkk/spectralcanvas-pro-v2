# SpectralCanvas Pro – DSP Specialist To-Do List

> Format designed for DSP engineers **and** Claude Code integration.
> Each task uses the 5-part structure Claude understands:  
> **Task → Context → To-Do → Constraints → Output**

---

## 🔑 Phase 1: Real-Time Safety Infrastructure

### Task: Replace shared_ptr with handle-based SamplePool
- **Context**:
  - Current `SampleMessage` uses `std::shared_ptr<SampleData>` in audio thread.
  - This risks allocations / refcount ops in `processBlock()`.
  - `RealtimeMemoryManager` is already imported from ARTEFACT.
- **To-Do**:
  - [ ] Add `SamplePool` (8 slots) to `RealtimeMemoryManager`.
  - [ ] Switch `SampleMessage` to carry an `int handle`.
  - [ ] Update `MessageBus` typedefs → `SpscRingBuffer<SampleMessage, 8>`.
  - [ ] Update UI loader → allocate slot, push handle.
  - [ ] Update audio `processBlock()` → pop handle, deref from pool.
- **Constraints**:
  - No allocations, locks, or refcounts in audio thread.
  - All slots must be trivially copyable + preallocated.
- **Output**:
  - Updated `RealtimeMemoryManager.h`
  - Updated `SampleMessage.h`
  - Updated `MessageBus.h`
  - UI + audio snippets integrated.

---

## 🎚️ Phase 2: DSP Core Path

### Task: Implement minimal paint→audio pipeline
- **Context**:
  - Paint UI produces `MaskColumn` events.
  - Audio thread must consume via `MaskColumnQueue`.
  - Current pipeline wiring incomplete (paint not audible).
- **To-Do**:
  - [ ] Confirm `CanvasComponent` pushes mask events.
  - [ ] In `processBlock()`, drain `MaskColumnQueue` each block.
  - [ ] Apply mask bins to `SpectralEngine` state.
  - [ ] Add temporary test: white column = unity gain modulation.
- **Constraints**:
  - Zero allocations in audio callback.
  - Use `SpscRingBuffer<MaskColumn, N>` only.
- **Output**:
  - Working "paint changes audio" path with test modulation.

---

### Task: Integrate ColorToSpectralMapper
- **Context**:
  - Imported from ARTEFACT, supports HSB→spectral mappings, harmonization.
  - Not yet wired to SpectralCanvas paint path.
- **To-Do**:
  - [ ] Convert brush color → `ColorAnalysis` params.
  - [ ] Push mapping results to `ParameterQueue`.
  - [ ] Apply mapped params inside `processBlock()` spectral engine.
- **Constraints**:
  - Run only UI→Audio, not GPU thread.
  - All heavy math (HSB→scale curves) done UI-side.
- **Output**:
  - Spectral mask changes based on brush color.

---

### Task: Implement minimal sample playback
- **Context**:
  - Samples load via UI but not audible.
  - Pool + queue infra required first (see Phase 1).
- **To-Do**:
  - [ ] Allocate sample into pool, push handle.
  - [ ] Audio thread derefs and copies small segment → output buffer.
  - [ ] Add basic playback position counter.
- **Constraints**:
  - Must respect audio block size boundaries.
  - No disk I/O or resampling in `processBlock()`.
- **Output**:
  - "Load Sample → hear playback" verified.

---

## 🎨 Phase 3: GPU & Visualization

### Task: GPU renderer resilience
- **Context**:
  - Current status: "GPU: No Active Renderer".
  - GPU lost events cause silence/blank UI.
- **To-Do**:
  - [ ] Implement D3D11 WARP fallback.
  - [ ] Add staging buffer upload pattern.
  - [ ] Verify recovery <100ms.
- **Constraints**:
  - No blocking calls in render loop.
- **Output**:
  - Renderer stays alive under GPU resets.

---

## 📊 Phase 4: Performance & Testing

### Task: Expand Debug Overlay
- **Context**:
  - Debug overlay already togglable.
  - Lacks queue depth / pool usage info.
- **To-Do**:
  - [ ] Show `SampleQueue.size/freeSpace`.
  - [ ] Show active handles in audio thread.
  - [ ] Show paint latency (UI→Audio).
- **Constraints**:
  - No allocations in overlay update.
- **Output**:
  - Realtime metrics visible for dev/debug.

---

### Task: Strengthen validation pipeline
- **Context**:
  - CLAUDE.md requires pluginval ≥5 and RTSafetyTest pass.
- **To-Do**:
  - [ ] Add unit tests for SpscRingBuffer (overflow, underflow).
  - [ ] Add SamplePool allocation/free stress test.
  - [ ] Re-enable STFT identity/continuity tests.
- **Constraints**:
  - Tests must run in CI.
- **Output**:
  - CI green gate: pluginval strictness ≥5, all RT tests pass.

---

# ✅ Summary Roadmap

1. **Infra** → SamplePool + handle-based messages.  
2. **DSP Core** → Paint→Audio + ColorToSpectralMapper + Sample playback.  
3. **GPU** → Resilience + visualization improvements.  
4. **Perf/Tests** → Debug overlay + pluginval/test suite gating.