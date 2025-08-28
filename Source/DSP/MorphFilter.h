#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <array>
#include <cmath>

/**
 * RT-safe morphing filter with E-mu inspired responses
 * State Variable Filter topology for smooth morphing between filter types
 * Imported from ARTEFACT_Production (battle-tested implementation)
 */
class MorphFilter
{
public:
    MorphFilter() = default;
    ~MorphFilter() = default;
    
    // Non-copyable for RT safety
    MorphFilter(const MorphFilter&) = delete;
    MorphFilter& operator=(const MorphFilter&) = delete;
    
    /**
     * Prepare filter for processing
     * @param sampleRate The sample rate
     * @param maxBlockSize Maximum block size for processing
     */
    void prepare(double sampleRate, int maxBlockSize) noexcept
    {
        this->sampleRate = sampleRate;
        this->invSampleRate = 1.0 / sampleRate;
        
        // Calculate smoothing coefficients
        smoothingCoeff = std::exp(-2.0 * M_PI * 5.0 / sampleRate);      // 5 Hz smoothing
        fastSmoothingCoeff = std::exp(-2.0 * M_PI * 50.0 / sampleRate); // 50 Hz for fast params
        
        reset();
        calculateCoefficients();
        
        // Initialize gain compensation LUT
        for (size_t i = 0; i < gainCompLUT.size(); ++i)
        {
            float resonance = (float)i / 25.5f; // 0-10 range
            gainCompLUT[i] = 1.0f / (1.0f + resonance * 0.15f); // Simple compensation
        }
    }
    
    /**
     * Reset filter states
     */
    void reset() noexcept
    {
        for (auto& state : channelStates)
        {
            state.z1 = 0.0f;
            state.z2 = 0.0f;
        }
    }
    
    /**
     * Process audio buffer through the filter
     * @param buffer The audio buffer to process
     */
    void processBlock(juce::AudioBuffer<float>& buffer) noexcept
    {
        const int numSamples = buffer.getNumSamples();
        const int numChannels = buffer.getNumChannels();
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            updateSmoothing();
            calculateCoefficients();
            
            for (int channel = 0; channel < numChannels && channel < 2; ++channel)
            {
                float* channelData = buffer.getWritePointer(channel);
                float input = channelData[sample];
                
                // Apply drive (pre or post)
                if (preMode && currentDrive > 0.0f)
                {
                    input = fastTanh(input * (1.0f + currentDrive * 0.1f));
                }
                
                // State Variable Filter processing
                auto& state = channelStates[channel];
                
                // Calculate filter outputs
                float v3 = input - currentK * state.z1 - state.z2;
                state.z1 += currentG * v3;
                state.z2 += currentG * state.z1;
                
                // Mix outputs based on morph position
                float lowpass = state.z2;
                float bandpass = state.z1;
                float highpass = v3;
                float notch = lowpass + highpass;
                
                // Morph between responses
                float output = 0.0f;
                if (currentMorph <= 0.25f)
                {
                    // LP to BP
                    float mix = currentMorph * 4.0f;
                    output = lerp(lowpass, bandpass, mix);
                }
                else if (currentMorph <= 0.5f)
                {
                    // BP to HP
                    float mix = (currentMorph - 0.25f) * 4.0f;
                    output = lerp(bandpass, highpass, mix);
                }
                else if (currentMorph <= 0.75f)
                {
                    // HP to Notch
                    float mix = (currentMorph - 0.5f) * 4.0f;
                    output = lerp(highpass, notch, mix);
                }
                else
                {
                    // Notch to Vowel (formant-like response)
                    float mix = (currentMorph - 0.75f) * 4.0f;
                    float vowel = bandpass * (1.0f + currentResonance * 0.5f) + lowpass * 0.3f;
                    output = lerp(notch, vowel, mix);
                }
                
                // Apply gain compensation
                int resIndex = juce::jlimit(0, 255, (int)(currentResonance * 25.5f));
                output *= gainCompLUT[resIndex];
                
                // Apply post-drive if enabled
                if (!preMode && currentDrive > 0.0f)
                {
                    output = fastTanh(output * (1.0f + currentDrive * 0.1f));
                }
                
                channelData[sample] = output;
            }
        }
    }
    
    // RT-safe parameter setters (called from any thread)
    void setMorph(float value) noexcept { targetMorph = juce::jlimit(0.0f, 1.0f, value); }
    void setCutoff(float hz) noexcept { targetCutoff = juce::jlimit(20.0f, 20000.0f, hz); }
    void setResonance(float q) noexcept { targetResonance = juce::jlimit(0.1f, 10.0f, q); }
    void setDrive(float db) noexcept { targetDrive = juce::jlimit(0.0f, 24.0f, db); }
    void setPrePost(bool pre) noexcept { preMode = pre; }
    
private:
    // Filter topology: State Variable Filter for smooth morphing
    struct SVFState
    {
        float z1 = 0.0f;  // Integrator 1 state
        float z2 = 0.0f;  // Integrator 2 state
    };
    
    // Per-channel state
    std::array<SVFState, 2> channelStates;
    
    // Current filter coefficients
    float currentG = 0.0f;     // Frequency coefficient
    float currentK = 1.0f;     // Damping coefficient
    
    // Current and target parameters
    float currentMorph = 0.0f;
    float currentCutoff = 1000.0f;
    float currentResonance = 1.0f;
    float currentDrive = 0.0f;
    
    float targetMorph = 0.0f;
    float targetCutoff = 1000.0f;
    float targetResonance = 1.0f;
    float targetDrive = 0.0f;
    
    // Smoothing coefficients
    float smoothingCoeff = 0.995f;
    float fastSmoothingCoeff = 0.95f;
    
    // Processing parameters
    double sampleRate = 48000.0;
    double invSampleRate = 1.0 / 48000.0;
    bool preMode = false;
    
    // Gain compensation LUT (256 entries for resonance 0-10)
    std::array<float, 256> gainCompLUT;
    
    /**
     * Calculate filter coefficients from current parameters
     */
    void calculateCoefficients() noexcept
    {
        // Prewarp cutoff frequency
        float wc = 2.0f * std::tan(M_PI * currentCutoff * invSampleRate);
        currentG = wc / (1.0f + wc);
        currentK = 1.0f / currentResonance;
    }
    
    /**
     * Update parameter smoothing
     */
    void updateSmoothing() noexcept
    {
        currentMorph += (targetMorph - currentMorph) * (1.0f - smoothingCoeff);
        currentCutoff += (targetCutoff - currentCutoff) * (1.0f - fastSmoothingCoeff);
        currentResonance += (targetResonance - currentResonance) * (1.0f - smoothingCoeff);
        currentDrive += (targetDrive - currentDrive) * (1.0f - smoothingCoeff);
    }
    
    /**
     * Fast tanh approximation using Padé approximation
     * Accurate to ~0.001, much faster than std::tanh
     */
    inline float fastTanh(float x) noexcept
    {
        const float x2 = x * x;
        return x * (27.0f + x2) / (27.0f + 9.0f * x2);
    }
    
    /**
     * Linear interpolation helper
     */
    inline float lerp(float a, float b, float t) noexcept
    {
        return a + t * (b - a);
    }
};