#!/usr/bin/env python3
"""
Convert local PDFs and a list of web links into lightweight Markdown notes
under sessions/knowledge/md, and maintain an index.json for Claude to reference.

Requirements (install automatically via PowerShell wrapper or manually):
  pip install pypdf readability-lxml html2text requests lxml

Usage:
  python tools/knowledge_ingest.py \
    --pdf-dir sessions/knowledge/pdf \
    --links-file sessions/knowledge/links.txt \
    --out-dir sessions/knowledge/md \
    --index sessions/knowledge/index.json
"""
import argparse
import json
import os
import re
import sys
from datetime import datetime
import plistlib
from pathlib import Path


def slugify(text: str) -> str:
    text = re.sub(r"[^A-Za-z0-9\-_. ]+", "", text).strip().lower()
    text = re.sub(r"[\s]+", "-", text)
    return text[:120] if text else "doc"


def read_pdf(path: Path) -> str:
    try:
        from pypdf import PdfReader
    except Exception as e:
        print(f"[warn] pypdf not available: {e}. Skipping PDF: {path}")
        return ""
    try:
        reader = PdfReader(str(path))
        chunks = []
        for page in reader.pages:
            text = page.extract_text() or ""
            chunks.append(text)
        return "\n".join(chunks)
    except Exception as e:
        print(f"[warn] Failed to read PDF {path}: {e}")
        return ""


def fetch_url(url: str) -> tuple[str, str]:
    """Return (title, markdown_text) for the URL, best-effort."""
    try:
        import requests
    except Exception as e:
        print(f"[warn] requests not available: {e}. Skipping URL: {url}")
        return "", ""
    try:
        resp = requests.get(url, timeout=20)
        resp.raise_for_status()
        html = resp.text
    except Exception as e:
        print(f"[warn] Failed to fetch {url}: {e}")
        return "", ""

    # Try readability for main content
    title = ""
    try:
        from readability import Document
        doc = Document(html)
        title = (doc.short_title() or "").strip()
        content_html = doc.summary(html_partial=True)
    except Exception:
        content_html = html

    # Convert HTML to Markdown
    try:
        import html2text
        h = html2text.HTML2Text()
        h.ignore_links = False
        md = h.handle(content_html)
    except Exception:
        # Fallback: very rough text-only
        md = re.sub(r"<[^>]+>", "", content_html)

    return title, md


def parse_links_from_text_file(path: Path) -> list[str]:
    urls: list[str] = []
    try:
        for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if line.lower().startswith("http://") or line.lower().startswith("https://"):
                urls.append(line)
    except Exception as e:
        print(f"[warn] Failed to read links file {path}: {e}")
    return urls


def parse_windows_url_shortcut(path: Path) -> str:
    """Parse a .url InternetShortcut file and return URL if present."""
    try:
        # Simple INI-like parsing
        url = ""
        for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
            if line.strip().lower().startswith("url="):
                url = line.split("=", 1)[1].strip()
                break
        if url.lower().startswith("http://") or url.lower().startswith("https://"):
            return url
    except Exception:
        pass
    return ""


def parse_macos_webloc(path: Path) -> str:
    """Parse a .webloc plist file and return URL if present."""
    try:
        with path.open("rb") as f:
            data = plistlib.load(f)
        url = data.get("URL", "")
        if isinstance(url, bytes):
            url = url.decode("utf-8", errors="ignore")
        if url.lower().startswith("http://") or url.lower().startswith("https://"):
            return url
    except Exception:
        pass
    return ""


def unique_outpath(out_dir: Path, base_slug: str) -> Path:
    candidate = out_dir / f"{base_slug}.md"
    if not candidate.exists():
        return candidate
    i = 2
    while True:
        candidate = out_dir / f"{base_slug}-{i}.md"
        if not candidate.exists():
            return candidate
        i += 1


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--pdf-dir", type=str, default="sessions/knowledge/pdf")
    ap.add_argument("--links-file", type=str, default="sessions/knowledge/links.txt")
    ap.add_argument("--out-dir", type=str, default="sessions/knowledge/md")
    ap.add_argument("--index", type=str, default="sessions/knowledge/index.json")
    args = ap.parse_args()

    pdf_dir = Path(args.pdf_dir)
    links_file = Path(args.links_file)
    out_dir = Path(args.out_dir)
    index_path = Path(args.index)

    out_dir.mkdir(parents=True, exist_ok=True)
    index_path.parent.mkdir(parents=True, exist_ok=True)

    # Load existing index
    index = {"docs": []}
    if index_path.exists():
        try:
            index = json.loads(index_path.read_text())
            if not isinstance(index, dict) or "docs" not in index:
                index = {"docs": []}
        except Exception:
            index = {"docs": []}

    now = datetime.utcnow().isoformat() + "Z"

    # Ingest PDFs
    if pdf_dir.exists():
        for pdf in sorted(pdf_dir.glob("*.pdf")):
            text = read_pdf(pdf)
            if not text.strip():
                continue
            title = pdf.stem
            slug = slugify(title)
            out_file = unique_outpath(out_dir, slug)
            meta = [
                f"---",
                f"title: {title}",
                f"source: {pdf.as_posix()}",
                f"ingested: {now}",
                f"type: pdf",
                f"---",
                "",
            ]
            out_file.write_text("\n".join(meta) + text)
            index["docs"].append({
                "title": title,
                "source": pdf.as_posix(),
                "path": out_file.as_posix(),
                "type": "pdf",
                "ingested": now,
            })
            print(f"[ok] PDF -> {out_file}")

    # Gather links from multiple sources
    link_sources: list[str] = []
    seen = set()

    # Primary links file
    if links_file.exists():
        for u in parse_links_from_text_file(links_file):
            if u not in seen:
                link_sources.append(u)
                seen.add(u)

    # Fallback: links.txt under the PDF directory
    fallback_links = pdf_dir / "links.txt"
    if fallback_links.exists():
        for u in parse_links_from_text_file(fallback_links):
            if u not in seen:
                link_sources.append(u)
                seen.add(u)

    # Windows .url shortcuts in the PDF directory
    if pdf_dir.exists():
        for url_file in sorted(pdf_dir.glob("*.url")):
            u = parse_windows_url_shortcut(url_file)
            if u and u not in seen:
                link_sources.append(u)
                seen.add(u)

        # macOS .webloc shortcuts
        for url_file in sorted(pdf_dir.glob("*.webloc")):
            u = parse_macos_webloc(url_file)
            if u and u not in seen:
                link_sources.append(u)
                seen.add(u)

        # Generic additional .txt link lists
        for txt_file in sorted(pdf_dir.glob("*.txt")):
            if txt_file.name.lower() == "links.txt":
                continue  # already handled
            for u in parse_links_from_text_file(txt_file):
                if u not in seen:
                    link_sources.append(u)
                    seen.add(u)

    # Ingest links
    for url in link_sources:
        title, md = fetch_url(url)
        if not md.strip():
            continue
        title = title or url
        slug = slugify(title)
        out_file = unique_outpath(out_dir, slug)
        meta = [
            f"---",
            f"title: {title}",
            f"source: {url}",
            f"ingested: {now}",
            f"type: url",
            f"---",
            "",
        ]
        out_file.write_text("\n".join(meta) + md)
        index["docs"].append({
            "title": title,
            "source": url,
            "path": out_file.as_posix(),
            "type": "url",
            "ingested": now,
        })
        print(f"[ok] URL -> {out_file}")

    index_path.write_text(json.dumps(index, indent=2))
    print(f"[ok] Index updated: {index_path}")


if __name__ == "__main__":
    sys.exit(main())
