# Strategic DSP Knowledge Card Template

*Use this template for problem-first, validation-driven knowledge cards that keep you in the driver's seat.*

---

# [Topic Name]

## Problem Statement
**What specific bottleneck or limitation are you trying to solve?**
- Current implementation: [Describe existing approach]
- Performance issue: [Specific metrics - latency, CPU usage, memory]
- Constraint: [RT-audio requirement, hardware limitation, etc.]
- Goal: [Specific improvement target with numbers]

## JUCE Baseline Analysis
**Always start with what JUCE provides before going custom:**

### What JUCE Already Offers:
- **Relevant Classes**: juce::dsp::FFT, juce::dsp::WindowingFunction, etc.
- **Performance Baseline**: [Measured performance on your target hardware]
- **Pros**: [What works well with JUCE implementation]
- **Cons**: [Specific limitations for your use case]

### When to Use JUCE vs Custom:
- **Use JUCE when**: [Specific scenarios where JUCE is sufficient]
- **Go custom when**: [Specific scenarios requiring optimization]
- **Never worth optimizing**: [Cases where effort isn't justified]

## Core Concepts (Theory)
*Only the theory directly relevant to solving your problem:*

- **Key Principle 1**: [Mathematical concept with direct application]
- **Key Principle 2**: [Algorithm insight that enables optimization]
- **Key Principle 3**: [Trade-off or constraint to understand]

## Implementation Strategy

### Step-by-Step Approach:
1. **Measure Current Performance**: [How to profile existing code]
2. **Implement Textbook Solution**: [Direct translation from theory]
3. **Optimize for RT Constraints**: [Real-world adaptations needed]
4. **Validate Correctness**: [Testing against known-good reference]
5. **Profile Performance**: [Benchmark against baseline]

### Code Template:
```cpp
// Validation test harness - ALWAYS implement first
class DSPValidationTest {
public:
    void validateAgainstJUCE() {
        // Test with known input
        auto testSignal = generateTestSignal();
        
        // JUCE reference implementation
        auto juceResult = processWithJUCE(testSignal);
        
        // Your implementation
        auto customResult = processWithCustom(testSignal);
        
        // Verify accuracy within tolerance
        REQUIRE(compareResults(juceResult, customResult, 1e-6f));
    }
    
    void profilePerformance() {
        // Benchmark comparison
        auto juceTime = profileJUCE(1000); // 1000 iterations
        auto customTime = profileCustom(1000);
        
        // Must be measurably better
        REQUIRE(customTime < juceTime * 0.8f); // 20% improvement minimum
    }
};

// Your implementation with clear performance contracts
class OptimizedProcessor {
private:
    // Preallocated buffers - RT-safe
    juce::AudioBuffer<float> workBuffer_;
    
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        // ONE-TIME initialization
        workBuffer_.setSize(2, samplesPerBlock);
    }
    
    void processBlock(juce::AudioBuffer<float>& buffer) noexcept {
        juce::ScopedNoDenormals noDenormals;
        
        // Your optimized algorithm here
        // MUST maintain RT-safety constraints
    }
};
```

## RT-Audio Reality Check

### Performance Requirements:
- **Latency Target**: [Specific ms requirement]
- **CPU Budget**: [% of available processing power]
- **Memory Constraint**: [Maximum allocation during prepareToPlay]

### ✅ Implementation Checklist:
- [ ] **Accuracy verified** against JUCE reference implementation
- [ ] **Performance measured** and shows meaningful improvement (>20%)
- [ ] **RT-safety maintained** (no allocations in processBlock)
- [ ] **Edge cases handled** (silence, DC, Nyquist frequency)
- [ ] **Profiled on target hardware** (not just development machine)

### ❌ Red Flags:
- Implementation is slower than JUCE baseline
- Accuracy differs from reference by >1e-6
- Code allocates memory during processing
- Improvement is marginal (<10%) for added complexity

## Decision Framework

### Use This Implementation When:
- **Performance gain is significant**: >20% improvement in measured scenarios
- **JUCE limitation is confirmed**: Specific bottleneck that can't be worked around
- **Complexity is justified**: Maintenance cost is acceptable for performance gain

### Stick with JUCE When:
- **Performance difference is marginal**: <20% improvement
- **Implementation complexity is high**: Hard to maintain or debug
- **JUCE solution exists**: Framework already provides adequate functionality

## Validation Protocol

### Required Tests:
1. **Correctness Test**: Compare output against JUCE reference
2. **Performance Test**: Measure actual improvement on target hardware  
3. **Stress Test**: Handle edge cases (silence, maximum amplitude, etc.)
4. **Integration Test**: Works correctly in full SpectralCanvasPro pipeline
5. **Regression Test**: Performance doesn't degrade over time

### Success Criteria:
- Passes all correctness tests with <1e-6 error
- Shows >20% performance improvement in realistic scenarios
- Maintains RT-safety under all conditions
- Integrates cleanly with existing codebase

## Sources & References
- **Primary Source**: [Specific textbook chapter/equation used]
- **JUCE Documentation**: [Relevant JUCE classes and methods]
- **Validation Reference**: [Known-good implementation used for comparison]
- **Performance Baseline**: [Benchmark results on target hardware]

## Lessons Learned
*Update this section after implementation:*
- **What worked**: [Successful optimizations and techniques]
- **What didn't**: [Approaches that failed or weren't worth it]
- **Gotchas**: [Unexpected issues or edge cases discovered]
- **Recommendations**: [Advice for similar future optimizations]

---

## Template Usage Notes

### Before Starting:
1. **Identify specific performance bottleneck** with actual measurements
2. **Research what JUCE already provides** for this problem
3. **Set concrete success criteria** (numbers, not feelings)

### During Implementation:
1. **Write validation tests first** before implementing optimization
2. **Implement simplest version first** based on textbook theory
3. **Profile constantly** to ensure progress toward goal

### After Implementation:
1. **Document what you learned** for future reference
2. **Update performance baselines** for other optimizations
3. **Consider contributing back** to JUCE if improvement is significant

This template ensures every DSP optimization effort is strategic, measurable, and maintainable.