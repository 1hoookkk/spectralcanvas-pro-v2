#!/usr/bin/env python3
"""
Scaffold and index project knowledge cards.

- Creates numbered cards under sessions/knowledge/pdf/cards (default).
- Auto-increments the NN- prefix.
- Rewrites INDEX.md to include all cards in order.
- Optionally rewrites README.md with the updated list.

Usage examples:
  python tools/card_scaffold.py "CCRMA Online Books Index" \
    "Phase Vocoder: Accuracy & Phase Handling"

  python tools/card_scaffold.py --out-dir sessions/knowledge/pdf/cards \
    --update-readme "FFT Performance & Planning"
"""
from __future__ import annotations

import argparse
import re
from pathlib import Path


def slugify(title: str) -> str:
    s = title.strip()
    s = re.sub(r"[\u2014\u2013]", "-", s)  # em/en dash
    s = re.sub(r"[^A-Za-z0-9\-_. ]+", "", s)
    s = re.sub(r"\s+", "-", s)
    return s


def next_number(out_dir: Path) -> int:
    mx = 0
    for p in out_dir.glob("*.md"):
        if p.name.lower() in {"readme.md", "index.md"}:
            continue
        m = re.match(r"^(\d{2})-", p.name)
        if m:
            mx = max(mx, int(m.group(1)))
    return mx + 1


def enumerate_cards(out_dir: Path) -> list[tuple[int, str, Path]]:
    items: list[tuple[int, str, Path]] = []
    for p in out_dir.glob("*.md"):
        if p.name.lower() in {"readme.md", "index.md"}:
            continue
        m = re.match(r"^(\d{2})-([^.]+)\.md$", p.name)
        if not m:
            continue
        n = int(m.group(1))
        title_slug = m.group(2)
        title = title_slug.replace("-", " ")
        items.append((n, title, p))
    items.sort(key=lambda t: t[0])
    return items


def write_index(out_dir: Path) -> None:
    items = enumerate_cards(out_dir)
    lines = [
        "# Knowledge Cards Index",
        "",
    ]
    for n, title, path in items:
        lines.append(f"{n}. [{n:02d} — {title}](" + path.name + ")")
    (out_dir / "INDEX.md").write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_readme(out_dir: Path) -> None:
    items = enumerate_cards(out_dir)
    lines = [
        "# SpectralCanvas Pro — High-Priority Knowledge Cards",
        "",
        "These concise cards summarize the most important practices and commands for daily work.",
        "",
        "Export to PDF: open each `.md` in your editor and \"Print to PDF\" (or use `pandoc` if available).",
        "",
    ]
    for n, title, path in items:
        lines.append(f"- {n:02d} — {title} (`{path.name}`)")
    (out_dir / "README.md").write_text("\n".join(lines) + "\n", encoding="utf-8")


def create_card(out_dir: Path, number: int, title: str) -> Path:
    slug = slugify(title)
    fname = f"{number:02d}-" + slug + ".md"
    path = out_dir / fname
    if path.exists():
        raise SystemExit(f"Refusing to overwrite existing card: {path}")
    content = [
        f"# {number:02d} — {title}",
        "",
        "Context",
        "- Problem or goal this card addresses.",
        "",
        "Guidelines",
        "- Actionable steps and do/don't items.",
        "",
        "Validation",
        "- How to test or verify success.",
        "",
    ]
    path.write_text("\n".join(content) + "\n", encoding="utf-8")
    return path


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("titles", nargs="*", help="Card titles to create")
    ap.add_argument("--out-dir", default="sessions/knowledge/pdf/cards")
    ap.add_argument("--no-index", action="store_true", help="Do not rewrite INDEX.md")
    ap.add_argument("--update-readme", action="store_true", help="Rewrite README.md with full list")
    args = ap.parse_args()

    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    n = next_number(out_dir)
    created = []
    for title in args.titles:
        path = create_card(out_dir, n, title)
        created.append(path)
        n += 1

    if not args.no_index:
        write_index(out_dir)
    if args.update_readme:
        write_readme(out_dir)

    if created:
        print("[ok] Created:")
        for p in created:
            print("  -", p)
    else:
        print("[note] No titles provided; nothing created. Use --help for usage.")


if __name__ == "__main__":
    main()

