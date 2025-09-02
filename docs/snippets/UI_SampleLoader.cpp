// UI-side sample loader → publish handles to RT
#include "RealtimeMemoryManager.h"
#include "MessageBus.h"
#include "SampleMessage.h"
#include <vector>
#include <cstring>
#include <memory>

// Pseudocode: integrate with your real UI/sample loader.
struct SampleLoader {
    RealtimeMemoryManager& rtmm;
    SampleQueue& sampleQueue;
    // Hold UI-owned memory so SampleView pointers remain valid.
    std::vector<std::unique_ptr<float[]>> ownedPtrs;
    std::vector<const float*> channelPtrs;
    uint64_t nowMicros() const; // provide your UI clock

    void onSampleLoaded(const std::vector<std::vector<float>>& channels, double sr) {
        auto h = rtmm.samples.allocate();
        if (!h) return; // pool full: show UI message or replace policy

        ownedPtrs.resize(channels.size());
        channelPtrs.resize(channels.size());
        for (size_t ch=0; ch<channels.size(); ++ch) {
            ownedPtrs[ch].reset(new float[channels[ch].size()]);
            std::memcpy(ownedPtrs[ch].get(), channels[ch].data(), channels[ch].size()*sizeof(float));
            channelPtrs[ch] = ownedPtrs[ch].get();
        }

        SampleView view;
        view.channels    = channelPtrs.data();
        view.numChannels = static_cast<uint32_t>(channelPtrs.size());
        view.numFrames   = channels.empty()? 0u : static_cast<uint32_t>(channels[0].size());
        view.sampleRate  = sr;

        if (rtmm.samples.publish(*h, view)) {
            SampleMessage msg;
            msg.handle = *h;
            msg.tscOrUsec = nowMicros();
            sampleQueue.push(msg);
        } else {
            rtmm.samples.free(*h);
        }
    }
};