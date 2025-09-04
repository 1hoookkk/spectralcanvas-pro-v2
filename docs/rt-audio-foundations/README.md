# RT-Audio Foundations Knowledge System

## Overview

This directory contains curated DSP knowledge extracted from expert textbooks and authoritative online sources, specifically organized for real-time audio development and SpectralCanvasPro requirements.

## Contents

### Knowledge Cards (`card-*.md`)
Focused 2-3 page summaries of key DSP topics:
- **card-fft-fundamentals.md** - FFT theory, optimization, and RT constraints
- **card-stft-processing.md** - STFT, windowing, overlap-add reconstruction  
- **card-phase-vocoder.md** - Phase vocoder techniques and spectral manipulation
- **card-windowing-functions.md** - Window design, spectral leakage, trade-offs
- **card-filter-design.md** - IIR/FIR filters, zero-delay feedback, virtual analog
- **card-rt-safety-patterns.md** - Memory management, threading, latency patterns
- **card-juce-integration.md** - AudioProcessor lifecycle, parameter management
- **card-performance-optimization.md** - SIMD, buffer management, denormal handling

### Source Outlines (`outline-*.md`)
Per-PDF chapter summaries with page ranges:
- **outline-understanding-dsp.md** - Lyons: Understanding Digital Signal Processing
- **outline-audio-plugins-cpp.md** - Pirkle: Designing Audio Effect Plug-Ins in C++
- **outline-va-filters.md** - Virtual Analog Filter Design
- **outline-zolzer-dsp.md** - Zölzer: Digital Audio Signal Processing

### Search Index (`index.json`)
Machine-readable topic mapping:
```json
{
  "fft": [{"card": "card-fft-fundamentals.md", "confidence": 0.95, "pages": "45-78"}],
  "stft": [{"card": "card-stft-processing.md", "confidence": 0.92, "pages": "156-203"}],
  "latency": [{"card": "card-rt-safety-patterns.md", "confidence": 0.88}],
  // ... additional mappings
}
```

## Usage for Agents

### Primary Workflow
1. **Query phase**: Use `dsp-knowledge-loader` to retrieve relevant cards
2. **Reasoning phase**: Ground decisions in loaded knowledge with citations  
3. **Implementation phase**: Apply knowledge with RT-safety constraints

### Integration Examples

**For rt-safety-auditor:**
```markdown
Pre-read: Invoke dsp-knowledge-loader(topic_query="RT-safe STFT latency patterns") 
and include its Brief.md in your analysis.
```

**For code-review:**  
```markdown
Pre-read: Invoke dsp-knowledge-loader(topic_query inferred from diff context)
and cite relevant cards when making DSP-related recommendations.
```

**For general DSP questions:**
```markdown
Task: dsp-knowledge, "How should I implement overlap-add for 75% hop overlap?"
```

## Knowledge Source Authority

1. **SpectralCanvasPro codebase** (highest) - Actual implementation patterns
2. **Curated knowledge cards** (high) - Expert theory applied to RT audio  
3. **CLAUDE.md RT-safety rules** (absolute) - Non-negotiable constraints
4. **Original PDFs/CCRMA references** (detailed) - Mathematical foundations
5. **JUCE documentation** (implementation) - Framework integration patterns

## Citation Requirements

All technical claims must include:
- **Knowledge card reference**: `card-fft-fundamentals.md`  
- **Source material citation**: `Understanding DSP, pages 45-67`
- **Online reference** (if applicable): `CCRMA Filters: https://ccrma.stanford.edu/~jos/filters/`
- **Confidence level**: High (0.9+), Medium (0.7-0.9), Low (<0.7)

## Maintenance

### Adding New Knowledge
1. Run `pdf-curator` agent on new source materials
2. Update `index.json` with new topic mappings
3. Cross-reference with existing cards for consistency
4. Test integration with `dsp-knowledge-loader`

### Quality Assurance
- All cards must be ≤ 3 pages focused content
- Every technical claim requires explicit citation
- RT-safety considerations must be included for all topics
- Mathematical content should reference original sources

## SpectralCanvasPro Integration

### Core Constants Reference
```cpp
// From Source/Core/AtlasIds.h
FFT_SIZE = 512
HOP_SIZE = 128  
NUM_BINS = FFT_SIZE / 2 + 1 = 257
LATENCY = FFT_SIZE - HOP_SIZE = 384 samples
```

### Key Architecture Components
- **RealtimeSpectral**: STFT processing engine
- **HopScheduler**: UI-to-audio delta streaming  
- **MaskAtlas**: Double-buffered spectral mask storage
- **SpectralCanvasProAudioProcessor**: Main plugin integration

Knowledge cards should reference these components when providing implementation guidance specific to the project architecture.