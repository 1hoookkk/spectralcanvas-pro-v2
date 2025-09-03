#pragma once
#include <cstdint>
#include <atomic>
#include <type_traits>

/**
 * Handle-based IDs for tiled atlas system.
 * All types are POD/trivially copyable for RT-safe communication.
 */

// Atlas page handle for atomic page flipping
struct AtlasPageHandle {
    uint32_t id = 0;           // Page identifier
    uint32_t generation = 0;   // Generation counter for ABA protection
    
    bool isValid() const noexcept { return id != 0; }
    bool operator==(const AtlasPageHandle& other) const noexcept {
        return id == other.id && generation == other.generation;
    }
};

// Column position within atlas
struct AtlasPosition {
    uint16_t tileX = 0;        // Tile X coordinate (0-based)
    uint16_t tileY = 0;        // Tile Y coordinate (0-based)
    uint16_t columnInTile = 0; // Column within tile (0-511 for 512-wide tiles)
    uint16_t binStart = 0;     // Starting frequency bin
    
    bool isValid() const noexcept { return tileX < 4 && tileY < 1 && columnInTile < 512; }
};

// Spectral column metadata
struct ColumnMetadata {
    uint64_t samplePosition = 0;    // Sample position in source audio
    float timeSeconds = 0.0f;       // Time position in seconds
    float fundamentalHz = 0.0f;     // Detected fundamental frequency
    float spectralCentroid = 0.0f;  // Spectral centroid
    uint32_t sequenceNumber = 0;    // Ordering sequence
};

// Combined atlas update message
struct AtlasUpdate {
    AtlasPageHandle activePageHandle;   // Currently active page
    AtlasPageHandle pendingPageHandle;  // Page being prepared
    AtlasPosition position;             // Where to write column
    ColumnMetadata metadata;            // Column metadata
    
    bool isPageFlip() const noexcept {
        return pendingPageHandle.isValid() && !activePageHandle.isValid();
    }
    
    bool isColumnUpdate() const noexcept {
        return activePageHandle.isValid() && position.isValid();
    }
};

// RT safety validation
static_assert(std::is_trivially_copyable_v<AtlasPageHandle>, "AtlasPageHandle must be trivially copyable");
static_assert(std::is_trivially_copyable_v<AtlasPosition>, "AtlasPosition must be trivially copyable");
static_assert(std::is_trivially_copyable_v<ColumnMetadata>, "ColumnMetadata must be trivially copyable");
static_assert(std::is_trivially_copyable_v<AtlasUpdate>, "AtlasUpdate must be trivially copyable");

// Constants for atlas configuration
namespace AtlasConfig {
    static constexpr size_t TILE_WIDTH = 512;      // Columns per tile
    static constexpr size_t TILE_HEIGHT = 257;     // Frequency bins per tile (FFT_SIZE/2+1)
    static constexpr size_t ATLAS_WIDTH = 2048;    // Total atlas width (4 tiles)
    static constexpr size_t ATLAS_HEIGHT = 512;    // Total atlas height (2 pages)
    static constexpr size_t NUM_TILES_X = ATLAS_WIDTH / TILE_WIDTH;  // 4 tiles horizontally
    static constexpr size_t NUM_TILES_Y = ATLAS_HEIGHT / TILE_HEIGHT; // 2 tiles vertically
    static constexpr size_t NUM_PAGES = 2;          // Double buffering
    
    // Single source of truth: All spectral engines must match this
    static constexpr size_t FFT_SIZE = 512;
    static constexpr size_t NUM_BINS = FFT_SIZE / 2 + 1;  // 257
    static constexpr size_t HOP_SIZE = 128;
}

// Hard guards to prevent bins/columns mismatch heap corruption
static_assert(AtlasConfig::NUM_BINS == AtlasConfig::TILE_HEIGHT, "NUM_BINS must equal TILE_HEIGHT to prevent heap corruption");
static_assert(AtlasConfig::TILE_HEIGHT == 257, "TILE_HEIGHT must be 257 (hardcoded in MaskColumnDelta)");
static_assert(AtlasConfig::FFT_SIZE % 2 == 0, "FFT_SIZE must be even");
static_assert(AtlasConfig::TILE_WIDTH <= 4096, "TILE_WIDTH sanity check");
static_assert(AtlasConfig::NUM_BINS <= 4096, "NUM_BINS sanity check");