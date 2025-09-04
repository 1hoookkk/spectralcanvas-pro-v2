# Using DSP Knowledge Chunks with AI Models

## Quick Start
1. **Generate chunks**: `python tools/build_dsp_knowledge_base.py`
2. **Find chunks**: `docs/rt-audio-foundations/chunks/`
3. **Copy prompt**: `tools/golden_invariants_prompt.md` + paste chunk
4. **Process with AI**: Follow model-specific instructions below

## Model-Specific Instructions

### 🤖 Claude (Anthropic)
**Best for**: Mathematical rigor, code quality, complex reasoning

```bash
# In Claude interface:
1. Paste tools/golden_invariants_prompt.md
2. Add: "[PASTE PDF CHUNK HERE]" → paste your chunk content
3. Submit for knowledge card generation
```

**Claude Web/API Tips**:
- Works best with 2800-word chunks (default setting)
- Excellent mathematical symbol handling
- Strong at citing sources properly
- Good at maintaining SpectralCanvasPro context

### 🔮 Gemini (Google)
**Best for**: Creative solutions, broad knowledge synthesis

**Using mcp__gemini-collab (built-in):**
```bash
# From SpectralCanvasPro directory:
# Use built-in Gemini collaboration tools
mcp__gemini-collab__ask_gemini "$(cat tools/golden_invariants_prompt.md)\n\n[PASTE CHUNK HERE]"
```

**Using Gemini Web Interface:**
1. Go to https://gemini.google.com/
2. Paste `tools/golden_invariants_prompt.md`
3. Add your PDF chunk content
4. Submit for knowledge card generation

**Gemini-Specific Tips**:
- Good with overlap-buffered chunks (handles context well)
- May need reminders about RT-safety constraints
- Excellent at cross-referencing CCRMA sources

### 🚀 DeepSeek (DeepSeek AI)
**Best for**: Code optimization, performance analysis

**Using DeepSeek Chat:**
1. Go to https://chat.deepseek.com/
2. Paste the golden invariants prompt
3. Add your PDF chunk
4. Request knowledge card generation

**DeepSeek-Specific Optimizations:**
- Emphasize code performance aspects in your prompts
- Great for RT-safety pattern analysis
- Strong at suggesting SIMD optimizations
- May need explicit reminders about Julius O. Smith authority

## Batch Processing Workflow

### Option 1: Sequential Processing
```bash
# Process chunks one by one
for chunk in docs/rt-audio-foundations/chunks/*/*.md; do
  echo "Processing: $chunk"
  # Copy chunk + prompt to your preferred AI
  # Save result as card-*.md in docs/rt-audio-foundations/
done
```

### Option 2: Parallel Processing (Multiple AI Models)
```bash
# Split chunks between models for speed:
# Claude: STFT/FFT theory chunks (mathematical rigor)
# Gemini: Filter design chunks (creative synthesis) 
# DeepSeek: Performance optimization chunks (code focus)
```

## Prompt Customization for Each Model

### For Claude:
```markdown
# Add to golden_invariants_prompt.md:
**Additional Instructions for Claude**:
Focus on mathematical precision and proper citation format. 
Ensure all DSP equations reference Julius O. Smith CCRMA when applicable.
```

### For Gemini:
```markdown
# Add to golden_invariants_prompt.md:
**Additional Instructions for Gemini**:
Synthesize information creatively while maintaining technical accuracy.
Cross-reference multiple sources when creating practical guidance.
```

### For DeepSeek:
```markdown
# Add to golden_invariants_prompt.md:
**Additional Instructions for DeepSeek**:
Emphasize performance optimization and RT-safety patterns.
Suggest SIMD optimizations and memory-efficient implementations.
```

## Quality Assurance Checklist

After getting knowledge cards from any AI model, verify:

- [ ] **Citations present**: Every technical claim has [Source.pdf, pages X-Y]
- [ ] **RT-safety included**: Cards mention real-time constraints
- [ ] **SpectralCanvasPro relevance**: Connects to STFT masking architecture
- [ ] **CCRMA integration**: References Julius O. Smith when applicable
- [ ] **Actionable content**: Provides practical implementation guidance
- [ ] **Proper length**: ≤3 pages focused content

## Troubleshooting Common Issues

### "Chunk too long" errors:
- Reduce `--target` in `pdf_to_md_chunks.py` to 2000-2500 words
- Split complex chapters manually

### "AI loses context" issues:
- Increase `--overlap` to 250-300 words
- Include more SpectralCanvasPro context in prompt

### "Citations missing" problems:
- Emphasize citation requirements in custom prompt additions
- Post-process cards to add missing [Source.pdf, pages X-Y] references

### "RT-safety ignored" issues:
- Add model-specific reminders about real-time constraints
- Include more examples of RT-unsafe patterns to avoid

## Integration with SpectralCanvasPro Workflow

1. **Generate cards** using your preferred AI model
2. **Save cards** as `docs/rt-audio-foundations/card-*.md`
3. **Update index.json** with new topic mappings
4. **Test integration** with `Task(dsp-knowledge, "your DSP question")`
5. **Use in development** via rt-safety-auditor and code-review agents

## Performance Tips

- **Batch similar chunks** to the same model for consistency
- **Process CCRMA-heavy topics** with Claude for mathematical rigor
- **Use Gemini for creative synthesis** of complex topics
- **Use DeepSeek for performance-critical** algorithm analysis
- **Save successful prompts** as templates for future processing