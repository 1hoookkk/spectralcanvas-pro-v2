# dsp-knowledge-loader

## Goal
Given a specific task or topic query, load only the most relevant DSP knowledge cards and provide a concise brief for the calling agent. Acts as an intelligent knowledge retrieval system that prevents context overload while ensuring relevant expert knowledge is available.

## Inputs
- `index_path`: Path to searchable knowledge index (default: ./docs/rt-audio-foundations/index.json)
- `cards_dir`: Directory containing knowledge cards (default: ./docs/rt-audio-foundations/)
- `topic_query`: Free-text description of the task or topic (e.g., "phase vocoder overlap-add", "latency reporting FFT-HOP", "RT-safe JUCE processBlock rules")
- `max_cards`: Maximum number of knowledge cards to load (default: 5)
- `allow_pdf_pages`: Whether to reference specific PDF pages for gaps (default: true)

## Process

### Phase 1: Topic Analysis
1. Parse `topic_query` to extract key DSP concepts:
   - Signal processing terms (FFT, STFT, windowing, filters, etc.)
   - RT-audio constraints (latency, allocation, threading)
   - Implementation context (JUCE, VST3, processBlock)
   - Performance requirements (real-time, buffer sizes)

2. Map query terms to index entries using fuzzy matching:
   - Direct term matches (high confidence)
   - Related concept matches (medium confidence)  
   - Context-based matches (lower confidence)

### Phase 2: Knowledge Card Selection
1. Load `index.json` and score all entries against `topic_query`
2. Select top `max_cards` knowledge cards by relevance score
3. Load the actual markdown content for selected cards
4. Extract 10-15 most relevant bullet points per card

### Phase 3: Gap Analysis (if allow_pdf_pages=true)
1. Identify knowledge gaps in selected cards
2. Find specific PDF page ranges that might fill gaps
3. Add pinpoint references (PDF name + page range) to supplement cards
4. Prioritize Julius O. Smith CCRMA references for mathematical foundations

### Phase 4: Brief Generation
Create concise `Brief.md` containing:

**Structure:**
```markdown
# DSP Knowledge Brief: [Topic Summary]

## Key Principles
- [10-15 bullet points from selected cards]
- [Core concepts relevant to the query]
- [Mathematical foundations if applicable]

## RT-Audio Specific Considerations  
- [Latency implications]
- [Memory allocation patterns]
- [Threading safety requirements]
- [Performance characteristics]

## Implementation Guidance
- [When to use specific techniques]
- [Common pitfalls to avoid]
- [Integration patterns with JUCE/C++]

## Authority Citations
**Knowledge Cards:**
- card-fft-fundamentals.md (confidence: 0.95)
- card-windowing-functions.md (confidence: 0.88)

**Direct References:**
- Understanding DSP, pages 145-167 (overlap-add reconstruction)
- CCRMA Spectral Processing: https://ccrma.stanford.edu/~jos/sasp/ (phase vocoder)

**Cross-References:**
- Related cards: [list]
- Project files: Source/DSP/RealtimeSpectral.h:45-78
```

## Query Pattern Examples

**Latency-focused queries:**
- "FFT latency reporting in audio plugins"
- "STFT hop size and processing delay"
- "Real-time constraints for spectral processing"

**Implementation-focused queries:**  
- "Thread-safe FFT in JUCE processBlock"
- "Memory allocation patterns for STFT"
- "Phase vocoder implementation in C++"

**Algorithm-focused queries:**
- "Overlap-add windowing for STFT"
- "Phase coherence in spectral masking"
- "Filter design for real-time audio"

## Output Contract
- Generate only `Brief.md` (≤ 1 page focused content)
- List all referenced card paths for calling agent
- Include confidence scores for knowledge sources
- Provide specific PDF page references when gaps exist
- Never modify source knowledge cards or index

## Integration Notes
- Called by other agents before DSP-related reasoning
- Results feed directly into specialized agents (rt-safety-auditor, code-review)
- Brief content is optimized for agent context windows
- Citations enable human verification of technical claims

## Performance Optimization
- Cache frequently-used knowledge cards in memory
- Precompute common topic-to-card mappings
- Use lightweight text matching for initial filtering
- Load full card content only for selected cards

## Error Handling
- Gracefully handle missing index files
- Return partial results if some cards are unavailable
- Flag low-confidence matches in Brief.md
- Suggest alternative query terms for zero-result searches