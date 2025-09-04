#!/usr/bin/env python3
"""
Batch process DSP PDFs into chunked knowledge cards for SpectralCanvasPro.
Creates chunks optimized for feeding to Claude/Gemini with DSP knowledge extraction prompts.
"""

import subprocess
import sys
from pathlib import Path

def run_chunker(pdf_path: Path, title: str, out_dir: Path):
    """Run the PDF chunker with SpectralCanvasPro-optimized settings."""
    cmd = [
        sys.executable, "pdf_to_md_chunks.py",
        str(pdf_path),
        str(out_dir),
        "--target", "2800",  # Smaller chunks for better AI processing
        "--overlap", "200",   # More overlap for DSP context continuity
        "--title", title,
        "--prefix", "DSP textbook content for SpectralCanvasPro RT-audio knowledge base"
    ]
    
    print(f"Processing {pdf_path.name}...")
    result = subprocess.run(cmd, cwd=Path(__file__).parent)
    if result.returncode != 0:
        print(f"Error processing {pdf_path.name}")
        return False
    return True

def main():
    # Define PDFs and their titles
    pdf_configs = [
        ("VAFilterDesign_2.1.0.pdf", "VA Filter Design"),
        ("UnderstandingDigitalSignalProcessing.pdf", "Understanding DSP (Lyons)"),
        ("Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf", "Audio Plugins C++ (Pirkle)"),
        ("Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf", "Digital Audio DSP (Zölzer)")
    ]
    
    pdf_dir = Path("../sessions/knowledge/pdf")
    chunks_dir = Path("../docs/rt-audio-foundations/chunks")
    chunks_dir.mkdir(exist_ok=True)
    
    success_count = 0
    for pdf_name, title in pdf_configs:
        pdf_path = pdf_dir / pdf_name
        if not pdf_path.exists():
            print(f"Skipping missing: {pdf_name}")
            continue
            
        # Create title-specific output directory
        safe_title = title.lower().replace(" ", "-").replace("(", "").replace(")", "")
        out_dir = chunks_dir / safe_title
        
        if run_chunker(pdf_path, title, out_dir):
            success_count += 1
    
    print(f"\n[ok] Successfully processed {success_count}/{len(pdf_configs)} PDFs")
    print(f"Chunks written to: {chunks_dir}")
    print("\nNext steps:")
    print("1. Review chunk quality in docs/rt-audio-foundations/chunks/")
    print("2. Feed chunks to Claude/Gemini with golden_invariants_prompt.md")
    print("3. Collect knowledge cards in docs/rt-audio-foundations/")

if __name__ == "__main__":
    main()