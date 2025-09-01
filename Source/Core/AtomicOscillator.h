#pragma once
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <atomic>
#include <array>
#include <algorithm>
#include <immintrin.h>

/**
 * Lock-Free Atomic Oscillator for Real-Time Audio Processing
 * 
 * Thread-safe oscillator with atomic parameter updates:
 * - No synchronization on parameter modifications
 * - Lock-free parameter updates using atomic operations
 * - Cache-friendly memory layout for SIMD optimization
 * - Smooth parameter interpolation
 */

class AtomicOscScopedNoDenormals
{
public:
    AtomicOscScopedNoDenormals() noexcept
    {
#if defined(_MSC_VER) || defined(__GNUC__)
        oldState_ = _mm_getcsr();
        _mm_setcsr(oldState_ | 0x8040); // Set FTZ and DAZ bits
#endif
    }
    
    ~AtomicOscScopedNoDenormals() noexcept
    {
#if defined(_MSC_VER) || defined(__GNUC__)
        _mm_setcsr(oldState_);
#endif
    }
    
private:
    unsigned int oldState_;
};

struct AtomicOscillatorParams
{
    std::atomic<float> frequency{440.0f};
    std::atomic<float> amplitude{0.0f};
    std::atomic<float> targetAmplitude{0.0f};
    std::atomic<float> pan{0.5f};
    std::atomic<float> targetPan{0.5f};
    
    // Extended parameters for spectral synthesis
    std::atomic<float> filterCutoff{1.0f};
    std::atomic<float> resonance{0.0f};
    std::atomic<float> modDepth{0.0f};
    
    AtomicOscillatorParams(const AtomicOscillatorParams&) = delete;
    AtomicOscillatorParams& operator=(const AtomicOscillatorParams&) = delete;
    
    AtomicOscillatorParams() = default;
    AtomicOscillatorParams(AtomicOscillatorParams&& other) noexcept
    {
        frequency.store(other.frequency.load());
        amplitude.store(other.amplitude.load());
        targetAmplitude.store(other.targetAmplitude.load());
        pan.store(other.pan.load());
        targetPan.store(other.targetPan.load());
        filterCutoff.store(other.filterCutoff.load());
        resonance.store(other.resonance.load());
        modDepth.store(other.modDepth.load());
    }
    
    AtomicOscillatorParams& operator=(AtomicOscillatorParams&& other) noexcept
    {
        if (this != &other)
        {
            frequency.store(other.frequency.load());
            amplitude.store(other.amplitude.load());
            targetAmplitude.store(other.targetAmplitude.load());
            pan.store(other.pan.load());
            targetPan.store(other.targetPan.load());
            filterCutoff.store(other.filterCutoff.load());
            resonance.store(other.resonance.load());
            modDepth.store(other.modDepth.load());
        }
        return *this;
    }
    
    void setFrequency(float freq) noexcept { frequency.store(freq, std::memory_order_relaxed); }
    void setAmplitude(float amp) noexcept { amplitude.store(amp, std::memory_order_relaxed); }
    void setTargetAmplitude(float amp) noexcept { targetAmplitude.store(amp, std::memory_order_relaxed); }
    void setPan(float p) noexcept { pan.store(p, std::memory_order_relaxed); }
    void setTargetPan(float p) noexcept { targetPan.store(p, std::memory_order_relaxed); }
    void setFilterCutoff(float cutoff) noexcept { filterCutoff.store(cutoff, std::memory_order_relaxed); }
    void setResonance(float res) noexcept { resonance.store(res, std::memory_order_relaxed); }
    void setModDepth(float depth) noexcept { modDepth.store(depth, std::memory_order_relaxed); }
    
    float getFrequency() const noexcept { return frequency.load(std::memory_order_relaxed); }
    float getAmplitude() const noexcept { return amplitude.load(std::memory_order_relaxed); }
    float getTargetAmplitude() const noexcept { return targetAmplitude.load(std::memory_order_relaxed); }
    float getPan() const noexcept { return pan.load(std::memory_order_relaxed); }
    float getTargetPan() const noexcept { return targetPan.load(std::memory_order_relaxed); }
    float getFilterCutoff() const noexcept { return filterCutoff.load(std::memory_order_relaxed); }
    float getResonance() const noexcept { return resonance.load(std::memory_order_relaxed); }
    float getModDepth() const noexcept { return modDepth.load(std::memory_order_relaxed); }
};

class AtomicOscillator
{
public:
    AtomicOscillator() = default;
    ~AtomicOscillator() = default;
    
    void prepareToPlay(double sampleRate) noexcept;
    void processBlock(juce::AudioBuffer<float>& buffer) noexcept;
    void reset() noexcept;
    
    // Thread-safe parameter updates
    AtomicOscillatorParams& getParams() noexcept { return params_; }
    const AtomicOscillatorParams& getParams() const noexcept { return params_; }
    
    // Real-time safe parameter updates
    void updateParametersFromColorMapper(float frequency, float amplitude, float pan, 
                                       float filterCutoff, float resonance) noexcept;
    
private:
    AtomicOscillatorParams params_;
    
    // Oscillator state (audio thread only)
    double phase_ = 0.0;
    double phaseIncrement_ = 0.0;
    double sampleRate_ = 44100.0;
    
    // Parameter smoothing (audio thread only)
    float smoothedAmplitude_ = 0.0f;
    float smoothedPan_ = 0.5f;
    float smoothedFilterCutoff_ = 1.0f;
    float smoothedResonance_ = 0.0f;
    
    // Smoothing coefficients
    static constexpr float kSmoothingCoeff = 0.001f;
    static constexpr float kFastSmoothingCoeff = 0.01f;
    
    // Helper methods
    void updateSmoothingParameters() noexcept;
    float generateSample() noexcept;
    float applyFilter(float sample) noexcept;
};

// Template class for managing pools of atomic oscillators
template<size_t PoolSize = 256>
class OscillatorBankPool
{
public:
    OscillatorBankPool()
    {
        for (size_t i = 0; i < PoolSize; ++i)
        {
            freeList_[i] = i;
        }
        nextFreeIndex_.store(0, std::memory_order_relaxed);
    }
    
    AtomicOscillator* acquire() noexcept
    {
        int freeIndex = nextFreeIndex_.fetch_add(1, std::memory_order_acquire);
        if (freeIndex >= static_cast<int>(PoolSize))
        {
            nextFreeIndex_.fetch_sub(1, std::memory_order_acq_rel);
            return nullptr;
        }
        
        size_t oscillatorIndex = freeList_[freeIndex];
        return &oscillators_[oscillatorIndex];
    }
    
    void release(AtomicOscillator* oscillator) noexcept
    {
        if (!oscillator) return;
        
        size_t oscillatorIndex = oscillator - oscillators_.data();
        if (oscillatorIndex >= PoolSize) return;
        
        oscillator->reset();
        
        int freeIndex = nextFreeIndex_.fetch_sub(1, std::memory_order_acq_rel) - 1;
        if (freeIndex >= 0)
        {
            freeList_[freeIndex] = oscillatorIndex;
        }
    }
    
    size_t getActiveOscillatorCount() const noexcept
    {
        return std::max(0, static_cast<int>(nextFreeIndex_.load(std::memory_order_relaxed)));
    }
    
    void prepareToPlay(double sampleRate) noexcept
    {
        for (auto& osc : oscillators_)
        {
            osc.prepareToPlay(sampleRate);
        }
    }
    
private:
    std::array<AtomicOscillator, PoolSize> oscillators_;
    std::array<size_t, PoolSize> freeList_;
    std::atomic<int> nextFreeIndex_{0};
};

// Global oscillator bank pool access
OscillatorBankPool<256>& getOscillatorBankPool() noexcept;