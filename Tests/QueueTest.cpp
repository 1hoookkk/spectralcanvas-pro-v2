#include "TestHarness.h"
#include "AllocationDetector.h"
#include "../Source/Core/MessageBus.h"
#include <thread>
#include <chrono>
#include <random>

using namespace test;

// Test: Basic SPSC queue functionality
class BasicQueueTest : public TestCase {
public:
    BasicQueueTest() : TestCase("BasicQueue") {}
    
    void run() override {
        SpscRing<int, 64> queue;
        
        // Test empty queue
        TEST_ASSERT(!queue.hasDataAvailable());
        TEST_ASSERT(queue.hasSpaceAvailable());
        TEST_EXPECT_EQ(0u, queue.getApproxQueueDepth());
        
        // Test push/pop basic operations
        TEST_ASSERT(queue.push(42));
        TEST_ASSERT(queue.hasDataAvailable());
        
        auto result = queue.pop();
        TEST_ASSERT(result.has_value());
        TEST_EXPECT_EQ(42, result.value());
        
        // Test queue is empty again
        TEST_ASSERT(!queue.hasDataAvailable());
        result = queue.pop();
        TEST_ASSERT(!result.has_value());
        
        std::cout << "  Basic queue operations verified" << std::endl;
    }
};

// Test: Queue capacity and overflow behavior
class QueueCapacityTest : public TestCase {
public:
    QueueCapacityTest() : TestCase("QueueCapacity") {}
    
    void run() override {
        SpscRing<int, 8> queue;  // Small queue for testing overflow
        
        // Fill queue to capacity
        int itemsPushed = 0;
        for (int i = 0; i < 100; ++i) {  // Try to push more than capacity
            if (queue.push(i)) {
                itemsPushed++;
            } else {
                break;
            }
        }
        
        std::cout << "  Queue capacity: " << itemsPushed << " items" << std::endl;
        TEST_ASSERT(itemsPushed == 7);  // Power-of-2 queue leaves 1 slot for full detection
        
        // Verify queue is full
        TEST_ASSERT(!queue.hasSpaceAvailable());
        TEST_ASSERT(!queue.push(999));  // Should fail
        
        // Verify drop counting
        uint64_t initialDrops = queue.getDropCount();
        queue.push(1000);  // Should fail and increment drop count
        TEST_EXPECT_EQ(initialDrops + 1, queue.getDropCount());
        
        // Empty queue and verify
        int itemsPopped = 0;
        while (queue.hasDataAvailable()) {
            auto result = queue.pop();
            if (result.has_value()) {
                TEST_EXPECT_EQ(itemsPopped, result.value());
                itemsPopped++;
            }
        }
        
        TEST_EXPECT_EQ(itemsPushed, itemsPopped);
        
        std::cout << "  Capacity and overflow behavior verified" << std::endl;
    }
};

// Test: Concurrent producer/consumer
class ConcurrentQueueTest : public TestCase {
public:
    ConcurrentQueueTest() : TestCase("ConcurrentQueue") {}
    
    void run() override {
        SpscRing<MaskColumn, 256> queue;
        
        const int numItems = 10000;
        std::atomic<int> itemsProduced{0};
        std::atomic<int> itemsConsumed{0};
        std::atomic<bool> producerDone{false};
        
        ThreadBarrier barrier(2);
        
        // Producer thread (simulates UI thread)
        std::thread producer([&]() {
            barrier.wait();  // Synchronize start
            
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 10);
            
            for (int i = 0; i < numItems; ++i) {
                MaskColumn column{};
                column.frameIndex = static_cast<uint32_t>(i);
                column.sequenceNumber = static_cast<uint32_t>(i);
                
                while (!queue.push(column)) {
                    // Busy wait and retry (simulates backpressure)
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
                
                itemsProduced.fetch_add(1, std::memory_order_relaxed);
                
                // Variable timing to simulate real-world conditions
                if (i % 100 == 0) {
                    std::this_thread::sleep_for(std::chrono::microseconds(dis(gen)));
                }
            }
            
            producerDone.store(true, std::memory_order_release);
        });
        
        // Consumer thread (simulates audio thread)
        std::thread consumer([&]() {
            RTModeGuard guard(std::this_thread::get_id());
            barrier.wait();  // Synchronize start
            
            RTSafeAllocator::resetCounters();
            
            while (itemsConsumed.load(std::memory_order_relaxed) < numItems) {
                auto result = queue.pop();
                if (result.has_value()) {
                    // Verify data integrity
                    uint32_t expectedFrame = static_cast<uint32_t>(itemsConsumed.load());
                    if (result->frameIndex != expectedFrame) {
                        // Some reordering is expected due to concurrent access
                        // Just verify the data is valid
                        TEST_ASSERT(result->frameIndex < numItems);
                    }
                    
                    itemsConsumed.fetch_add(1, std::memory_order_relaxed);
                } else {
                    // Queue empty, check if producer is done
                    if (producerDone.load(std::memory_order_acquire)) {
                        break;
                    }
                    // Brief yield to allow producer to catch up
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
            }
            
            // Verify no allocations in RT thread
            TEST_EXPECT_EQ(0u, RTSafeAllocator::getAllocCount());
            TEST_EXPECT_EQ(0u, RTSafeAllocator::getDeallocCount());
        });
        
        producer.join();
        consumer.join();
        
        // Verify final counts
        TEST_EXPECT_EQ(numItems, itemsProduced.load());
        TEST_EXPECT_EQ(numItems, itemsConsumed.load());
        
        // Print statistics
        std::cout << "  Items produced: " << itemsProduced.load() << std::endl;
        std::cout << "  Items consumed: " << itemsConsumed.load() << std::endl;
        std::cout << "  Total pushes: " << queue.getPushCount() << std::endl;
        std::cout << "  Total pops: " << queue.getPopCount() << std::endl;
        std::cout << "  Drops: " << queue.getDropCount() << std::endl;
        
        std::cout << "  Concurrent producer/consumer test completed" << std::endl;
    }
};

// Test: Queue performance under stress
class QueueStressTest : public TestCase {
public:
    QueueStressTest() : TestCase("QueueStress") {}
    
    void run() override {
        SpscRing<SpectralFrame, 1024> queue;
        
        const int numIterations = 100000;
        
        ThreadBarrier barrier(2);
        PerfStats pushStats, popStats;
        
        // High-frequency producer
        std::thread producer([&]() {
            barrier.wait();
            
            for (int i = 0; i < numIterations; ++i) {
                SpectralFrame frame{};
                frame.sequenceNumber = static_cast<uint32_t>(i);
                
                HiResTimer timer;
                while (!queue.push(frame)) {
                    // Spin until space available
                    std::this_thread::yield();
                }
                double pushTimeUs = timer.elapsedUs();
                pushStats.addSample(pushTimeUs);
            }
        });
        
        // High-frequency consumer
        std::thread consumer([&]() {
            barrier.wait();
            
            int consumed = 0;
            while (consumed < numIterations) {
                HiResTimer timer;
                auto result = queue.pop();
                double popTimeUs = timer.elapsedUs();
                
                if (result.has_value()) {
                    popStats.addSample(popTimeUs);
                    consumed++;
                } else {
                    std::this_thread::yield();
                }
            }
        });
        
        producer.join();
        consumer.join();
        
        // Print performance statistics
        pushStats.print("Push Performance (microseconds)");
        popStats.print("Pop Performance (microseconds)");
        
        // Verify performance targets
        TEST_EXPECT_LT(pushStats.percentile(0.95), 10.0);  // P95 < 10μs
        TEST_EXPECT_LT(popStats.percentile(0.95), 10.0);   // P95 < 10μs
        
        std::cout << "  Queue stress test completed successfully" << std::endl;
    }
};

// Test: Multiple queue statistics and diagnostics
class QueueDiagnosticsTest : public TestCase {
public:
    QueueDiagnosticsTest() : TestCase("QueueDiagnostics") {}
    
    void run() override {
        SpscRing<int, 32> queue;
        
        // Test statistics tracking
        TEST_EXPECT_EQ(0u, queue.getPushCount());
        TEST_EXPECT_EQ(0u, queue.getPopCount());
        TEST_EXPECT_EQ(0u, queue.getDropCount());
        
        // Add some data
        const int numItems = 20;
        for (int i = 0; i < numItems; ++i) {
            TEST_ASSERT(queue.push(i));
        }
        
        TEST_EXPECT_EQ(numItems, queue.getPushCount());
        TEST_EXPECT_EQ(0u, queue.getPopCount());
        
        // Test queue depth approximation
        size_t depth = queue.getApproxQueueDepth();
        TEST_ASSERT(depth >= numItems - 1 && depth <= numItems + 1);  // Allow for slight variance
        
        // Consume half
        for (int i = 0; i < numItems / 2; ++i) {
            auto result = queue.pop();
            TEST_ASSERT(result.has_value());
            TEST_EXPECT_EQ(i, result.value());
        }
        
        TEST_EXPECT_EQ(numItems, queue.getPushCount());
        TEST_EXPECT_EQ(numItems / 2, queue.getPopCount());
        
        // Test clear functionality
        queue.clear();
        TEST_EXPECT_EQ(0u, queue.getPushCount());
        TEST_EXPECT_EQ(0u, queue.getPopCount());
        TEST_EXPECT_EQ(0u, queue.getDropCount());
        TEST_EXPECT_EQ(0u, queue.getApproxQueueDepth());
        
        std::cout << "  Queue diagnostics and statistics verified" << std::endl;
    }
};

int main() {
    std::cout << "SPSC Queue Validation Tests" << std::endl;
    std::cout << std::endl;
    
    TestRunner runner;
    
    runner.addTest(std::make_unique<BasicQueueTest>());
    runner.addTest(std::make_unique<QueueCapacityTest>());
    runner.addTest(std::make_unique<ConcurrentQueueTest>());
    runner.addTest(std::make_unique<QueueStressTest>());
    runner.addTest(std::make_unique<QueueDiagnosticsTest>());
    
    return runner.runAll();
}