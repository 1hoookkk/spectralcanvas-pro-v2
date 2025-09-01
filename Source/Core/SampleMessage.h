#pragma once
#include <cstdint>

/**
 * RT-safe sample loading message using handle-based approach
 * 
 * Replaces unsafe std::shared_ptr<SampleData> with handle into
 * preallocated SamplePool managed by RealtimeMemoryManager.
 */
struct SampleMessage
{
    uint32_t handle = 0;        // Handle from SamplePool (0 = invalid)
    uint64_t timestampMicros = 0; // UI timestamp for latency tracking
    
    SampleMessage() = default;
    
    SampleMessage(uint32_t h, uint64_t ts = 0) noexcept
        : handle(h), timestampMicros(ts) {}
    
    bool isValid() const noexcept { return handle != 0; }
};

static_assert(std::is_trivially_copyable_v<SampleMessage>, "SampleMessage must be POD for RT-safe queue usage");