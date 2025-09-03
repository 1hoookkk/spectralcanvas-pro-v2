#pragma once
#include "../Core/AtlasIds.h"
#include "../Core/MessageBus.h"
#include "../Core/RealtimeSafeTypes.h"
#include <atomic>
#include <cstdint>

/**
 * RT-safe scheduler for applying mask column deltas at precise hop boundaries.
 * Synchronizes with SpectralEngine timing to ensure sample-accurate updates.
 */
class HopScheduler {
public:
    static constexpr size_t HOP_SIZE = 128;  // Must match SpectralEngine::HOP_SIZE
    static constexpr size_t MAX_PENDING_DELTAS = 64;
    
    HopScheduler() noexcept;
    ~HopScheduler() noexcept = default;
    
    // Non-copyable for RT safety
    HopScheduler(const HopScheduler&) = delete;
    HopScheduler& operator=(const HopScheduler&) = delete;
    
    // Initialization (main thread - may allocate)
    bool initialize(double sampleRate, 
                   MaskDeltaQueue& inputQueue,
                   AtlasUpdateQueue& outputQueue) noexcept;
    
    // RT-safe processing (audio thread only)
    void processHop(uint64_t currentSamplePosition, uint32_t hopIndex) noexcept;
    void reset() noexcept;
    
    // Delta scheduling (audio thread only)
    void scheduleDeltas() noexcept;
    void applyScheduledDeltas(uint64_t currentSamplePosition) noexcept;
    
    // Synchronization with SpectralEngine (audio thread only)
    void setSampleRate(double sampleRate) noexcept { sampleRate_.store(sampleRate, std::memory_order_relaxed); }
    void setPlaybackPosition(uint64_t samplePosition) noexcept;
    
    // State queries (any thread)
    bool isInitialized() const noexcept { return initialized_.load(std::memory_order_acquire); }
    size_t getPendingDeltaCount() const noexcept { return pendingDeltaCount_.load(std::memory_order_acquire); }
    uint64_t getLastAppliedSample() const noexcept { return lastAppliedSample_.load(std::memory_order_acquire); }

private:
    // Scheduled delta with timing
    struct ScheduledDelta {
        MaskColumnDelta delta;
        uint64_t targetSamplePosition;
        uint32_t targetHopIndex;
        bool active = false;
        
        ScheduledDelta() = default;
    };
    
    // RT-safe processing methods
    bool shouldApplyDelta(const ScheduledDelta& scheduledDelta, uint64_t currentSamplePos) const noexcept;
    void applyDelta(const MaskColumnDelta& delta) noexcept;
    uint32_t calculateHopIndex(uint64_t samplePosition) const noexcept;
    uint64_t calculateSamplePositionFromHop(uint32_t hopIndex) const noexcept;
    
    // Message queues
    MaskDeltaQueue* inputQueue_;
    AtlasUpdateQueue* outputQueue_;
    
    // Pending deltas buffer (fixed size for RT safety)
    AlignedStorage<std::array<ScheduledDelta, MAX_PENDING_DELTAS>> pendingDeltas_;
    std::atomic<size_t> pendingDeltaCount_;
    std::atomic<size_t> nextPendingIndex_;
    
    // Timing state
    std::atomic<double> sampleRate_;
    std::atomic<uint64_t> playbackStartSample_;
    std::atomic<uint64_t> lastAppliedSample_;
    std::atomic<uint32_t> currentHopIndex_;
    
    // Initialization state
    std::atomic<bool> initialized_;
    
    // Performance tracking
    std::atomic<uint32_t> deltasProcessed_;
    std::atomic<uint32_t> deltasSkipped_;
    
    // Lookahead scheduling parameters
    static constexpr uint64_t LOOKAHEAD_SAMPLES = HOP_SIZE * 4; // 4 hops lookahead
    static constexpr uint64_t MAX_SCHEDULING_JITTER = HOP_SIZE; // Max timing tolerance
};

// Inline performance-critical methods

inline uint32_t HopScheduler::calculateHopIndex(uint64_t samplePosition) const noexcept {
    const uint64_t startSample = playbackStartSample_.load(std::memory_order_acquire);
    if (samplePosition < startSample) return 0;
    
    return static_cast<uint32_t>((samplePosition - startSample) / HOP_SIZE);
}

inline uint64_t HopScheduler::calculateSamplePositionFromHop(uint32_t hopIndex) const noexcept {
    const uint64_t startSample = playbackStartSample_.load(std::memory_order_acquire);
    return startSample + (static_cast<uint64_t>(hopIndex) * HOP_SIZE);
}

inline bool HopScheduler::shouldApplyDelta(const ScheduledDelta& scheduledDelta, uint64_t currentSamplePos) const noexcept {
    if (!scheduledDelta.active) return false;
    
    const uint64_t targetSample = scheduledDelta.targetSamplePosition;
    const uint64_t tolerance = MAX_SCHEDULING_JITTER;
    
    // Apply if we're at or past the target position (within tolerance)
    return (currentSamplePos >= targetSample) && 
           (currentSamplePos <= targetSample + tolerance);
}

inline void HopScheduler::setPlaybackPosition(uint64_t samplePosition) noexcept {
    playbackStartSample_.store(samplePosition, std::memory_order_release);
    currentHopIndex_.store(0, std::memory_order_release);
}