#pragma once
#include <cstdint>
#include "../Core/AtlasIds.h"
#include "../Core/MessageBus.h"   // MaskDeltaQueue, MaskColumnDelta
#include "../Core/MaskAtlas.h"

class HopScheduler
{
public:
    void prepare(double sampleRate)
    {
        sr_ = sampleRate;
        hop_ = AtlasConfig::HOP_SIZE;
        samplePos_ = 0;
        hopIdx_ = 0;
        mask_.prepare();
    }

    void setQueue(MaskDeltaQueue* q) noexcept { q_ = q; }

    inline void drainAndApply(uint32_t maxDeltas = 16) noexcept
    {
        if (!q_) { drains_ = 0; return; }
        uint32_t applied = 0;
        MaskColumnDelta d{};
        while (applied < maxDeltas && q_->pop(d))
        {
            if ((unsigned)d.position.columnInTile >= (unsigned)AtlasConfig::TILE_WIDTH) continue;
            mask_.writeStagingColumn((int)d.position.columnInTile, d.values);
            ++applied;
        }
        if (applied) mask_.flip();
        drains_ = applied;
    }

    inline void advance(uint32_t numSamples) noexcept
    {
        samplePos_ += numSamples;
        hopIdx_ = samplePos_ / hop_;
    }

    inline int currentColInTile() const noexcept
    {
        return (int)(hopIdx_ % AtlasConfig::TILE_WIDTH);
    }

    inline const float* activeMaskCol(int colInTile) const noexcept
    { return mask_.activeColumn(colInTile); }

    inline uint32_t drainsPerBlock() const noexcept { return drains_; }
    inline uint64_t currentHop() const noexcept { return hopIdx_; }

private:
    double sr_{48000.0};
    uint32_t hop_{256};
    uint64_t samplePos_{0};
    uint64_t hopIdx_{0};

    MaskDeltaQueue* q_{nullptr};
    MaskAtlas mask_{};
    uint32_t drains_{0};
};