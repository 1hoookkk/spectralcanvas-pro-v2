#pragma once
#include <juce_dsp/juce_dsp.h>
#include "../rt/MaskColumnLite.h"
#include "../rt/SpscQueueLite.h"

namespace nodes {

class SpectralPaintNode : public juce::dsp::ProcessorBase {
public:
    SpectralPaintNode() = default;
    ~SpectralPaintNode() = default;

    void prepare (const juce::dsp::ProcessSpec& spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float>& ctx) override;
    void reset() override {}

    bool pushMask (const rt::MaskColumnLite& mc) noexcept { return maskQueue_.try_push(mc); }
    void setPinkBed (bool on) noexcept { enablePinkBed_ = on; }
    void setBrushRadius (int radius) noexcept { (void)radius; } // No-op for now

    // Getters for compatibility
    int getFFTOrder() const noexcept { return fftOrder_; }
    size_t getFFTSize() const noexcept { return fftSize_; }
    int getHopSize() const noexcept { return hop_; }
    int getNumBins() const noexcept { return numBins_; }
    size_t getMaskQueueDepth() const noexcept { return maskQueue_.size(); }
    size_t getMaskDropCount() const noexcept { return maskQueue_.get_drop_count(); }

private:
    // config
    double sampleRate_ = 48000.0;
    int    fftOrder_   = 8;            // 256
    size_t fftSize_    = 256;
    int    numBins_    = 128;
    int    hop_        = 64;

    // buffers
    std::unique_ptr<juce::dsp::FFT> fft_;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> window_;
    std::vector<float> timeBuffer_, freqBuffer_, overlapBuffer_, normBuffer_, sqrtWin_;
    int writeIndex_ = 0;

    // pink bed
    bool enablePinkBed_ = false;
    std::vector<float> bed_; bool bedReady_ = false;
    juce::Random bedRng_{ 0xC0FFEE };

    // queue
    rt::SpscQueueLite<rt::MaskColumnLite, 128> maskQueue_;

    // helpers
    int   mapYToBin(float y) const noexcept;
    void  applyRadialBoost(float* F, int center, float amt, int radius) noexcept;
    void  buildPinkBed(float dBFS);
    void  addPinkBed(float* F) noexcept;
    inline int  reIdx(int bin) const noexcept;
    inline int  imIdx(int bin) const noexcept;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralPaintNode)
};

} // namespace nodes