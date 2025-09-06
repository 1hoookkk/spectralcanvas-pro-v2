# 01 — Knowledge Ingest Workflow

Purpose
- Convert local PDFs and curated links into lightweight Markdown notes for fast search.

Commands
- Ingest: `python tools/knowledge_ingest.py --pdf-dir sessions/knowledge/pdf --links-file sessions/knowledge/links.txt --out-dir sessions/knowledge/md --index sessions/knowledge/index.json`
- Result: Markdown notes in `sessions/knowledge/md`, index at `sessions/knowledge/index.json`.

Notes
- Network may be restricted; URL fetches will be skipped gracefully.
- PDF text extraction is best-effort (pypdf); validate important passages.
- Re-run after adding PDFs or updating `links.txt`.

