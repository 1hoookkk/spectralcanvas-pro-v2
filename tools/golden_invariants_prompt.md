# DSP Knowledge Card Extraction Prompt

You are a DSP expert creating concise knowledge cards for real-time audio developers working on SpectralCanvasPro, an STFT spectral masking plugin.

## Your Task
Transform the following PDF chunk into a focused **knowledge card** (2-3 pages max) following this structure:

## Knowledge Card Template

```markdown
# [Topic Name]

## Core Concepts
- [5-7 key bullet points of essential ideas]
- [Focus on practical application over pure theory]
- [Mathematical foundations where directly relevant]

## Key Algorithms/Equations
```
// Compact pseudocode or essential equations
// Include computational complexity: O(n log n)
// Cite: [Source PDF, pages X-Y]
```

## RT-Audio Specific Considerations

### ✅ DO:
- Preallocate all buffers in prepareToPlay()
- Use lock-free SPSC queues for UI→Audio communication
- Apply ScopedNoDenormals in processBlock()
- Maintain latency = FFT_SIZE - HOP_SIZE (384 samples)

### ❌ DON'T:
- Allocate memory in processBlock() (new, malloc, vector resize)
- Use std::mutex or any locks in audio thread
- Call system functions (logging, file I/O, networking)
- Resize containers dynamically during processing

## When to Use
- [Specific use cases and scenarios]
- [Performance characteristics and trade-offs] 
- [Latency implications for real-time processing]

## SpectralCanvasPro Integration
- [How this relates to STFT masking pipeline]
- [Integration with HopScheduler, MaskAtlas, RealtimeSpectral]
- [JUCE AudioProcessor lifecycle considerations]

## Sources & References
- [Source PDF, pages X-Y]: [Specific concepts extracted]
- Julius O. Smith CCRMA: [Relevant online reference if applicable]
  - Introduction to Digital Filters: https://ccrma.stanford.edu/~jos/filters/
  - Spectral Audio Signal Processing: https://ccrma.stanford.edu/~jos/sasp/
  - Physical Audio Signal Processing: https://ccrma.stanford.edu/~jos/pasp/
- Cross-references: [Related knowledge cards]
```

## Julius O. Smith CCRMA Integration Guidelines

When the PDF chunk relates to topics covered by Julius O. Smith's CCRMA books, **cross-reference and prioritize CCRMA content**:

- **FFT/DFT topics** → Reference "Introduction to Digital Filters" 
- **STFT/Spectral processing** → Reference "Spectral Audio Signal Processing"
- **Filter design** → Reference both "Filters" and relevant PDF content
- **Physical modeling/synthesis** → Reference "Physical Audio Signal Processing"

**CCRMA Content Priority**: Julius O. Smith's work is **authoritative** - if there's any conflict between PDF content and CCRMA, note both but flag CCRMA as the mathematical standard.

## Quality Checklist

Before outputting your knowledge card, verify:

- [ ] **Conciseness**: Card is ≤3 pages of focused content
- [ ] **Traceability**: Every technical claim cites [Source PDF, pages X-Y]
- [ ] **RT-Safety**: Includes real-time constraints and patterns
- [ ] **Practicality**: Emphasizes actionable guidance over pure theory
- [ ] **SpectralCanvasPro relevance**: Connects to STFT masking workflow
- [ ] **CCRMA integration**: Cross-references Julius O. Smith when applicable
- [ ] **Accuracy**: Preserves technical precision while simplifying presentation

## Context: SpectralCanvasPro Architecture

You're creating knowledge for developers working on:
- **Real-time STFT processing** with 512-point FFT, 128-sample hop
- **Lock-free UI→Audio communication** via MaskDeltaQueue
- **Spectral masking pipeline** where painting gestures become frequency-domain attenuation
- **Strict RT-safety requirements** (no allocs, no locks, <5ms latency)
- **JUCE-based audio plugin** (VST3/AU) with modern C++

Focus on knowledge that **directly serves this architecture**.

---

**Now process the following PDF chunk and create a knowledge card:**

[PASTE PDF CHUNK HERE]