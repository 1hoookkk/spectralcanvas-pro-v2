# Agent Risk Assessment: RT Audio Development

## The Core Risk

* **False Confidence**: Agents can generate ✅ "PASS" assessments even if the code doesn't compile.
* **Why it matters**: RT audio has *zero margin for error*. Missteps are catastrophic at runtime.

## Specific Risk Categories

1. **API Assumptions**

   * Example:

     ```cpp
     void suspendProcessing(bool shouldBeSuspended) override;
     // ❌ Not a JUCE AudioProcessor virtual
     ```
   * Example:

     ```cpp
     hop_.reset();
     // ❌ HopScheduler has no reset()
     ```
   * Root cause: agents generalize from patterns, not your actual headers.

2. **Context Blindness**

   * Agents can't "see" JUCE headers or custom classes during reasoning.
   * Static analysis without real build context misses mismatches.

3. **RT-Safety False Positives**

   * Subtle violations (allocations through templates, implicit locks) may go unnoticed.
   * Result: glitches, dropouts, or crashes under load.

## Why It's Dangerous in RT Audio

* A **single allocation** in `processBlock()` → glitch/dropout.
* A **single lock** → priority inversion → audible glitch.
* Wrong latency report → host timing misalignment.
* Memory corruption → live performance crash, possible hardware/hearing risk.

Unlike general software:

* Bug in app → patch in next release.
* Bug in RT DSP → ruined show in real time.

---

## Mitigation Strategy: "Trust but Verify"

### 1. Mandatory Build Gate

Never trust an agent change until it compiles clean:

```bash
cmake --build build --config RelWithDebInfo
```

### 2. Multi-Layer Validation

Pipeline for every change:

1. **Agent analysis / patch**
2. **Build & link** (compile-time gate)
3. **Unit tests**
4. **RT safety stress tests** (ReactivationStress, allocation sentry)
5. **pluginval --strict**
6. **Manual review** (engineer sign-off)

### 3. Domain-Specific Verification

* `pluginval --strictness-level 8 <plugin>.vst3`
* Build with `-fsanitize=address,undefined` (ASan/UBSan).
* Run stress harness (`ReactivationStress.cpp`).
* Optional: heap allocation counters in debug builds.

### 4. Treat Agents as "Smart Interns"

* They accelerate development and spot issues (like `latencyWritePos_` crosstalk).
* They generate infrastructure (stress tests, simplification).
* **But**: you are the senior engineer. Agents assist, they don't sign off.

---

## The Value Proposition

Even with compilation errors:

* ✅ Found **real bug**: `latencyWritePos_` shared state.
* ✅ Preserved **RT safety**: no new allocs/locks in hot path.
* ✅ Simplified code: removed cruft.
* ✅ Added **verification infra**: stress harness + watchdog.

The compilation fixes were trivial once surfaced; the structural RT analysis was genuinely valuable.

---

## Verdict

* **Valuable with guardrails**:

  * Build/test after every agent patch.
  * Mandatory stress + pluginval validation.
  * Engineer sign-off required.

* **Dangerous without guardrails**:

  * Deploying agent code "as-is".
  * Believing ✅ PASS without compile/run validation.
  * Using agents as substitutes for engineering judgment.

---

✨ **Key Insight**
Agents are powerful accelerants. They catch issues and scaffold tests you might not have time to write.
But in RT audio, they're **never final authority** — they're copilots.

---

## Lessons Learned from RT DSP Reactivation

**What Worked:**
- Agent-driven code simplification removed 35+ lines of cruft
- Found critical latency delay bug that would cause channel crosstalk
- Maintained all RT-safety invariants throughout refactoring
- Generated comprehensive stress test infrastructure

**What Failed:**
- Static analysis passed code that didn't compile
- API assumptions about JUCE AudioProcessor and custom classes
- Missing build verification in validation pipeline

**The Fix:**
- Always run `cmake --build` after agent changes
- Add `build-validator` agent to permanent pipeline
- Treat agent output as "smart draft code" requiring verification