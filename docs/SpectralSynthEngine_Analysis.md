# Technical Analysis: Spectral Synthesis Engine

**Source Location:** `C:\spectralCanvasPro\Source\Core\`
**Key Files:** `SpectralSynthEngine.h`, `SpectralSynthEngine.cpp`, `PaintEngine.h`

This document provides an architectural overview of the `SpectralSynthEngine` for the purpose of integration into the new `SpectralCanvasPro` foundation.

---

## 1. Overview & Purpose

The `SpectralSynthEngine` is the creative counterpart to the `PsychoacousticMaskingEngine`. While the psychoacoustic engine is for *analyzing* and *modifying* existing sound, the synth engine is for *generating* sound directly from user input. It is the core of the "paint-to-audio" feature.

The engine is designed as a real-time, polyphonic additive synthesizer. It receives "paint events" from the UI—representing a user's brush strokes on the canvas—and translates them into audible sound by spawning and controlling a pool of oscillators (voices).

---

## 2. Core Architecture & API

The system is split into two main conceptual parts:

*   **`PaintEngine`:** A more abstract, high-level class that seems to represent an older or more comprehensive vision. It manages concepts like sparse canvas storage (`CanvasRegion`), individual strokes (`Stroke`), and contains its own internal, drift-free `Phasor` oscillator.
*   **`SpectralSynthEngine`:** A more modern, focused, and JUCE-native implementation. It uses a `VoicePool` of modern `juce::dsp` components and is designed as a singleton for easy access. This appears to be the most recent and relevant implementation.

For our purposes, we will focus on the **`SpectralSynthEngine`** as the primary target for migration.

### `SpectralSynthEngine` Public API:
```cpp
class SpectralSynthEngine
{
public:
    // 1. One-time setup on the audio thread
    void prepare(double sampleRate, int maxBlockSize) noexcept;

    // 2. Main entry point from UI to queue a paint event
    void pushGestureRT(const PaintEvent& g) noexcept;

    // 3. Main audio callback to render the sound
    void processAudioBlock(juce::AudioBuffer<float>& buffer, double sampleRate) noexcept;

    // 4. Setters for real-time parameter control
    void setHarmonicDepth(float v) noexcept;
    void setMasterGain(float v) noexcept;
    void setNumPartials(int n) noexcept;
    // ... and others
};
```

---

## 3. Data Flow: From Paint to Audio

The engine's data flow is a clear example of a real-time safe UI-to-audio pipeline:

1.  **UI Captures Input:** The UI (not part of the engine itself) captures a user's brush stroke. This is represented by an external `PaintEvent` struct, which contains normalized X/Y coordinates and pressure.

2.  **Gesture Pushed to Engine:** The UI calls `SpectralSynthEngine::pushGestureRT()` on the message thread, passing in the `PaintEvent`.

3.  **Event Conversion & Queuing:** Inside the engine, the `PaintEvent` is converted into a smaller, internal `InternalPaintEvent`. This conversion is a critical step:
    *   The `Y` coordinate is mapped to a base frequency (Hz).
    *   This frequency is then **quantized to a musical scale** (C major in the current implementation) using the `HarmonicQuantizer` component. This is a key feature that makes the output musical.
    *   The `X` coordinate is mapped to stereo pan position.
    *   `Pressure` is mapped to amplitude and the number of harmonic partials.
    *   The resulting `InternalPaintEvent` is pushed into a lock-free, single-producer, single-consumer ring buffer (`SpscRing`).

4.  **Audio Thread Processing:** In `processAudioBlock()`, the audio thread safely pops events from the `SpscRing`. For each event, it allocates a free `Voice` from a `VoicePool`.

5.  **Voice Activation:** The `noteOn()` method of the allocated `Voice` is called with the parameters from the paint event (frequency, amplitude, partials, pan).

6.  **Synthesis:** The `voicePool->render()` method is called, which sums the output of all active voices into the output audio buffer. The result is a rich, polyphonic texture that directly corresponds to the user's painting gestures.

---

## 4. Integration Strategy for `SpectralCanvasLite`

The `SpectralSynthEngine` is a prime candidate for our first major feature integration after we have a basic STFT pipeline working.

**Proposed Steps:**

1.  **Document Dependencies:** First, create analysis documents for its key dependencies, which include `VoicePool`, `Voice`, and `HarmonicQuantizer`.

2.  **Copy Source Files:** Copy the engine and its dependencies from the `ResearchVault` into our new `Source` directory.

3.  **Instantiate in `SpectralCanvasProProcessor`:**
    *   Add a `SpectralSynthEngine` member variable (as it's a singleton, we would access `SpectralSynthEngine::instance()`).
    *   In `prepareToPlay()`, call `engine.prepare()`.
    *   In `releaseResources()`, call `engine.releaseResources()`.

4.  **Connect UI Events:**
    *   In our `SpectralCanvasProEditor`, the `mouseDown`, `mouseDrag`, and `mouseUp` callbacks will need to be updated.
    *   Instead of just calling `processor.setPaintActive()`, they will create a `PaintEvent` struct with the mouse coordinates and pressure, and pass it to a new method on the processor, like `processor.pushPaintEvent()`.
    *   This new processor method will then call `engine.pushGestureRT()`.

5.  **Integrate into `processBlock()`:**
    *   The `processBlock` method will be modified to call `engine.processAudioBlock()`. Since the engine *generates* sound, this would likely replace any passthrough audio when the synth is active. We will need a parameter to control whether the synth is active or if we are processing live input.

This engine represents the core "creative" feature of the application. By documenting it now, we create a clear roadmap for transforming our simple plugin into the powerful spectral synthesizer it was designed to be.
