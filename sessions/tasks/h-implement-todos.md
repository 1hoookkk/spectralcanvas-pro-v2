---
task: h-implement-todos
branch: none
status: in-progress
created: 2025-09-03
modules: [Core, DSP, Audio]
---

# Implement Concrete TODOs from Codebase

## Problem/Goal
Address the specific TODO items found in the actual codebase to improve functionality and completeness. Focus on grounded implementation without hallucinations - only work on what actually exists.

## Success Criteria
- [x] ✅ Identify concrete TODOs from existing code
- [x] ✅ Fix STFT latency configuration in SpectralCanvasProAudioProcessor (line 725)
- [x] ✅ Wire respeed parameter to spectral player (line 756) 
- [x] ✅ Implement XRun detection infrastructure (line 1020)
- [x] ✅ Verify all changes build successfully
- [x] ✅ Ensure RT-safety compliance for all modifications

## Context Files
- @Source/SpectralCanvasProAudioProcessor.cpp:725  # STFT latency TODO
- @Source/SpectralCanvasProAudioProcessor.cpp:756  # Respeed parameter TODO  
- @Source/SpectralCanvasProAudioProcessor.cpp:1020 # XRun detection TODO
- @Source/DSP/SpectralEngine.h                     # FFT constants
- @docs/Developer-Notes.md                         # RT-safety rules
- @CMakeLists.txt                                  # Build configuration

## User Notes
- Focus on streamlined workflow with no AI hallucinations
- Only modify files that actually exist
- Follow existing patterns from implemented components
- Use actual constants and infrastructure already in place

## Work Log
- [2025-09-03] Started task, analyzed codebase for concrete TODOs
- [2025-09-03] Found 3 specific TODO items in audio processor
- [2025-09-03] Created task tracking and todo list
- [2025-09-03] Fixed STFT latency configuration: uncommented setLatencySamples(384) with proper constants
- [2025-09-03] Wired respeed parameter: added setRespeed() method to SpectralPlayer, implemented fractional frame advance
- [2025-09-03] Implemented XRun detection: added atomic counter and buffer size monitoring in processBlock()
- [2025-09-03] Fixed compilation errors: replaced canPush() with hasSpaceAvailable(), fixed syntax error in BottomBar.cpp
- [2025-09-03] Build successful: VST3 and Standalone targets built with only warnings (no errors)
- [2025-09-03] Task completed - all TODOs addressed with RT-safe implementations