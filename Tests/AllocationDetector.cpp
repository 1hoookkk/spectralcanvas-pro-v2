#include "AllocationDetector.h"
#include <cstdlib>

namespace test {

// Global state definitions
std::thread::id g_audioThreadId{};
std::atomic<bool> g_rtModeEnabled{false};
std::atomic<size_t> g_allocCount{0};
std::atomic<size_t> g_deallocCount{0};

} // namespace test

#ifdef ENABLE_RT_SAFETY_CHECKS

// Global operator overrides for allocation detection
void* operator new(size_t size) {
    test::RTSafeAllocator::checkRTViolation("operator new");
    test::g_allocCount.fetch_add(1, std::memory_order_relaxed);
    return std::malloc(size);
}

void* operator new[](size_t size) {
    test::RTSafeAllocator::checkRTViolation("operator new[]");
    test::g_allocCount.fetch_add(1, std::memory_order_relaxed);
    return std::malloc(size);
}

void operator delete(void* ptr) noexcept {
    if (ptr) {
        test::RTSafeAllocator::checkRTViolation("operator delete");
        test::g_deallocCount.fetch_add(1, std::memory_order_relaxed);
        std::free(ptr);
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr) {
        test::RTSafeAllocator::checkRTViolation("operator delete[]");
        test::g_deallocCount.fetch_add(1, std::memory_order_relaxed);
        std::free(ptr);
    }
}

#endif // ENABLE_RT_SAFETY_CHECKS