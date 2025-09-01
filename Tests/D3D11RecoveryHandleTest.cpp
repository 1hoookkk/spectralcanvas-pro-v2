/**
 * D3D11 Recovery Handle Test Suite
 * 
 * Tests HWND storage, WARP state reporting, and device recovery API
 * without requiring actual GPU device creation (CI-safe).
 */

#include "TestHarness.h"
#ifdef _WIN32
#include "../Source/Viz/backends/D3D11Renderer.h"
#endif

using namespace test;

#ifdef _WIN32

class D3D11RecoveryHandleTest : public TestCase
{
public:
    D3D11RecoveryHandleTest() : TestCase("D3D11RecoveryHandle") {}
    
    void run() override
    {
        std::cout << "Testing D3D11 HWND storage and recovery API..." << std::endl;
        
        testHWNDStorage();
        testInitializationValidation();
        testWARPStateAPI();
        testRecoveryCallSafety();
        
        std::cout << "D3D11 recovery handle tests passed!" << std::endl;
    }
    
private:
    void testHWNDStorage()
    {
        std::cout << "  Testing HWND storage round-trip..." << std::endl;
        
        D3D11Renderer renderer;
        
        // Initially no HWND should be stored
        TEST_ASSERT(renderer.getStoredHWND() == nullptr);
        
        // Test null HWND rejection
        bool initResult = renderer.initialize(nullptr, 800, 600);
        TEST_ASSERT(!initResult); // Should fail with null HWND
        TEST_ASSERT(renderer.getStoredHWND() == nullptr); // Should not store null
        
        // Test non-null HWND storage (even if initialization fails due to no GPU in CI)
        HWND fakeHWND = reinterpret_cast<HWND>(0x12345678ULL);
        initResult = renderer.initialize(fakeHWND, 800, 600);
        
        // Whether init succeeds or fails, HWND should be stored if non-null
        TEST_ASSERT(renderer.getStoredHWND() == fakeHWND);
        
        // Test cleanup
        renderer.shutdown();
        TEST_ASSERT(renderer.getStoredHWND() == nullptr);
    }
    
    void testInitializationValidation()
    {
        std::cout << "  Testing initialization parameter validation..." << std::endl;
        
        D3D11Renderer renderer;
        
        // Test various invalid parameters
        TEST_ASSERT(!renderer.initialize(nullptr, 800, 600));
        TEST_ASSERT(!renderer.initialize(nullptr, 0, 600));
        TEST_ASSERT(!renderer.initialize(nullptr, 800, 0));
        
        // Test valid parameters (may still fail without GPU, but shouldn't crash)
        HWND fakeHWND = reinterpret_cast<HWND>(0x87654321ULL);
        bool result = renderer.initialize(fakeHWND, 1920, 1080);
        
        // In CI without GPU, this may fail, but it shouldn't crash
        // We just test that the API is stable
        if (result)
        {
            TEST_ASSERT(renderer.isInitialized());
            renderer.shutdown();
            TEST_ASSERT(!renderer.isInitialized());
        }
        else
        {
            // Failed initialization should still store HWND for later recovery attempts
            TEST_ASSERT(renderer.getStoredHWND() == fakeHWND);
        }
    }
    
    void testWARPStateAPI()
    {
        std::cout << "  Testing WARP state reporting..." << std::endl;
        
        D3D11Renderer renderer;
        
        // Initially should not be using WARP
        TEST_ASSERT(!renderer.isUsingWarp());
        
        HWND fakeHWND = reinterpret_cast<HWND>(0xABCDEF00ULL);
        bool initResult = renderer.initialize(fakeHWND, 800, 600);
        
        // After initialization (successful or not), WARP state should be queryable
        bool warpState = renderer.isUsingWarp();
        std::cout << "    WARP state after init: " << (warpState ? "true" : "false") << std::endl;
        
        // WARP state should be consistent
        TEST_ASSERT(renderer.isUsingWarp() == warpState);
        
        renderer.shutdown();
        
        // After shutdown, should not be using WARP
        TEST_ASSERT(!renderer.isUsingWarp());
    }
    
    void testRecoveryCallSafety()
    {
        std::cout << "  Testing device recovery call safety..." << std::endl;
        
        D3D11Renderer renderer;
        
        // Recovery should be safe to call even without initialization
        bool recoveryResult = renderer.tryRecreateDevice();
        TEST_ASSERT(!recoveryResult); // Should fail without stored HWND
        
        // Store HWND and test recovery
        HWND fakeHWND = reinterpret_cast<HWND>(0xDEADBEEFULL);
        renderer.initialize(fakeHWND, 640, 480);
        
        // Recovery attempt should not crash (may fail in CI, but should be stable)
        recoveryResult = renderer.tryRecreateDevice();
        std::cout << "    Recovery attempt result: " << (recoveryResult ? "success" : "failed") << std::endl;
        
        // Multiple recovery attempts should be safe
        for (int i = 0; i < 3; ++i)
        {
            recoveryResult = renderer.tryRecreateDevice();
            // Should not crash regardless of result
        }
        
        renderer.shutdown();
        
        // After shutdown, recovery should fail safely
        recoveryResult = renderer.tryRecreateDevice();
        TEST_ASSERT(!recoveryResult); // Should fail after shutdown
    }
};

#else

class D3D11RecoveryHandleTest : public TestCase
{
public:
    D3D11RecoveryHandleTest() : TestCase("D3D11RecoveryHandle") {}
    
    void run() override
    {
        std::cout << "D3D11 recovery tests skipped: not on Windows platform" << std::endl;
    }
};

#endif // _WIN32

int main()
{
    TestRunner runner;
    runner.addTest(std::make_unique<D3D11RecoveryHandleTest>());
    return runner.runAll();
}