#!/usr/bin/env python3
"""
Process Julius O. Smith CCRMA online references into manageable chunks.
Fetches key sections and creates structured content for knowledge card creation.
"""

import json
import requests
from pathlib import Path
from datetime import datetime

def fetch_ccrma_section(url: str, title: str) -> dict:
    """Fetch a CCRMA page and return metadata + content summary."""
    try:
        response = requests.get(url, timeout=30)
        response.raise_for_status()
        
        # For now, create a stub that documents the resource
        # In practice, you'd want to parse HTML content intelligently
        return {
            "url": url,
            "title": title,
            "status": "available",
            "content_type": "online_reference",
            "extraction_method": "manual_reference",
            "access_date": datetime.utcnow().isoformat() + "Z"
        }
    except Exception as e:
        return {
            "url": url, 
            "title": title,
            "status": "error",
            "error": str(e),
            "access_date": datetime.utcnow().isoformat() + "Z"
        }

def create_ccrma_reference_cards():
    """Create reference cards for Julius O. Smith CCRMA resources."""
    
    ccrma_resources = [
        {
            "title": "Introduction to Digital Filters",
            "url": "https://ccrma.stanford.edu/~jos/filters/",
            "topics": ["digital filter design", "frequency response", "z-transform", "filter implementation"],
            "relevance": "High - fundamental for any filter design in SpectralCanvasPro"
        },
        {
            "title": "Spectral Audio Signal Processing", 
            "url": "https://ccrma.stanford.edu/~jos/sasp/",
            "topics": ["STFT", "phase vocoder", "spectral effects", "windowing", "overlap-add"],
            "relevance": "Critical - core theory for STFT masking pipeline"
        },
        {
            "title": "Physical Audio Signal Processing",
            "url": "https://ccrma.stanford.edu/~jos/pasp/",
            "topics": ["real-time synthesis", "computational efficiency", "delay lines", "digital waveguides"],
            "relevance": "Medium - useful for performance optimization patterns"
        }
    ]
    
    chunks_dir = Path("../docs/rt-audio-foundations/chunks/ccrma-references")
    chunks_dir.mkdir(parents=True, exist_ok=True)
    
    manifest = {
        "source": "Julius O. Smith CCRMA Online Books",
        "created_utc": datetime.utcnow().isoformat() + "Z",
        "extraction_method": "reference_cards",
        "note": "CCRMA resources are authoritative - use as primary mathematical reference",
        "resources": []
    }
    
    for resource in ccrma_resources:
        # Create reference card
        card_content = f"""# {resource['title']} (Julius O. Smith)

> Source: CCRMA Stanford · URL: {resource['url']}  
> Authority Level: **GOLD STANDARD** (mathematical foundation reference)  
> SpectralCanvasPro Relevance: {resource['relevance']}

---

## Overview

This is a **reference pointer** to Julius O. Smith's authoritative online DSP textbook. The CCRMA books are considered the mathematical gold standard for digital signal processing theory and should be consulted for:

## Key Topic Coverage
{chr(10).join(f'- **{topic.title()}**' for topic in resource['topics'])}

## Usage in SpectralCanvasPro Context

### Primary Applications:
- Mathematical verification of DSP algorithm implementations
- Understanding theoretical foundations of STFT processing
- Filter design principles for spectral masking
- Performance optimization based on computational complexity analysis

### Integration with PDF Knowledge Cards:
When PDF textbook content conflicts with or lacks mathematical rigor compared to CCRMA content, **prioritize Julius O. Smith's formulations** as the authoritative reference.

## RT-Audio Considerations

While CCRMA books focus on mathematical theory, apply this knowledge with SpectralCanvasPro's RT constraints:

### ✅ Extract Theory For:
- Algorithm selection (O(n log n) FFT vs O(n²) DFT)
- Windowing function trade-offs (main lobe width vs side lobe suppression)
- Filter coefficient calculation (offline, in prepareToPlay())
- Spectral reconstruction accuracy analysis

### ❌ Don't Apply Directly:
- Arbitrary precision arithmetic (use float32 for RT performance)
- Infinite-length filter impulse responses (must be finite for RT)
- Complex number libraries without SIMD optimization
- Floating-point operations without denormal protection

## Access Strategy

1. **For specific equations**: Navigate directly to relevant sections
2. **For algorithm comparisons**: Use index to find trade-off discussions  
3. **For mathematical proofs**: Reference when designing new spectral algorithms
4. **For implementation details**: Cross-reference with RT-safe programming patterns

## Cross-References

- **Related Knowledge Cards**: [Will be populated as PDF cards are created]
- **SpectralCanvasPro Integration**: Source/DSP/RealtimeSpectral.h, Source/Core/AtlasIds.h
- **JUCE Framework Docs**: AudioProcessor, FFT classes

## Authority Level: 🏆 GOLD STANDARD

Julius O. Smith's work represents the mathematical foundation that other DSP texts build upon. When in doubt about theoretical correctness, consult CCRMA first.

---

**Direct Link**: [{resource['url']}]({resource['url']})
"""
        
        # Write reference card
        safe_title = resource['title'].lower().replace(' ', '-').replace(',', '')
        card_file = chunks_dir / f"ccrma-{safe_title}.md"
        card_file.write_text(card_content)
        
        # Add to manifest
        manifest['resources'].append({
            "title": resource['title'],
            "file": card_file.name,
            "url": resource['url'],
            "topics": resource['topics'],
            "relevance": resource['relevance']
        })
    
    # Write manifest
    manifest_file = chunks_dir / "manifest.json"
    manifest_file.write_text(json.dumps(manifest, indent=2))
    
    print(f"[ok] Created {len(ccrma_resources)} CCRMA reference cards")
    print(f"     Location: {chunks_dir}")
    print(f"     Manifest: {manifest_file}")

if __name__ == "__main__":
    create_ccrma_reference_cards()