# Technical Analysis: Psychoacoustic Masking Engine

**Source Location:** `C:\spectralCanvasPro\ResearchVault\Psychoacoustics\`
**Key Files:** `PsychoacousticMaskingEngine.h`, `PsychoacousticMaskingEngine.cpp`

This document provides an architectural overview of the `PsychoacousticMaskingEngine` for the purpose of integration into the new `SpectralCanvasPro` foundation.

---

## 1. Overview & Purpose

The `PsychoacousticMaskingEngine` is a real-time, thread-safe C++ component that calculates the threshold of human hearing for a given audio spectrum. Its primary function is to determine, on a per-frequency-bin basis, the level below which a sound will be masked (i.e., made inaudible) by louder sounds nearby in the spectrum.

This is the core innovation of the original project. It allows for "perceptually-aware" audio processing, where edits (like spectral painting) can be automatically constrained to only make changes that are actually audible to the user, preventing artifacts and enabling a more intuitive workflow.

---

## 2. Core API

The engine has a simple and clean public interface designed for real-time use:

```cpp
class PsychoacousticMaskingEngine {
public:
    // 1. One-time setup on the audio thread
    void prepare(const PsychoacousticMasking::PsychoConfig& config);

    // 2. Real-time processing per audio frame (e.g., in processBlock)
    void computeThreshold(std::span<const float> magnitudeSpectrum, 
                          std::span<float> outputThresholds) noexcept;

    // 3. (Optional) Real-time validation of proposed edits
    void jndClampBins(std::span<const float> magnitudeSpectrum, 
                      std::span<const float> calculatedThresholds, 
                      std::span<float> proposedGains_io) const noexcept;
    
    // 4. Real-time parameter tuning
    void setRuntimeTuning(float marginDb, float strictnessC) noexcept;
};
```

### Configuration (`PsychoConfig` struct)
The engine is configured via a `PsychoConfig` struct passed to the `prepare()` method. Key parameters include:
- `fftBins`: The number of frequency bins in the input spectrum (e.g., 513 for a 1024-point FFT).
- `bands`: The number of psychoacoustic bands to use (defaults to 24 for Bark scale).
- `sampleRate`: The project's sample rate.
- `alpha`: A compression factor for the magnitude spectrum.
- `strictnessC` & `marginDb`: Tuning parameters for the JND (Just Noticeable Difference) calculation.

---

## 3. Mathematical Pipeline (The "Secret Sauce")

The engine's core logic is a five-stage mathematical pipeline that is explicitly "locked" in the source code, indicating it is a proven and non-negotiable formula. The process is as follows:

1.  **Compress:** The input magnitude spectrum is compressed using an alpha-law (`|X|^α`) to better match the non-linear response of the human ear.
2.  **Pool:** The compressed spectrum is pooled into a smaller number of psychoacoustic bands (e.g., 24 Bark bands) using a pre-computed matrix multiplication.
3.  **Spread:** A "spreading function" is applied. This is a critical step that simulates how a loud sound at one frequency makes it harder to hear nearby frequencies. The function is asymmetric, modeling the fact that masking is stronger for higher frequencies.
4.  **Expand:** The spread band energies are combined with the Absolute Threshold of Hearing (ATH) and expanded back out of the compressed domain.
5.  **Back-project:** The final thresholds for the psychoacoustic bands are projected back onto the original high-resolution frequency spectrum.

The output is a **per-bin masking threshold**, representing the quietest sound that can be heard at each specific frequency.

---

## 4. Integration Strategy for `SpectralCanvasLite`

This engine is well-suited for integration into our new, clean codebase. It is self-contained, has no external dependencies beyond the standard library, and is explicitly designed for real-time safety.

**Proposed Steps:**

1.  **Copy Source Files:** Copy the following files from the `ResearchVault` into our new `Source` directory:
    *   `PsychoacousticMaskingEngine.h`
    *   `PsychoacousticMaskingEngine.cpp`
    *   `PsychoacousticBandsPrecompute.h` (a dependency)
    *   `PsychoacousticBandsPrecompute.cpp` (a dependency)

2.  **Instantiate in `SpectralCanvasProProcessor`:**
    *   Add a `PsychoacousticMaskingEngine` member variable to our `SpectralCanvasProProcessor` class.
    *   In `prepareToPlay()`, call the engine's `prepare()` method with the correct configuration (FFT size, sample rate, etc.).

3.  **Integrate into `processBlock()`:**
    *   Our current `processBlock` just ducks the audio. We will need to replace this with a proper STFT (Short-Time Fourier Transform) process.
    *   Inside the STFT loop, for each frame of audio, we will:
        a. Perform an FFT to get the magnitude spectrum.
        b. Pass this spectrum to `engine.computeThreshold()`.
        c. Use the resulting threshold to modify the audio (e.g., by applying spectral painting gains that have been validated by `engine.jndClampBins()`).
        d. Perform an inverse FFT to convert the modified spectrum back to audio.

4.  **Connect to UI:**
    *   Expose the `marginDb` and `strictnessC` parameters from the engine as JUCE parameters, allowing the user to control the "strength" of the psychoacoustic effect from the UI.

This component is the most valuable piece of IP in the vault. Documenting and integrating it first will be a massive step toward realizing the original vision of the project on a new, stable foundation.
