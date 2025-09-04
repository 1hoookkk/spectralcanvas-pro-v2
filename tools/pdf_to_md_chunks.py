#!/usr/bin/env python3
"""
Chunk a PDF into ~N-word Markdown slices with small overlaps + a JSON manifest.

- Prefers pdfminer.six for extraction; falls back to pypdf if unavailable.
- Splits by paragraphs (double newlines), aims for target word count per chunk.
- Adds overlap words between chunks to maintain context across prompts.
- Writes chunk files + manifest.json with metadata (page ranges, word counts).

Usage:
  python pdf_to_md_chunks.py input.pdf out_dir --target 3000 --overlap 150 --title "DAFX (Zölzer)"
"""

from __future__ import annotations
import argparse, json, math, os, re, sys, textwrap
from datetime import datetime
from pathlib import Path
from typing import List, Tuple, Optional

# ------------------------- Text extraction helpers -------------------------

def extract_text_pdfminer(pdf_path: str) -> Tuple[str, List[Tuple[int, int]]]:
    """
    Returns (full_text, page_spans) where page_spans[i] = (start_index, end_index)
    character offsets for each page in concatenated text. Requires pdfminer.six.
    """
    try:
        from pdfminer.high_level import extract_pages
        from pdfminer.layout import LTTextContainer, LTAnno
    except Exception as e:
        raise ImportError("pdfminer.six not available") from e

    texts: List[str] = []
    spans: List[Tuple[int, int]] = []
    cursor = 0

    for page_layout in extract_pages(pdf_path):
        page_lines: List[str] = []
        # Collect text in reading order
        for element in page_layout:
            if isinstance(element, LTTextContainer):
                page_lines.append(element.get_text())
            elif isinstance(element, LTAnno):
                page_lines.append(str(element))
        page_text = "".join(page_lines)
        # Normalize line endings but keep paragraph blanks
        page_text = page_text.replace("\r\n", "\n").replace("\r", "\n")
        # Append page delimiter to keep page boundaries discoverable
        if not page_text.endswith("\n"):
            page_text += "\n"
        texts.append(page_text)
        spans.append((cursor, cursor + len(page_text)))
        cursor += len(page_text)

    full_text = "".join(texts)
    return full_text, spans


def extract_text_pypdf(pdf_path: str) -> Tuple[str, List[Tuple[int, int]]]:
    """Fallback extractor using pypdf (quality can be lower than pdfminer)."""
    try:
        from pypdf import PdfReader
    except Exception as e:
        raise ImportError("pypdf not available for fallback") from e

    reader = PdfReader(pdf_path)
    texts: List[str] = []
    spans: List[Tuple[int, int]] = []
    cursor = 0
    for i, page in enumerate(reader.pages):
        try:
            page_text = page.extract_text() or ""
        except Exception:
            page_text = ""
        page_text = page_text.replace("\r\n", "\n").replace("\r", "\n")
        if not page_text.endswith("\n"):
            page_text += "\n"
        texts.append(page_text)
        spans.append((cursor, cursor + len(page_text)))
        cursor += len(page_text)
    return "".join(texts), spans


# ------------------------- Cleaning & chunking -------------------------

_heading_line = re.compile(r"^\s*(?:[A-Z0-9][A-Z0-9 \-:().]{3,}|[0-9]+\.[0-9. ]+.+)\s*$")

def normalize_text(s: str) -> str:
    # Remove excessive whitespace while preserving paragraphs
    s = s.replace("\t", "    ")
    # Collapse >2 blank lines to 2
    s = re.sub(r"\n{3,}", "\n\n", s)
    # Trim trailing spaces
    s = "\n".join(line.rstrip() for line in s.splitlines())
    return s.strip()

def split_into_paragraphs(s: str) -> List[str]:
    # Split by blank lines; keep headings alone if they look like headings
    raw = s.split("\n\n")
    out: List[str] = []
    for para in raw:
        block = para.strip("\n")
        if not block:
            continue
        # If the first line is heading-ish and the paragraph is long, split heading
        first, *rest = block.split("\n")
        if _heading_line.match(first) and rest:
            out.append(first.strip())
            rest_text = "\n".join(rest).strip()
            if rest_text:
                out.append(rest_text)
        else:
            out.append(block)
    return out

def count_words(s: str) -> int:
    return len(re.findall(r"\b\w+\b", s))

def chunk_paragraphs(paragraphs: List[str], target_words: int, overlap_words: int) -> List[List[str]]:
    chunks: List[List[str]] = []
    buf: List[str] = []
    wcount = 0
    for p in paragraphs:
        pw = count_words(p)
        if wcount + pw > target_words and buf:
            chunks.append(buf)
            # Build overlap tail from the end of buf
            if overlap_words > 0:
                tail = build_overlap_tail(buf, overlap_words)
                buf = tail[:]  # start next chunk with overlap
                wcount = sum(count_words(x) for x in buf)
            else:
                buf = []
                wcount = 0
        buf.append(p)
        wcount += pw
    if buf:
        chunks.append(buf)
    return chunks

def build_overlap_tail(paras: List[str], overlap_words: int) -> List[str]:
    tail: List[str] = []
    acc = 0
    # walk backwards until we hit overlap_words
    for p in reversed(paras):
        tail.append(p)
        acc += count_words(p)
        if acc >= overlap_words:
            break
    return list(reversed(tail))


# ------------------------- Page range estimation -------------------------

def estimate_page_range(chunk_text: str, full_text: str, page_spans: List[Tuple[int, int]]) -> Tuple[int, int]:
    """
    Best-effort estimate of page range for a chunk by matching a short fingerprint.
    If not found, returns (1, len(page_spans)).
    """
    if not page_spans:
        return (1, 1)
    # fingerprint: first 200 non-whitespace chars
    key = re.sub(r"\s+", " ", chunk_text).strip()[:200]
    if not key:
        return (1, len(page_spans))
    pos = full_text.find(key)
    if pos == -1:
        return (1, len(page_spans))
    # map char offset to page index using spans
    start_page = 1
    end_page = len(page_spans)
    for i, (a, b) in enumerate(page_spans):
        if a <= pos < b:
            start_page = i + 1
            break
    # rough end using end of chunk
    end_pos = pos + len(key)
    for j, (a, b) in enumerate(page_spans):
        if a <= end_pos <= b:
            end_page = j + 1
            break
    return (start_page, end_page)


# ------------------------- Main runner -------------------------

def main():
    ap = argparse.ArgumentParser(description="Chunk a PDF into ~N-word Markdown slices with overlaps.")
    ap.add_argument("pdf", help="Input PDF path")
    ap.add_argument("out_dir", help="Output directory for chunks")
    ap.add_argument("--target", type=int, default=3000, help="Target words per chunk (default 3000)")
    ap.add_argument("--overlap", type=int, default=150, help="Word overlap between chunks (default 150)")
    ap.add_argument("--title", type=str, default="", help="Optional source title for headers")
    ap.add_argument("--prefix", type=str, default="", help="Optional prefix note for each chunk")
    ap.add_argument("--no-pdfminer", action="store_true", help="Force pypdf fallback (ignore pdfminer if present)")
    args = ap.parse_args()

    pdf_path = Path(args.pdf)
    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    # Extract
    try:
        if args.no_pdfminer:
            raise ImportError("forced fallback")
        full_text, spans = extract_text_pdfminer(str(pdf_path))
        extractor = "pdfminer.six"
    except Exception:
        full_text, spans = extract_text_pypdf(str(pdf_path))
        extractor = "pypdf"

    full_text = normalize_text(full_text)
    paragraphs = split_into_paragraphs(full_text)
    chunks = chunk_paragraphs(paragraphs, args.target, args.overlap)

    base = pdf_path.stem
    manifest = {
        "source_pdf": str(pdf_path),
        "title": args.title or base,
        "extracted_with": extractor,
        "target_words": args.target,
        "overlap_words": args.overlap,
        "created_utc": datetime.utcnow().isoformat() + "Z",
        "chunks": []
    }

    for idx, paras in enumerate(chunks, start=1):
        body = "\n\n".join(paras).strip()
        words = count_words(body)
        p1, p2 = estimate_page_range(body, full_text, spans)
        header_lines = [
            f"# {args.title or base} — Chunk {idx}/{len(chunks)}",
            "",
            f"> Source: `{pdf_path.name}` · Pages ~{p1}-{p2} · Extractor: {extractor}",
            f"> Target ≈ {args.target} words, Overlap = {args.overlap} words · Actual = {words}",
        ]
        if args.prefix:
            header_lines += ["", f"> Note: {args.prefix}"]
        header_lines.append("\n---\n")

        md = "\n".join(header_lines) + body + "\n"
        fname = f"{base}_chunk{idx:02d}_p{p1}-{p2}.md"
        (out_dir / fname).write_text(md, encoding="utf-8")

        manifest["chunks"].append({
            "index": idx,
            "file": fname,
            "approx_pages": [p1, p2],
            "word_count": words
        })

    (out_dir / "manifest.json").write_text(json.dumps(manifest, indent=2), encoding="utf-8")

    print(f"[ok] wrote {len(chunks)} chunks to {out_dir}")
    print(f"     manifest: {out_dir / 'manifest.json'}")
    print(f"     extractor: {extractor}")

if __name__ == "__main__":
    main()