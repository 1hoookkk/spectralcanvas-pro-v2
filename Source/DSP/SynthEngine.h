#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "../Core/RealtimeSafeTypes.h"
#include <array>
#include <atomic>
#include <cmath>

/**
 * Always-on synthesis engine (AdditivePad + NoiseExciter)
 * Drives the living nebula when no sample is loaded
 * Based on AtomicOscillator pattern from ARTEFACT_Production
 */
class SynthEngine
{
public:
    static constexpr size_t MAX_PARTIALS = 64;
    static constexpr size_t MAX_VOICES = 8;
    
    SynthEngine() noexcept;
    ~SynthEngine() noexcept = default;
    
    // Non-copyable for RT safety
    SynthEngine(const SynthEngine&) = delete;
    SynthEngine& operator=(const SynthEngine&) = delete;
    
    // Initialization (called from prepareToPlay - may allocate)
    void initialize(double sampleRate, int maxBlockSize);
    void reset() noexcept;
    
    // RT-safe processing (audio thread only)
    void processBlock(juce::AudioBuffer<float>& buffer, 
                     const juce::MidiBuffer& midiMessages) noexcept;
    
    // RT-safe parameter updates (any thread)
    void setBrightness(float value) noexcept { brightness_.store(value, std::memory_order_release); }
    void setSpread(float value) noexcept { spread_.store(value, std::memory_order_release); }
    void setDrift(float value) noexcept { driftAmount_.store(value, std::memory_order_release); }
    void setNoiseAmount(float value) noexcept { noiseAmount_.store(value, std::memory_order_release); }
    void setFilterCutoff(float hz) noexcept { filterCutoff_.store(hz, std::memory_order_release); }
    void setFilterResonance(float q) noexcept { filterResonance_.store(q, std::memory_order_release); }
    
    // Get current spectral data for visualization
    void getSpectralMagnitudes(float* magnitudes, size_t numBins) const noexcept;
    
private:
    // Partial oscillator structure
    struct Partial
    {
        float phase = 0.0f;
        float phaseIncrement = 0.0f;
        float amplitude = 0.0f;
        float targetAmplitude = 0.0f;
        float pan = 0.5f;
        
        // LFO for slow drift
        float lfoPhase = 0.0f;
        float lfoRate = 0.0f;
        float lfoDepth = 0.0f;
    };
    
    // Voice structure for polyphony
    struct Voice
    {
        std::array<Partial, MAX_PARTIALS> partials;
        float fundamentalFreq = 0.0f;
        float amplitude = 0.0f;
        float targetAmplitude = 0.0f;
        int noteNumber = -1;
        bool active = false;
        
        // ADSR envelope
        float envLevel = 0.0f;
        float attackRate = 0.0f;
        float decayRate = 0.0f;
        float sustainLevel = 0.7f;
        float releaseRate = 0.0f;
        int envStage = 0; // 0=off, 1=attack, 2=decay, 3=sustain, 4=release
    };
    
    // Voices array
    std::array<Voice, MAX_VOICES> voices_;
    
    // Noise exciter state
    struct NoiseExciter
    {
        juce::Random random;
        float lastSample = 0.0f;
        
        // Band-limited noise filter
        float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f;
        float a1 = 0.0f, a2 = 0.0f;
        float z1L = 0.0f, z2L = 0.0f;
        float z1R = 0.0f, z2R = 0.0f;
    } noiseExciter_;
    
    // Global parameters (atomics for thread safety)
    std::atomic<float> brightness_{0.5f};       // Spectral centroid control
    std::atomic<float> spread_{0.3f};          // Harmonic spread
    std::atomic<float> driftAmount_{0.1f};     // LFO drift amount
    std::atomic<float> noiseAmount_{0.05f};    // Noise mix level
    std::atomic<float> filterCutoff_{2000.0f}; // Global filter cutoff
    std::atomic<float> filterResonance_{1.0f}; // Global filter resonance
    
    // Processing state
    double sampleRate_ = 44100.0;
    double invSampleRate_ = 1.0 / 44100.0;
    float smoothingCoeff_ = 0.995f;
    
    // Helper methods
    void updatePartialFrequencies(Voice& voice) noexcept;
    void processVoice(Voice& voice, float* leftOut, float* rightOut, int numSamples) noexcept;
    void processNoiseExciter(float* leftOut, float* rightOut, int numSamples) noexcept;
    void updateNoiseFilter() noexcept;
    
    // MIDI handling
    void handleMidiMessage(const juce::MidiMessage& message) noexcept;
    void noteOn(int noteNumber, float velocity) noexcept;
    void noteOff(int noteNumber) noexcept;
    Voice* findFreeVoice() noexcept;
    Voice* findVoiceForNote(int noteNumber) noexcept;
    
    // Fast math helpers
    inline float fastSin(float x) noexcept
    {
        // Use phasor → sine approximation
        x = x - std::floor(x); // Wrap to 0-1
        x = x * 4.0f - 2.0f;   // Scale to -2 to 2
        return x * (2.0f - std::abs(x)); // Parabolic approximation
    }
    
    inline float mtof(float midiNote) noexcept
    {
        return 440.0f * std::pow(2.0f, (midiNote - 69.0f) / 12.0f);
    }
};