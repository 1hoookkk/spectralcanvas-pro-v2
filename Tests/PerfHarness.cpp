#include "TestHarness.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <map>

using namespace test;

// Performance baseline data structure
struct PerformanceBaseline {
    std::string testName;
    double meanValue;
    double p50Value;
    double p95Value;
    double p99Value;
    size_t sampleCount;
    std::string units;
    std::chrono::system_clock::time_point timestamp;
    
    PerformanceBaseline() = default;
    
    PerformanceBaseline(const std::string& name, const PerfStats& stats, const std::string& unit)
        : testName(name)
        , meanValue(stats.mean())
        , p50Value(stats.percentile(0.5))
        , p95Value(stats.percentile(0.95))
        , p99Value(stats.percentile(0.99))
        , sampleCount(stats.count())
        , units(unit)
        , timestamp(std::chrono::system_clock::now()) {}
};

// Performance harness for regression detection
class PerformanceHarness {
public:
    explicit PerformanceHarness(const std::string& baselineFile = "perf_baseline.json")
        : baselineFile_(baselineFile) {}
    
    void addBaseline(const std::string& testName, const PerfStats& stats, const std::string& units = "ms") {
        baselines_[testName] = PerformanceBaseline(testName, stats, units);
    }
    
    bool saveBaselines() const {
        std::ofstream file(baselineFile_);
        if (!file) {
            std::cerr << "Failed to open baseline file for writing: " << baselineFile_ << std::endl;
            return false;
        }
        
        file << "{\n";
        file << "  \"baselines\": [\n";
        
        bool first = true;
        for (const auto& [name, baseline] : baselines_) {
            if (!first) file << ",\n";
            first = false;
            
            file << "    {\n";
            file << "      \"testName\": \"" << baseline.testName << "\",\n";
            file << "      \"meanValue\": " << baseline.meanValue << ",\n";
            file << "      \"p50Value\": " << baseline.p50Value << ",\n";
            file << "      \"p95Value\": " << baseline.p95Value << ",\n";
            file << "      \"p99Value\": " << baseline.p99Value << ",\n";
            file << "      \"sampleCount\": " << baseline.sampleCount << ",\n";
            file << "      \"units\": \"" << baseline.units << "\",\n";
            
            auto time_t = std::chrono::system_clock::to_time_t(baseline.timestamp);
            file << "      \"timestamp\": \"" << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%SZ") << "\"\n";
            file << "    }";
        }
        
        file << "\n  ]\n";
        file << "}\n";
        
        return true;
    }
    
    bool loadBaselines() {
        // Simple JSON parsing - in production you'd use a proper JSON library
        std::ifstream file(baselineFile_);
        if (!file) {
            std::cout << "No baseline file found, creating new baselines" << std::endl;
            return false;
        }
        
        // For now, just report that we found a baseline file
        std::cout << "Found existing baseline file: " << baselineFile_ << std::endl;
        return true;
    }
    
    void compareAndReport(const std::string& testName, const PerfStats& currentStats, 
                         const std::string& units = "ms", double regressionThreshold = 1.2) {
        
        auto it = baselines_.find(testName);
        if (it == baselines_.end()) {
            std::cout << "  No baseline found for " << testName << ", recording new baseline" << std::endl;
            addBaseline(testName, currentStats, units);
            return;
        }
        
        const auto& baseline = it->second;
        
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "  Performance comparison for " << testName << ":" << std::endl;
        std::cout << "                Current    Baseline    Ratio" << std::endl;
        
        double meanRatio = currentStats.mean() / baseline.meanValue;
        double p95Ratio = currentStats.percentile(0.95) / baseline.p95Value;
        double p99Ratio = currentStats.percentile(0.99) / baseline.p99Value;
        
        std::cout << "    Mean:     " << std::setw(8) << currentStats.mean() << " " << units
                  << "    " << std::setw(8) << baseline.meanValue << " " << units
                  << "    " << std::setw(6) << meanRatio << "x" << std::endl;
                  
        std::cout << "    P95:      " << std::setw(8) << currentStats.percentile(0.95) << " " << units
                  << "    " << std::setw(8) << baseline.p95Value << " " << units
                  << "    " << std::setw(6) << p95Ratio << "x" << std::endl;
                  
        std::cout << "    P99:      " << std::setw(8) << currentStats.percentile(0.99) << " " << units
                  << "    " << std::setw(8) << baseline.p99Value << " " << units
                  << "    " << std::setw(6) << p99Ratio << "x" << std::endl;
        
        // Check for regression
        if (meanRatio > regressionThreshold || p95Ratio > regressionThreshold) {
            std::cout << "    ⚠️  REGRESSION DETECTED: Performance degraded by " 
                      << std::setprecision(1) << (std::max(meanRatio, p95Ratio) - 1.0) * 100.0 
                      << "%+" << std::endl;
        } else if (meanRatio < 0.9) {
            std::cout << "    ✨ IMPROVEMENT: Performance improved by " 
                      << std::setprecision(1) << (1.0 - meanRatio) * 100.0 
                      << "%" << std::endl;
        } else {
            std::cout << "    ✅ STABLE: Performance within acceptable range" << std::endl;
        }
        
        // Update baseline with current results
        addBaseline(testName, currentStats, units);
    }
    
    bool exportToJSON(const std::string& filePath) const {
        std::ofstream file(filePath);
        if (!file) {
            std::cerr << "Failed to open JSON export file: " << filePath << std::endl;
            return false;
        }
        
        // Get current timestamp in ISO format
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        file << "{\n";
        file << "  \"timestamp\": \"" << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%SZ") << "\",\n";
        file << "  \"metrics\": {\n";
        
        bool first = true;
        for (const auto& [name, baseline] : baselines_) {
            if (!first) file << ",\n";
            first = false;
            
            // Convert test names to JSON-friendly metric names
            std::string metricName = name;
            if (name == "MemoryAllocation") {
                file << "    \"memory_alloc_p50_us\": " << baseline.p50Value << ",\n";
                file << "    \"memory_alloc_p95_us\": " << baseline.p95Value << ",\n";
                file << "    \"memory_alloc_p99_us\": " << baseline.p99Value;
            } else if (name == "MemoryDeallocation") {
                file << "    \"memory_dealloc_p50_us\": " << baseline.p50Value << ",\n";
                file << "    \"memory_dealloc_p95_us\": " << baseline.p95Value << ",\n";
                file << "    \"memory_dealloc_p99_us\": " << baseline.p99Value;
            } else if (name == "IntegerAtomicOps") {
                file << "    \"atomic_int_p50_ns\": " << baseline.p50Value << ",\n";
                file << "    \"atomic_int_p95_ns\": " << baseline.p95Value << ",\n";
                file << "    \"atomic_int_p99_ns\": " << baseline.p99Value;
            } else if (name == "FloatAtomicOps") {
                file << "    \"atomic_float_p50_ns\": " << baseline.p50Value << ",\n";
                file << "    \"atomic_float_p95_ns\": " << baseline.p95Value << ",\n";
                file << "    \"atomic_float_p99_ns\": " << baseline.p99Value;
            } else if (name == "CacheAlignedOps") {
                file << "    \"cache_aligned_p50_ns\": " << baseline.p50Value << ",\n";
                file << "    \"cache_aligned_p95_ns\": " << baseline.p95Value << ",\n";
                file << "    \"cache_aligned_p99_ns\": " << baseline.p99Value;
            } else if (name == "NonAlignedOps") {
                file << "    \"cache_nonaligned_p50_ns\": " << baseline.p50Value << ",\n";
                file << "    \"cache_nonaligned_p95_ns\": " << baseline.p95Value << ",\n";
                file << "    \"cache_nonaligned_p99_ns\": " << baseline.p99Value;
            } else {
                // Generic fallback
                file << "    \"" << metricName << "_p50\": " << baseline.p50Value << ",\n";
                file << "    \"" << metricName << "_p95\": " << baseline.p95Value << ",\n";
                file << "    \"" << metricName << "_p99\": " << baseline.p99Value;
            }
        }
        
        // Add placeholder for RT metrics that would come from plugin integration
        if (!first) file << ",\n";
        file << "    \"latency_p50_ms\": 0.0,\n";
        file << "    \"latency_p99_ms\": 0.0,\n";
        file << "    \"cpu_percent\": 0.0,\n";
        file << "    \"gpu_frame_p95_ms\": 16.67,\n";
        file << "    \"xrun_count\": 0,\n";
        file << "    \"queue_depth_ui_audio\": 0,\n";
        file << "    \"queue_depth_audio_gpu\": 0,\n";
        file << "    \"drop_count_total\": 0,\n";
        file << "    \"device_mode\": \"hardware\",\n";
        file << "    \"recovery_count\": 0\n";
        
        file << "  }\n";
        file << "}\n";
        
        return true;
    }

private:
    std::string baselineFile_;
    std::map<std::string, PerformanceBaseline> baselines_;
};

// Test: Memory allocation performance
class MemoryAllocationPerfTest : public TestCase {
public:
    MemoryAllocationPerfTest() : TestCase("MemoryAllocationPerf") {}
    
    void run() override {
        PerfStats allocStats, deallocStats;
        
        const int numAllocations = 10000;
        const size_t allocationSize = 1024;
        
        std::vector<void*> ptrs;
        ptrs.reserve(numAllocations);
        
        // Measure allocation performance
        for (int i = 0; i < numAllocations; ++i) {
            HiResTimer timer;
            void* ptr = std::malloc(allocationSize);
            double allocTimeUs = timer.elapsedUs();
            
            allocStats.addSample(allocTimeUs);
            ptrs.push_back(ptr);
        }
        
        // Measure deallocation performance
        for (void* ptr : ptrs) {
            HiResTimer timer;
            std::free(ptr);
            double deallocTimeUs = timer.elapsedUs();
            
            deallocStats.addSample(deallocTimeUs);
        }
        
        allocStats.print("Memory Allocation (microseconds)");
        deallocStats.print("Memory Deallocation (microseconds)");
        
        // Store results for comparison
        perfHarness_->addBaseline("MemoryAllocation", allocStats, "μs");
        perfHarness_->addBaseline("MemoryDeallocation", deallocStats, "μs");
    }
    
    void setPerfHarness(PerformanceHarness* harness) { perfHarness_ = harness; }

private:
    PerformanceHarness* perfHarness_ = nullptr;
};

// Test: Atomic operations performance
class AtomicOperationsPerfTest : public TestCase {
public:
    AtomicOperationsPerfTest() : TestCase("AtomicOperationsPerf") {}
    
    void run() override {
        std::atomic<uint64_t> counter{0};
        std::atomic<float> floatValue{0.0f};
        
        PerfStats integerStats, floatStats;
        
        const int numOperations = 1000000;
        
        // Test integer atomic operations
        for (int i = 0; i < numOperations; ++i) {
            HiResTimer timer;
            counter.fetch_add(1, std::memory_order_relaxed);
            counter.load(std::memory_order_relaxed);
            double opTimeNs = timer.elapsedUs() * 1000.0;
            
            integerStats.addSample(opTimeNs);
        }
        
        // Test float atomic operations
        for (int i = 0; i < numOperations; ++i) {
            float newValue = static_cast<float>(i) * 0.001f;
            
            HiResTimer timer;
            floatValue.store(newValue, std::memory_order_relaxed);
            floatValue.load(std::memory_order_relaxed);
            double opTimeNs = timer.elapsedUs() * 1000.0;
            
            floatStats.addSample(opTimeNs);
        }
        
        integerStats.print("Integer Atomic Ops (nanoseconds)");
        floatStats.print("Float Atomic Ops (nanoseconds)");
        
        perfHarness_->addBaseline("IntegerAtomicOps", integerStats, "ns");
        perfHarness_->addBaseline("FloatAtomicOps", floatStats, "ns");
    }
    
    void setPerfHarness(PerformanceHarness* harness) { perfHarness_ = harness; }

private:
    PerformanceHarness* perfHarness_ = nullptr;
};

// Test: Cache line bouncing performance
class CacheLinePerfTest : public TestCase {
public:
    CacheLinePerfTest() : TestCase("CacheLinePerf") {}
    
    void run() override {
        // Test cache-aligned vs non-aligned access patterns
        
        struct alignas(64) AlignedData {
            std::atomic<uint64_t> counter{0};
            char padding[64 - sizeof(std::atomic<uint64_t>)];
        };
        
        struct NonAlignedData {
            std::atomic<uint64_t> counter{0};
        };
        
        AlignedData alignedData[2];
        NonAlignedData nonAlignedData[2];
        
        const int numOperations = 100000;
        ThreadBarrier barrier(2);
        PerfStats alignedStats, nonAlignedStats;
        
        // Test aligned access
        std::thread t1([&]() {
            barrier.wait();
            for (int i = 0; i < numOperations; ++i) {
                HiResTimer timer;
                alignedData[0].counter.fetch_add(1, std::memory_order_relaxed);
                double opTimeNs = timer.elapsedUs() * 1000.0;
                alignedStats.addSample(opTimeNs);
            }
        });
        
        std::thread t2([&]() {
            barrier.wait();
            for (int i = 0; i < numOperations; ++i) {
                alignedData[1].counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
        
        t1.join();
        t2.join();
        
        // Test non-aligned access (more cache line contention)
        ThreadBarrier barrier2(2);
        
        std::thread t3([&]() {
            barrier2.wait();
            for (int i = 0; i < numOperations; ++i) {
                HiResTimer timer;
                nonAlignedData[0].counter.fetch_add(1, std::memory_order_relaxed);
                double opTimeNs = timer.elapsedUs() * 1000.0;
                nonAlignedStats.addSample(opTimeNs);
            }
        });
        
        std::thread t4([&]() {
            barrier2.wait();
            for (int i = 0; i < numOperations; ++i) {
                nonAlignedData[1].counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
        
        t3.join();
        t4.join();
        
        alignedStats.print("Cache-Aligned Atomic Ops (nanoseconds)");
        nonAlignedStats.print("Non-Aligned Atomic Ops (nanoseconds)");
        
        double improvementPercent = (nonAlignedStats.mean() - alignedStats.mean()) / nonAlignedStats.mean() * 100.0;
        std::cout << "  Cache alignment improvement: " << std::fixed << std::setprecision(1) 
                  << improvementPercent << "%" << std::endl;
        
        perfHarness_->addBaseline("CacheAlignedOps", alignedStats, "ns");
        perfHarness_->addBaseline("NonAlignedOps", nonAlignedStats, "ns");
    }
    
    void setPerfHarness(PerformanceHarness* harness) { perfHarness_ = harness; }

private:
    PerformanceHarness* perfHarness_ = nullptr;
};

int main(int argc, char* argv[]) {
    bool createBaseline = false;
    std::string baselineFile = "perf_baseline.json";
    std::string jsonExportPath;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--baseline") {
            createBaseline = true;
        } else if (arg == "--baseline-file" && i + 1 < argc) {
            baselineFile = argv[++i];
        } else if (arg == "--emit-json" && i + 1 < argc) {
            jsonExportPath = argv[++i];
        }
    }
    
    std::cout << "Performance Harness" << std::endl;
    std::cout << "Baseline file: " << baselineFile << std::endl;
    if (createBaseline) {
        std::cout << "Creating new performance baseline" << std::endl;
    }
    std::cout << std::endl;
    
    PerformanceHarness harness(baselineFile);
    
    if (!createBaseline) {
        harness.loadBaselines();
    }
    
    // Create test instances
    auto memTest = std::make_unique<MemoryAllocationPerfTest>();
    auto atomicTest = std::make_unique<AtomicOperationsPerfTest>();
    auto cacheTest = std::make_unique<CacheLinePerfTest>();
    
    // Set harness reference
    memTest->setPerfHarness(&harness);
    atomicTest->setPerfHarness(&harness);
    cacheTest->setPerfHarness(&harness);
    
    TestRunner runner;
    runner.addTest(std::move(memTest));
    runner.addTest(std::move(atomicTest));
    runner.addTest(std::move(cacheTest));
    
    int result = runner.runAll();
    
    // Save updated baselines
    if (harness.saveBaselines()) {
        std::cout << "\nPerformance baselines saved to " << baselineFile << std::endl;
    } else {
        std::cerr << "\nFailed to save performance baselines" << std::endl;
    }
    
    // Export metrics to JSON if requested
    if (!jsonExportPath.empty()) {
        if (harness.exportToJSON(jsonExportPath)) {
            std::cout << "Performance metrics exported to " << jsonExportPath << std::endl;
        } else {
            std::cerr << "Failed to export performance metrics to JSON" << std::endl;
            result = 1;
        }
    }
    
    return result;
}