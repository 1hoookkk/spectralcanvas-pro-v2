#pragma once
#include <atomic>
#include <array>
#include <memory>
#include <cmath>
#include <cstring>

namespace dsp {

enum class ScaleType {
    Chromatic = 0,
    Major = 1,
    NaturalMinor = 2
};

struct KeyFilterLUT {
    std::unique_ptr<std::uint8_t[]> mask;  // 0 or 1 per bin
    int numBins = 0;
    int fftSize = 0;
    double sampleRate = 0.0;
    
    KeyFilterLUT() = default;
    KeyFilterLUT(int numBins) : numBins(numBins) {
        mask = std::make_unique<std::uint8_t[]>(numBins);
        std::memset(mask.get(), 1, numBins);  // Default: pass all
    }
};

class KeyFilter {
public:
    KeyFilter() = default;
    ~KeyFilter() = default;
    
    // Initialize with audio parameters
    void prepare(int fftSize, int numBins, double sampleRate);
    
    // Build new LUT off audio thread and swap atomically
    void rebuildAsync(int rootPc, ScaleType scale);
    
    // Audio-thread safe: multiplies magnitudes by 0/1 in-place
    void apply(float* magnitudes, int numBins) const noexcept;
    
    // Parameters (atomics) updated by parameter thread
    std::atomic<int> rootPc{0};
    std::atomic<ScaleType> scaleType{ScaleType::Major};
    std::atomic<bool> enabled{true};

private:
    std::atomic<const KeyFilterLUT*> current_{nullptr};
    std::unique_ptr<KeyFilterLUT> lutA_, lutB_;
    
    // Build LUT into target buffer
    void buildInto(KeyFilterLUT& dst, int rootPc, ScaleType scale) const;
    
    // Scale definitions
    static constexpr std::array<int, 7> major_ = {0, 2, 4, 5, 7, 9, 11};
    static constexpr std::array<int, 7> naturalMinor_ = {0, 2, 3, 5, 7, 8, 10};
    
    // Helper to build pitch-class mask
    static void buildScaleMask(std::uint8_t* out12, int rootPc, ScaleType scale);
    
    // Current parameters for rebuild detection
    int currentFftSize_ = 0;
    int currentNumBins_ = 0;
    double currentSampleRate_ = 0.0;
};

}  // namespace dsp