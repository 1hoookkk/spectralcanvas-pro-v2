# SpectralCanvas Pro: Agent OS Instructions
## EMU Audity 2000 Hybrid Implementation Guide

**Project Type:** JUCE Audio Plugin with Advanced DSP Processing  
**Architecture:** Digital Brain (EMU Audity 2000) + Analog Heart (SSM Character Chain)  
**Development Phase:** Phase 1 - Digital Foundation (Weeks 1-6)

---

## 🎯 PROJECT CONTEXT & VISION

### Core Mission
> **SpectralCanvas Pro: The world's first EMU Audity 2000 recreation with gesture-based control, combining the digital brain of 1998 with the analog soul of 1979.**

### Hybrid Architecture Philosophy
- **Digital Core:** Authentic EMU Audity 2000 sample engine with Z-plane filter morphing
- **Analog Heart:** SSM-inspired character processing via EMUFilter + TubeStage chain  
- **Modern Interface:** Paint-gesture control impossible with traditional keyboard interfaces

### Key Technical Components
1. **Sample Playback Engine:** 39kHz/16-bit PCM streaming with JUCE
2. **Z-Plane Filter System:** 50 filter types with real-time morphing
3. **Modulation Matrix:** 64×64 routing with paint gesture integration
4. **16-Part Arpeggiator:** Complex pattern generation and tempo sync
5. **Analog Character Chain:** EMUFilter + TubeStage for vintage warmth

---

## 🔧 DEVELOPMENT GUIDELINES

### Code Architecture Standards

#### JUCE Integration
- **PluginProcessor.cpp:** Main audio processing pipeline with RT-safety
- **SpectralSynthEngine.h/.cpp:** Core spectral synthesis engine
- **PaintQueue.h:** Lock-free paint gesture queue for real-time processing
- **CrashToggles.h:** Feature safety toggles for development stability

#### DSP Implementation
```cpp
// CORRECT: Behavioral recreation approach
class SpectralCanvasZPlaneFilter {
    // Our own DSP implementation that recreates EMU behavior
    void morphBetweenTypes(FilterType a, FilterType b, float morph);
    
private:
    // Our own filter coefficients and algorithms
    IIRFilter filterImplementation;
    FilterBehaviorConfig behaviorModel;  // Extracted from X3 analysis
};
```

#### Real-Time Safety Requirements
- **Latency Target:** <5ms paint-gesture latency
- **Polyphony:** 64-voice polyphony support
- **CPU Usage:** Optimized for complex Z-plane calculations
- **Memory Management:** Efficient sample library handling

### Clean-Room Implementation Protocol

#### ✅ Permitted Activities
- **Behavioral Analysis:** Study how presets respond to parameter changes
- **Architecture Documentation:** Document system organization and relationships
- **Parameter Mapping:** Extract parameter ranges and modulation routings
- **Pattern Analysis:** Study arpeggiator patterns and timing behaviors

#### ❌ Forbidden Activities
- **Sample Extraction:** No copyrighted audio content usage
- **Code Copying:** No direct algorithm or code duplication
- **ROM Dumping:** No binary ROM extraction or usage
- **Trademark Infringement:** No use of EMU trademarks or copyrighted names

---

## 🗓️ CURRENT DEVELOPMENT PHASE

### Phase 1: Digital Foundation (Weeks 1-6)
**Goal:** Implement authentic EMU Audity 2000 core engine

#### Week 1-2: Sample Playback Engine
- [ ] 39kHz/16-bit PCM sample streaming with JUCE
- [ ] Memory-efficient sample management and caching
- [ ] Multi-voice allocation system (32-64 voices dynamic)
- [ ] Performance optimization for real-time playback

#### Week 3-4: Z-Plane Filter System
- [ ] 50 filter type implementation based on extracted behaviors
- [ ] Real-time morphing interpolation system
- [ ] 6-pole and 12-pole filter variants
- [ ] Performance optimization for real-time processing

#### Week 5-6: Core Engine Integration
- [ ] Sample playback engine with 39kHz/16-bit streaming
- [ ] Modulation matrix with 64×64 routing capability
- [ ] 16-part arpeggiator engine with pattern memory
- [ ] APVTS parameter system matching EMU architecture

---

## 🎨 UI/UX DEVELOPMENT GUIDELINES

### Paint Canvas Integration
- **Visual Feedback:** Real-time display of Z-plane filter morphing states
- **Pattern Visualization:** Arpeggiator patterns displayed on canvas
- **Modulation Indicators:** Visual representation of active modulation routings

### Parameter Control Evolution
- **EMU Tone Knob:** Central character control with visual feedback
- **Filter Morphing Display:** Real-time visualization of current filter type
- **Arpeggiator Controls:** Integration with existing UI patterns

### User Experience Standards
- **Immediate Feedback:** Paint gestures must provide instant audio response
- **Visual Clarity:** Complex DSP operations must be visually comprehensible
- **Workflow Integration:** Seamless operation within professional DAW environments

---

## 🧪 TESTING & VALIDATION

### Technical Validation Criteria
- [ ] **Authentic Sound Recreation:** A/B testing against hardware Audity 2000 units
- [ ] **Performance Targets:** <5ms paint-gesture latency, 64-voice polyphony
- [ ] **Stability Requirements:** No audio dropouts under maximum load
- [ ] **Integration Success:** Seamless operation within SpectralCanvas Pro

### Development Testing
- **Emergency Test Tone:** Press 'T' key in plugin editor for audio debugging
- **Continuous Testing:** Use `tdd-guard.bat` for automated testing
- **Quick Validation:** Use `tools/test/phase7-quick-validation.bat`

### Build System Integration
```bash
# Development build with all tests
cmake -S . -B build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --config RelWithDebInfo --target ALL_BUILD

# Run RT-safety validation
ctest --test-dir build -C RelWithDebInfo --output-on-failure
```

---

## 📚 RESOURCE INTEGRATION

### Emulator X3 Archive Analysis
- **Archive Structure:** Complete file system mapping and cataloging
- **Preset Analysis:** Parameter extraction and behavioral documentation
- **Behavioral Modeling:** Clean-room methodology for authentic recreation
- **Implementation Guidance:** Architectural blueprints for development

### Supporting Documentation
- **EMU_AUDITY_2000_DISCOVERY.md:** The breakthrough discovery document
- **SPECTRALCANVAS_PRO_ROADMAP_2025.md:** Complete development timeline
- **Z_PLANE_FILTER_IMPLEMENTATION_SPEC.md:** Technical filter implementation
- **EMULATOR_X3_INTEGRATION_STRATEGY.md:** Archive analysis framework

---

## 🎯 SUCCESS METRICS

### Technical Success Criteria
- **Authentic Sound Recreation:** Producer recognition of EMU character
- **Performance Excellence:** <5ms latency with 64-voice polyphony
- **Stability:** No audio dropouts under maximum load
- **Integration:** Seamless DAW operation and workflow adoption

### Market Validation Criteria
- **Producer Recognition:** Electronic music producers identify authentic sound
- **Workflow Adoption:** Integration into professional production workflows
- **Educational Success:** Adoption in electronic music production courses
- **Community Validation:** Positive response from vintage synth communities

---

## 🚀 IMMEDIATE NEXT STEPS

### Week 1 Priorities
1. **Emulator X3 Archive Access:** Verify and document complete archive structure
2. **Analysis Environment:** Configure development tools and documentation systems
3. **Team Briefing:** Ensure all team members understand clean-room methodology
4. **Legal Framework:** Confirm compliance approach and create documentation standards

### Critical Dependencies
1. **Archive Access Confirmation:** Verify complete Emulator X3 installation availability
2. **Legal Compliance Framework:** Establish and document clean-room methodology
3. **Technical Architecture:** Confirm JUCE integration approach and performance targets
4. **Team Assembly:** Ensure specialized skills availability for archive analysis

---

## 🔗 RELATED DOCUMENTS

- **Project Brief:** `docs/SPECTRALCANVAS_PRO_PROJECT_BRIEF.md`
- **Development Roadmap:** `docs/SPECTRALCANVAS_PRO_ROADMAP_2025.md`
- **Technical Specifications:** `docs/research/EMU_AUDITY_2000_TECHNICAL_SPECS.md`
- **Implementation Strategy:** `docs/EMULATOR_X3_INTEGRATION_STRATEGY.md`

---

**Document Status:** ACTIVE - Development Ready  
**Last Updated:** August 26, 2025  
**Next Review:** Phase 1 Milestone Completion
