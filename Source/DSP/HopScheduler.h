#pragma once
#include <cstdint>
#include "../Core/AtlasIds.h"
#include "../Core/MessageBus.h"   // MaskDeltaQueue, MaskColumnDelta
#include "../Core/MaskAtlas.h"
#include "../Core/TiledAtlas.h"

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
    void setAtlas(TiledAtlas* atlas, AtlasPageHandle staging) noexcept { atlas_ = atlas; stagingHandle_ = staging; activeHandle_ = {}; }
    void setAtlasUpdateQueue(AtlasUpdateQueue* q) noexcept { atlasUpdateQ_ = q; }

    inline void drainAndApply(uint32_t maxDeltas = 16) noexcept
    {
        if (!q_) { drains_ = 0; return; }
        uint32_t applied = 0;
        MaskColumnDelta d{};
        while (applied < maxDeltas && q_->pop(d))
        {
            if ((unsigned)d.position.columnInTile >= (unsigned)AtlasConfig::TILE_WIDTH) continue;
            mask_.writeStagingColumn((int)d.position.columnInTile, d.values);

            // Mirror into tiled atlas staging page for GPU/UI
            if (atlas_ && stagingHandle_.isValid())
            {
                AtlasPosition pos = d.position;
                pos.binStart = 0;
                atlas_->writeColumn(stagingHandle_, pos, d.values, AtlasConfig::NUM_BINS);
            }
            ++applied;
        }
        if (applied)
        {
            mask_.flip();
            if (atlas_ && stagingHandle_.isValid())
            {
                // Flip tiled atlas page and notify UI
                atlas_->activatePage(stagingHandle_);
                // Swap handles (simple ping-pong)
                if (!activeHandle_.isValid()) activeHandle_ = stagingHandle_;
                auto oldActive = activeHandle_;
                activeHandle_ = stagingHandle_;
                stagingHandle_ = oldActive.isValid() ? oldActive : atlas_->allocateFreePage();
                if (atlasUpdateQ_)
                {
                    AtlasUpdate u{};
                    u.activePageHandle = {};
                    u.pendingPageHandle = activeHandle_;
                    atlasUpdateQ_->push(u);
                }
            }
        }
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
    AtlasUpdateQueue* atlasUpdateQ_{nullptr};
    MaskAtlas mask_{};
    uint32_t drains_{0};

    // Tiled atlas mirroring
    TiledAtlas* atlas_{nullptr};
    AtlasPageHandle stagingHandle_{};
    AtlasPageHandle activeHandle_{};
};
