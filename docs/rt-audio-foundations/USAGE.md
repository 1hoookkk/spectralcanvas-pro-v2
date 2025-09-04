# DSP Knowledge System Usage Guide

## Quick Start 🚀

The SpectralCanvasPro DSP Knowledge System is now fully operational! Here's how to use it:

### 1. Processing Your PDFs (Done ✅)
You've successfully created chunks in `docs/rt-audio-foundations/chunks/`. Now you can:

### 2. Generate Knowledge Cards
**For each chunk file, follow this process:**

```bash
# Example workflow for a chunk:
# 1. Copy the golden prompt template
cat tools/golden_invariants_prompt.md

# 2. Add your chunk content to the prompt
# 3. Process with your preferred AI:

# Option A: Built-in Gemini
mcp__gemini-collab__ask_gemini "$(cat tools/golden_invariants_prompt.md)\n\n$(cat docs/rt-audio-foundations/chunks/understanding-dsp/UnderstandingDigitalSignalProcessing_chunk01_p1-15.md)"

# Option B: Claude Web/API (copy-paste)
# Option C: DeepSeek (copy-paste to https://chat.deepseek.com/)
```

### 3. Save Knowledge Cards
Save AI-generated cards as:
```
docs/rt-audio-foundations/card-[topic].md
```

Example card names:
- `card-fft-fundamentals.md`
- `card-stft-windowing.md`
- `card-phase-vocoder.md`
- `card-rt-safety-patterns.md`

### 4. Update Index
Add new cards to `docs/rt-audio-foundations/index.json`:
```json
{
  "your-topic": [
    {
      "card": "card-your-topic.md",
      "source": "YourPDF.pdf",
      "pages": "123-145", 
      "confidence": 0.92,
      "keywords": ["keyword1", "keyword2"]
    }
  ]
}
```

## Using the Knowledge System

### Direct Questions (Manual)
Since the agents aren't in the Task system yet, you can manually invoke the workflow:

**Step 1: Load Relevant Knowledge**
```markdown
# Find relevant topics in index.json
# Read the corresponding knowledge cards
# Extract key information for your question
```

**Step 2: Apply to Your Problem**
```markdown
# Use knowledge cards to inform DSP decisions
# Cross-reference with Julius O. Smith CCRMA for mathematical verification
# Apply RT-safety constraints from project context
```

### With rt-safety-auditor Integration ✅
The rt-safety-auditor now automatically considers DSP knowledge:
```bash
# This will work immediately:
Task: rt-safety-auditor, "Check my STFT implementation for RT-safety"
# Agent will reference DSP knowledge cards for windowing, latency, etc.
```

### With code-review Integration ✅  
The code-review agent is enhanced with DSP expertise:
```bash
# This will work immediately:
Task: code-review, "Review my FFT processing changes"
# Agent will apply DSP knowledge for correctness analysis
```

## Knowledge Sources Available

### 📚 CCRMA References (Ready to Use)
Located in: `docs/rt-audio-foundations/chunks/ccrma-references/`
- `ccrma-introduction-to-digital-filters.md`
- `ccrma-spectral-audio-signal-processing.md` 
- `ccrma-physical-audio-signal-processing.md`

**Authority Level**: 🏆 **GOLD STANDARD** - Use for mathematical foundations

### 📖 PDF Chunks (Ready for Processing)
Your chunks are organized by source:
- `chunks/understanding-dsp/` - DSP fundamentals
- `chunks/audio-plugins-cpp/` - JUCE/C++ patterns
- `chunks/va-filter-design/` - Advanced filter theory
- `chunks/digital-audio-dsp/` - Comprehensive coverage

## Recommended Processing Order

1. **Start with CCRMA references** (already processed)
2. **Process Understanding DSP chunks** - core theory foundation
3. **Process Audio Plugins C++ chunks** - JUCE integration patterns
4. **Process VA Filter Design chunks** - advanced filter theory
5. **Process Zölzer DSP chunks** - comprehensive coverage

## Example Workflow: Processing Your First Chunk

```bash
# 1. Pick a chunk (start with FFT fundamentals)
CHUNK="docs/rt-audio-foundations/chunks/understanding-dsp/UnderstandingDigitalSignalProcessing_chunk01_p1-15.md"

# 2. Create AI prompt
echo "$(cat tools/golden_invariants_prompt.md)" > temp_prompt.md
echo -e "\n---\n" >> temp_prompt.md
cat "$CHUNK" >> temp_prompt.md

# 3. Process with AI (copy temp_prompt.md to Claude/Gemini/DeepSeek)
# 4. Save result as card-fft-fundamentals.md
# 5. Update index.json with new entry
```

## Quality Checklist for Generated Cards

Ensure every card has:
- [ ] **Title**: Clear topic identification
- [ ] **Core Concepts**: 5-7 bullet points
- [ ] **RT-Audio Considerations**: DO/DON'T lists
- [ ] **SpectralCanvasPro Integration**: Project-specific guidance
- [ ] **Sources**: Explicit [PDF, pages X-Y] citations
- [ ] **CCRMA Cross-reference**: When applicable
- [ ] **Length**: ≤ 3 pages focused content

## Integration Status

### ✅ Ready Now:
- **PDF chunking system**
- **CCRMA reference cards**
- **Enhanced rt-safety-auditor**
- **Enhanced code-review agent**
- **Golden invariants prompt template**
- **Multi-AI model support**

### 🔄 Manual Process (Working):
- **Knowledge card generation** (via AI + template)
- **Index.json updates** (manual editing)
- **Direct knowledge lookup** (read cards manually)

### 🚧 Future Enhancement:
- **Automated dsp-knowledge agent** (register in Task system)
- **Automated index updates** (after card generation)
- **Search interface** (query knowledge base directly)

## Troubleshooting

### "Can't find chunks"
Ensure chunks were created: `python tools/build_dsp_knowledge_base.py`

### "AI responses lack citations"
Emphasize citation requirements in your prompts: "Every technical claim must include [Source.pdf, pages X-Y]"

### "Cards too theoretical"
Add to prompt: "Focus on practical implementation for real-time audio processing"

### "Missing RT-safety considerations"
Verify golden_invariants_prompt.md includes RT-safety sections and SpectralCanvasPro context

## Next Steps

1. **Process 2-3 chunks** with your preferred AI model
2. **Save knowledge cards** in docs/rt-audio-foundations/
3. **Test rt-safety-auditor** with DSP code changes
4. **Refine prompts** based on initial results
5. **Scale up processing** across remaining chunks

The foundation is complete - now you can systematically transform your DSP textbooks into actionable, searchable knowledge cards that integrate seamlessly with your development workflow! 🎛️