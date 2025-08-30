#pragma once
#include <cstdint>

// UI -> Audio: compact, RT‑safe gesture description.
// Values are normalized to 0..1 unless stated.
struct GestureEvent
{
    float y01 = 0.5f;         // vertical position (0 = top/high freq, 1 = bottom/low freq)
    float intensity01 = 1.0f; // stroke intensity (speed/pressure mapped to 0..1)
    float radius01 = 0.1f;    // brush size in normalized "octaves" span (approximate)
    uint32_t timestampMs = 0; // Time::getMillisecondCounter()
};
