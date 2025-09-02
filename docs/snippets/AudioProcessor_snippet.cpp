// RT audio thread snippet: drain queues, play sample, apply paint/params
#include "RealtimeMemoryManager.h"
#include "MessageBus.h"
#include "SampleMessage.h"
#include <optional>
// Minimal pseudo JUCE-like interfaces for illustration:
template<typename T> struct AudioBuffer {
    int getNumSamples() const; int getNumChannels() const;
    void clear(int ch, int start, int num);
    T getSample(int ch, int idx) const;
    void setSample(int ch, int idx, T v);
};
struct MidiBuffer {};

struct AudioProcessor {
    RealtimeMemoryManager& rtmm;
    SampleQueue& sampleQueue;
    MaskColumnQueue& maskQueue;
    ParameterQueue& parameterQueue;
    uint64_t lastPaintToAudioMicros = 0;

    std::optional<SampleView> currentSample;
    uint32_t currentHandle = 0;
    uint64_t samplePos = 0; // frames

    uint64_t nowMicros() const; // provide your RT-safe time source (optional)

    void drainMaskColumnsAndApply();  // implemented in your engine
    void applyParameterQueue();       // implemented in your engine

    void processBlock(AudioBuffer<float>& out, MidiBuffer&) {
        // 1) Drain SampleQueue
        SampleMessage msg;
        while (sampleQueue.pop(msg)) {
            if (msg.handle != 0) {
                auto v = rtmm.samples.lookup(msg.handle);
                if (v) {
                    currentSample = v;
                    currentHandle = msg.handle;
                    samplePos = 0;
                    if (msg.tscOrUsec) lastPaintToAudioMicros = nowMicros() - msg.tscOrUsec;
                }
            }
        }

        // 2) Parameters & mask events
        applyParameterQueue();
        drainMaskColumnsAndApply();

        // 3) Render (block-accurate)
        const int numSamples = out.getNumSamples();
        const int numOutCh   = out.getNumChannels();
        for (int ch=0; ch<numOutCh; ++ch) out.clear(ch, 0, numSamples);

        if (currentSample) {
            const auto& sv = *currentSample;
            if (sv.channels && sv.numFrames > 0) {
                for (int n=0; n<numSamples; ++n) {
                    const uint64_t p = samplePos;
                    float s = 0.f;
                    if (p < sv.numFrames) {
                        for (uint32_t ch=0; ch<sv.numChannels; ++ch)
                            s += sv.channels[ch][p];
                        s /= float(sv.numChannels ? sv.numChannels : 1);
                    }
                    for (int ch=0; ch<numOutCh; ++ch)
                        out.setSample(ch, n, out.getSample(ch, n) + s);
                    if (++samplePos >= sv.numFrames) {
                        currentSample.reset(); // stop (no loop)
                        break;
                    }
                }
            }
        }
    }
};