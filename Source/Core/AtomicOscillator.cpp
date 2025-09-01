#include "AtomicOscillator.h"
#include <cmath>

OscillatorBankPool<256>& getOscillatorBankPool() noexcept
{
    static OscillatorBankPool<256> instance;
    return instance;
}

void AtomicOscillator::prepareToPlay(double sampleRate) noexcept
{
    sampleRate_ = sampleRate;
    phase_ = 0.0;
    phaseIncrement_ = 0.0;
    
    smoothedAmplitude_ = params_.getAmplitude();
    smoothedPan_ = params_.getPan();
    smoothedFilterCutoff_ = params_.getFilterCutoff();
    smoothedResonance_ = params_.getResonance();
}

void AtomicOscillator::processBlock(juce::AudioBuffer<float>& buffer) noexcept
{
    AtomicOscScopedNoDenormals denormalGuard;
    
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    
    // Update parameters with smoothing
    updateSmoothingParameters();
    
    // Calculate phase increment
    float frequency = params_.getFrequency();
    phaseIncrement_ = frequency / sampleRate_;
    
    // Generate samples
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float oscillatorSample = generateSample();
        float filteredSample = applyFilter(oscillatorSample);
        
        // Apply amplitude and pan
        float amplifiedSample = filteredSample * smoothedAmplitude_;
        
        // Write to channels with panning
        if (numChannels >= 2)
        {
            float leftGain = std::sqrt(1.0f - smoothedPan_);
            float rightGain = std::sqrt(smoothedPan_);
            
            buffer.addSample(0, sample, amplifiedSample * leftGain);
            buffer.addSample(1, sample, amplifiedSample * rightGain);
        }
        else if (numChannels == 1)
        {
            buffer.addSample(0, sample, amplifiedSample);
        }
        
        // Update phase
        phase_ += phaseIncrement_;
        if (phase_ >= 1.0)
            phase_ -= 1.0;
    }
}

void AtomicOscillator::reset() noexcept
{
    phase_ = 0.0;
    phaseIncrement_ = 0.0;
    smoothedAmplitude_ = 0.0f;
    smoothedPan_ = 0.5f;
    smoothedFilterCutoff_ = 1.0f;
    smoothedResonance_ = 0.0f;
    
    // Reset parameters to defaults
    params_.setFrequency(440.0f);
    params_.setAmplitude(0.0f);
    params_.setTargetAmplitude(0.0f);
    params_.setPan(0.5f);
    params_.setTargetPan(0.5f);
    params_.setFilterCutoff(1.0f);
    params_.setResonance(0.0f);
    params_.setModDepth(0.0f);
}

void AtomicOscillator::updateParametersFromColorMapper(float frequency, float amplitude, float pan, 
                                                     float filterCutoff, float resonance) noexcept
{
    params_.setFrequency(frequency);
    params_.setTargetAmplitude(amplitude);
    params_.setTargetPan(pan);
    params_.setFilterCutoff(filterCutoff);
    params_.setResonance(resonance);
}

void AtomicOscillator::updateSmoothingParameters() noexcept
{
    // Smooth amplitude
    float targetAmplitude = params_.getTargetAmplitude();
    smoothedAmplitude_ += (targetAmplitude - smoothedAmplitude_) * kSmoothingCoeff;
    params_.setAmplitude(smoothedAmplitude_);
    
    // Smooth pan
    float targetPan = params_.getTargetPan();
    smoothedPan_ += (targetPan - smoothedPan_) * kSmoothingCoeff;
    params_.setPan(smoothedPan_);
    
    // Smooth filter parameters
    float targetCutoff = params_.getFilterCutoff();
    smoothedFilterCutoff_ += (targetCutoff - smoothedFilterCutoff_) * kFastSmoothingCoeff;
    
    float targetResonance = params_.getResonance();
    smoothedResonance_ += (targetResonance - smoothedResonance_) * kFastSmoothingCoeff;
}

float AtomicOscillator::generateSample() noexcept
{
    // Simple sine wave oscillator
    return std::sin(2.0 * juce::MathConstants<double>::pi * phase_);
}

float AtomicOscillator::applyFilter(float sample) noexcept
{
    // Simple one-pole low-pass filter
    static float lastSample = 0.0f;
    
    float cutoff = juce::jlimit(0.001f, 0.999f, smoothedFilterCutoff_);
    float filtered = lastSample + (sample - lastSample) * cutoff;
    
    // Add resonance
    if (smoothedResonance_ > 0.0f)
    {
        filtered += (sample - filtered) * smoothedResonance_ * 0.5f;
    }
    
    lastSample = filtered;
    return filtered;
}