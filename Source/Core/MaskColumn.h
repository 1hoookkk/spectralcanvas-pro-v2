#pragma once
#include <cstdint>

// Trivially-copyable paint event for Effect mode.
// y_position: 0..1 (top..bottom) for UI reference (not used directly here).
// intensity:  0..1 strength of brush.
// startBin/endBin: spectral bin range (inclusive) to attenuate.
struct MaskColumn
{
    float y_position = 0.5f;
    float intensity  = 0.0f;
    int   startBin   = 0;
    int   endBin     = 0;
};
static_assert(std::is_trivially_copyable_v<MaskColumn>, "MaskColumn must be trivially copyable");
