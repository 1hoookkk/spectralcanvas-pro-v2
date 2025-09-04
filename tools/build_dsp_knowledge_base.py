#!/usr/bin/env python3
"""
Master workflow script for building the SpectralCanvasPro DSP knowledge base.

Processes PDFs, creates CCRMA reference cards, and sets up the complete
knowledge curation system for real-time audio development.
"""

import subprocess
import sys
from pathlib import Path

def main():
    print("🎛️  Building SpectralCanvasPro DSP Knowledge Base")
    print("=" * 50)
    
    tools_dir = Path(__file__).parent
    project_root = tools_dir.parent
    
    print("\n1️⃣  Installing Python dependencies...")
    deps = ["pdfminer.six", "pypdf", "requests"]
    for dep in deps:
        print(f"   Installing {dep}...")
        subprocess.run([sys.executable, "-m", "pip", "install", dep], 
                      capture_output=True)
    
    print("\n2️⃣  Creating CCRMA reference cards...")
    result = subprocess.run([sys.executable, "process_ccrma_links.py"], 
                           cwd=tools_dir)
    if result.returncode == 0:
        print("   ✅ CCRMA reference cards created")
    else:
        print("   ❌ Error creating CCRMA cards")
    
    print("\n3️⃣  Processing DSP PDF textbooks...")
    result = subprocess.run([sys.executable, "process_dsp_pdfs.py"], 
                           cwd=tools_dir)
    if result.returncode == 0:
        print("   ✅ PDF chunks created")
    else:
        print("   ❌ Error processing PDFs (check if files exist)")
    
    chunks_dir = project_root / "docs/rt-audio-foundations/chunks"
    print(f"\n4️⃣  Knowledge base structure created:")
    print(f"   📁 Chunks: {chunks_dir}")
    print(f"   📄 Golden prompt: {tools_dir / 'golden_invariants_prompt.md'}")
    
    print("\n🎯 Next Steps:")
    print("   1. Review chunks in docs/rt-audio-foundations/chunks/")
    print("   2. For each chunk, use tools/golden_invariants_prompt.md with Claude/Gemini")
    print("   3. Save knowledge cards as card-*.md in docs/rt-audio-foundations/")
    print("   4. Update index.json with new entries")
    print("   5. Test with: Task(dsp-knowledge, 'your DSP question')")
    
    print("\n📚 Recommended processing order:")
    print("   • CCRMA references (already done) - use as mathematical foundation")
    print("   • Understanding DSP chunks - core theory")
    print("   • Audio Plugins C++ chunks - JUCE patterns") 
    print("   • VA Filter Design chunks - advanced filters")
    print("   • Zölzer DSP chunks - comprehensive coverage")
    
    print(f"\n✨ DSP knowledge base foundation ready!")

if __name__ == "__main__":
    main()