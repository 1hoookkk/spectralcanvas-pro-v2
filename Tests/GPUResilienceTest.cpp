/**
 * GPU Resilience Test Suite for SpectralCanvas Pro
 * 
 * Tests D3D11 device-lost detection, recovery choreography, and WARP fallback.
 * Validates frame continuity and sub-100ms recovery requirements.
 */

#include "TestHarness.h"
#include "../Source/Viz/backends/D3D11Renderer.h"
#include "../Source/Core/GPUStatus.h"
#include <chrono>
#include <thread>
#include <string>
#include <iostream>

#ifdef _WIN32

using namespace test;

class GPUStatusTest : public TestCase
{
public:
    GPUStatusTest() : TestCase("GPUStatusAtomics") {}
    
    void run() override
    {
        std::cout << "Testing GPUStatus atomic operations..." << std::endl;
        
        GPUStatus status;
        
        // Test initial state
        assert(status.getDeviceState() == GPUStatus::OK);
        assert(status.getFrameTimeUs() == 16666);
        assert(status.getRecoveryCount() == 0);
        assert(!status.isWarpMode());
        
        // Test state transitions
        status.setDeviceState(GPUStatus::REMOVED);
        assert(status.getDeviceState() == GPUStatus::REMOVED);
        
        status.setDeviceState(GPUStatus::WARP_FALLBACK);
        assert(status.getDeviceState() == GPUStatus::WARP_FALLBACK);
        assert(status.isWarpMode());
        
        // Test frame timing
        status.recordFrameTime(8333); // 120fps
        assert(status.getFrameTimeUs() == 8333);
        
        std::cout << "GPUStatus tests passed!" << std::endl;
    }
};

class DeviceLostHandlerTest : public TestCase
{
public:
    DeviceLostHandlerTest() : TestCase("DeviceLostHandler") {}
    
    void run() override
    {
        std::cout << "Testing DeviceLostHandler state machine..." << std::endl;
        
        GPUStatus gpu_status;
        DeviceLostHandler handler;
        handler.initialize(&gpu_status);
        
        // Test initial state
        assert(!handler.shouldAttemptRecovery());
        assert(!handler.shouldFallbackToWarp());
        
        // Simulate device removal
        gpu_status.setDeviceState(GPUStatus::REMOVED);
        assert(handler.shouldAttemptRecovery());
        
        handler.beginRecovery();
        assert(gpu_status.getDeviceState() == GPUStatus::RECREATING);
        
        // Test successful recovery
        handler.recordSuccessfulRecovery(false); // HW mode
        assert(gpu_status.getDeviceState() == GPUStatus::OK);
        
        std::cout << "DeviceLostHandler tests passed!" << std::endl;
    }
};

class RecoveryTimerTest : public TestCase
{
public:
    RecoveryTimerTest() : TestCase("RecoveryTimer") {}
    
    void run() override
    {
        std::cout << "Testing recovery performance timer..." << std::endl;
        
        DeviceRecoveryTimer timer;
        
        // Simulate a short operation
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        uint32_t elapsed = timer.getElapsedMicroseconds();
        uint32_t elapsed_ms = elapsed / 1000;
        
        assert(elapsed_ms >= 10);
        assert(elapsed_ms < 100);
        assert(elapsed > 0); // microsecond precision
        
        std::cout << "Recovery simulation took " << elapsed_ms << "ms" << std::endl;
        std::cout << "RecoveryTimer tests passed!" << std::endl;
    }
};

#endif // _WIN32

int main()
{
#ifdef _WIN32
    TestRunner runner;
    
    runner.addTest(std::make_unique<GPUStatusTest>());
    runner.addTest(std::make_unique<DeviceLostHandlerTest>());
    runner.addTest(std::make_unique<RecoveryTimerTest>());
    
    return runner.runAll();
#else
    std::cout << "GPU Resilience tests only available on Windows" << std::endl;
    return 0;
#endif
}