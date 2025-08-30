#pragma once

#include <atomic>
#include <thread>
#include <iostream>
#include <cassert>

namespace test {

// Global state for allocation tracking
extern std::thread::id g_audioThreadId;
extern std::atomic<bool> g_rtModeEnabled;
extern std::atomic<size_t> g_allocCount;
extern std::atomic<size_t> g_deallocCount;

// Custom allocator that detects RT violations
class RTSafeAllocator {
public:
    static void enableRTMode(std::thread::id audioThreadId) {
        g_audioThreadId = audioThreadId;
        g_rtModeEnabled.store(true, std::memory_order_release);
        g_allocCount.store(0, std::memory_order_relaxed);
        g_deallocCount.store(0, std::memory_order_relaxed);
    }
    
    static void disableRTMode() {
        g_rtModeEnabled.store(false, std::memory_order_release);
    }
    
    static bool isAudioThread() {
        return std::this_thread::get_id() == g_audioThreadId;
    }
    
    static bool isRTModeEnabled() {
        return g_rtModeEnabled.load(std::memory_order_acquire);
    }
    
    static void checkRTViolation(const char* operation) {
        if (isRTModeEnabled() && isAudioThread()) {
            std::cerr << "RT VIOLATION: " << operation 
                      << " called on audio thread!" << std::endl;
            std::abort();
        }
    }
    
    static size_t getAllocCount() {
        return g_allocCount.load(std::memory_order_relaxed);
    }
    
    static size_t getDeallocCount() {
        return g_deallocCount.load(std::memory_order_relaxed);
    }
    
    static void resetCounters() {
        g_allocCount.store(0, std::memory_order_relaxed);
        g_deallocCount.store(0, std::memory_order_relaxed);
    }
};

// RAII helper for RT mode testing
class RTModeGuard {
public:
    explicit RTModeGuard(std::thread::id audioThreadId) {
        RTSafeAllocator::enableRTMode(audioThreadId);
    }
    
    ~RTModeGuard() {
        RTSafeAllocator::disableRTMode();
    }
    
    RTModeGuard(const RTModeGuard&) = delete;
    RTModeGuard& operator=(const RTModeGuard&) = delete;
};

} // namespace test

// Global operator overrides for allocation detection
#ifdef ENABLE_RT_SAFETY_CHECKS

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

// Placement new variants are already provided by the standard library
// inline void* operator new(size_t, void* ptr) noexcept { return ptr; }
// inline void* operator new[](size_t, void* ptr) noexcept { return ptr; }
// inline void operator delete(void*, void*) noexcept {}
// inline void operator delete[](void*, void*) noexcept {}

#endif // ENABLE_RT_SAFETY_CHECKS