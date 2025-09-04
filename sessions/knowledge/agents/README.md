# Project Subagents (Claude Code)

These are ready-to-use agent definitions for Claude Code, stored alongside project knowledge in `sessions/knowledge`.

How to use with Claude Code
- Open the `/agents` UI in Claude Code.
- Create a new Project agent and paste the contents of one of these files.
- Optionally, save a copy under `.claude/agents/` to enable auto-discovery.

Agents
- build-tester.md — Configure, build, run tests, pluginval
- rt-audio-sheriff.md — Enforce RT-safety (no allocs/locks/logging in `processBlock`)
- dsp-knowledge.md — Ground answers in `sessions/knowledge/index.json` + cards
- roadmap-editor.md — Keep README roadmap in sync with code
- debugger.md — Root-cause analysis + minimal fix loop

Notes
- Knowledge index: `sessions/knowledge/index.json`
- Cards: `sessions/knowledge/pdf/cards/`
- CCRMA links: `sessions/knowledge/pdf/links.txt`
- Build tips: see root `README.md` and `CMakePresets.json`
