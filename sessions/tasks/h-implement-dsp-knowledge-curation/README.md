---
task: h-implement-dsp-knowledge-curation
branch: feature/implement-dsp-knowledge-curation
status: in-progress
created: 2025-09-04
modules: [sessions, docs, .claude/agents]
---

# DSP Knowledge Curation System

## Problem/Goal
Transform the existing collection of expert DSP textbook PDFs (sessions/knowledge/pdf/) into a curated, searchable knowledge system that integrates seamlessly with Claude Code agents. The current raw PDFs cannot be efficiently referenced by specialized agents due to context limits, and there's no systematic way to apply expert DSP knowledge to RT-safety decisions.

## Success Criteria
- [ ] Create `pdf-curator` agent for automated knowledge extraction
- [ ] Create `dsp-knowledge-loader` agent for topic-based knowledge retrieval  
- [ ] Create `dsp-knowledge` agent for DSP-grounded code guidance
- [ ] Set up `docs/rt-audio-foundations/` knowledge repository structure
- [ ] Process 4 existing DSP textbooks into knowledge cards (2-3 pages each)
- [ ] Create searchable index.json mapping topics to cards and PDF pages
- [ ] Integrate DSP knowledge with `rt-safety-auditor` and `code-review` agents
- [ ] Validate knowledge system with sample DSP code reviews
- [ ] Create usage documentation and examples
- [ ] Ensure build workflow remains unaffected

## Context Files
- @sessions/knowledge/pdf/VAFilterDesign_2.1.0.pdf                                    # VA Filter Design textbook
- @sessions/knowledge/pdf/UnderstandingDigitalSignalProcessing.pdf                    # DSP fundamentals
- @sessions/knowledge/pdf/Designing Audio Effect Plug-Ins in C++...pdf               # Audio plugin development
- @sessions/knowledge/pdf/Udo-Zolzer-digital-audio-signal-processing...pdf          # Advanced DSP techniques
- @sessions/knowledge/pdf/links.txt                                                  # Additional references
- @.claude/agents/rt-safety-auditor.md                                              # RT-safety analysis agent
- @.claude/agents/code-review.md                                                    # Code review agent
- @Source/Core/AtlasIds.h                                                           # FFT constants and configuration
- @Source/DSP/RealtimeSpectral.h                                                    # STFT processing core
- @docs/Developer-Notes.md                                                          # RT-safety rules and patterns

## User Notes
Implementation approach follows **curate → distill → reference** pattern:
1. **Raw PDFs** stay in sessions/knowledge/pdf/ (don't modify originals)
2. **Knowledge cards** (2-3 pages each) go in docs/rt-audio-foundations/
3. **Agents** reference cards first, drill down to PDF pages only when needed
4. **Every claim** must be backed by explicit citations (card path + PDF page ranges)

Key integration points:
- rt-safety-auditor gets DSP expertise for latency, allocation, and threading decisions
- code-review gets advanced signal processing pattern recognition
- All agents maintain "compile-before-assert" discipline with build-validator

## Work Log
- [2025-09-04] Started task, created directory structure and main README