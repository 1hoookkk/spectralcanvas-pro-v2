#include "HopScheduler.h"
#include "../Core/TimeUtils.h"
#include <algorithm>
#include <juce_core/juce_core.h>

HopScheduler::HopScheduler() noexcept
    : inputQueue_(nullptr)
    , outputQueue_(nullptr)
    , pendingDeltaCount_(0)
    , nextPendingIndex_(0)
    , sampleRate_(44100.0)
    , playbackStartSample_(0)
    , lastAppliedSample_(0)
    , currentHopIndex_(0)
    , initialized_(false)
    , deltasProcessed_(0)
    , deltasSkipped_(0)
{
    // Initialize pending deltas array
    auto& deltas = pendingDeltas_.get();
    for (auto& delta : deltas) {
        delta.active = false;
    }
}

bool HopScheduler::initialize(double sampleRate, 
                             MaskDeltaQueue& inputQueue,
                             AtlasUpdateQueue& outputQueue) noexcept {
    if (sampleRate <= 0.0) return false;
    
    sampleRate_.store(sampleRate, std::memory_order_relaxed);
    inputQueue_ = &inputQueue;
    outputQueue_ = &outputQueue;
    
    reset();
    
    initialized_.store(true, std::memory_order_release);
    return true;
}

void HopScheduler::reset() noexcept {
    // Clear pending deltas
    auto& deltas = pendingDeltas_.get();
    for (auto& delta : deltas) {
        delta.active = false;
    }
    
    pendingDeltaCount_.store(0, std::memory_order_release);
    nextPendingIndex_.store(0, std::memory_order_release);
    playbackStartSample_.store(0, std::memory_order_release);
    lastAppliedSample_.store(0, std::memory_order_release);
    currentHopIndex_.store(0, std::memory_order_release);
    deltasProcessed_.store(0, std::memory_order_release);
    deltasSkipped_.store(0, std::memory_order_release);
}

void HopScheduler::processHop(uint64_t currentSamplePosition, uint32_t hopIndex) noexcept {
    if (!initialized_.load(std::memory_order_acquire)) return;
    
    currentHopIndex_.store(hopIndex, std::memory_order_relaxed);
    
    // Schedule new deltas from input queue
    scheduleDeltas();
    
    // Apply any deltas that are due at this hop
    applyScheduledDeltas(currentSamplePosition);
}

void HopScheduler::scheduleDeltas() noexcept {
    if (!inputQueue_) return;
    
    MaskColumnDelta incomingDelta;
    auto& deltas = pendingDeltas_.get();
    
    // Process all available deltas from input queue
    while (inputQueue_->pop(incomingDelta)) {
        const size_t pendingCount = pendingDeltaCount_.load(std::memory_order_acquire);
        
        if (pendingCount >= MAX_PENDING_DELTAS) {
            // Buffer full - skip oldest delta and log
            deltasSkipped_.fetch_add(1, std::memory_order_relaxed);
            continue;
        }
        
        // Find next available slot
        size_t insertIndex = nextPendingIndex_.load(std::memory_order_acquire);
        while (deltas[insertIndex].active) {
            insertIndex = (insertIndex + 1) % MAX_PENDING_DELTAS;
        }
        
        // Calculate target timing for this delta
        const auto sampleRate = sampleRate_.load(std::memory_order_acquire);
        const uint64_t targetSample = static_cast<uint64_t>(incomingDelta.metadata.samplePosition);
        const uint32_t targetHop = calculateHopIndex(targetSample);
        
        // Schedule the delta
        ScheduledDelta& scheduled = deltas[insertIndex];
        scheduled.delta = incomingDelta;
        scheduled.targetSamplePosition = targetSample;
        scheduled.targetHopIndex = targetHop;
        scheduled.active = true;
        
        // Update counters
        nextPendingIndex_.store((insertIndex + 1) % MAX_PENDING_DELTAS, std::memory_order_release);
        pendingDeltaCount_.fetch_add(1, std::memory_order_release);
    }
}

void HopScheduler::applyScheduledDeltas(uint64_t currentSamplePosition) noexcept {
    if (!outputQueue_) return;
    
    auto& deltas = pendingDeltas_.get();
    size_t deltasApplied = 0;
    
    // Scan pending deltas for those ready to apply
    for (auto& scheduledDelta : deltas) {
        if (!scheduledDelta.active) continue;
        
        if (shouldApplyDelta(scheduledDelta, currentSamplePosition)) {
            applyDelta(scheduledDelta.delta);
            
            // Mark as applied
            scheduledDelta.active = false;
            deltasApplied++;
            
            lastAppliedSample_.store(scheduledDelta.targetSamplePosition, std::memory_order_release);
        }
    }
    
    // Update counters
    if (deltasApplied > 0) {
        pendingDeltaCount_.fetch_sub(deltasApplied, std::memory_order_release);
        deltasProcessed_.fetch_add(static_cast<uint32_t>(deltasApplied), std::memory_order_relaxed);
    }
}

void HopScheduler::applyDelta(const MaskColumnDelta& delta) noexcept {
    if (!outputQueue_) return;
    
    // CRITICAL: Bounds validation before processing delta
    if (delta.position.columnInTile >= AtlasConfig::TILE_WIDTH) {
        deltasSkipped_.fetch_add(1, std::memory_order_relaxed);
        return; // Skip out-of-bounds delta to prevent corruption
    }
    
    // Create atlas update message
    AtlasUpdate update;
    update.activePageHandle = {}; // Will be filled by atlas manager
    update.pendingPageHandle = {}; // Will be filled by atlas manager  
    update.position = delta.position;
    update.metadata = delta.metadata;
    
    // Send to atlas update queue (non-blocking)
    if (!outputQueue_->push(update)) {
        // Queue full - could implement backpressure or logging
        deltasSkipped_.fetch_add(1, std::memory_order_relaxed);
    }
}