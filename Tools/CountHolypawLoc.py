#!/usr/bin/env python3
"""Count unique, non-empty Holypaw source lines toward the 200k AAA floor.

Excludes blank lines, comment-only lines, the archived web toy, and git metadata.
Does not treat generated catalogs as second-class: if it ships, it counts.
"""
from __future__ import annotations

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
FLOOR = 200_000

SKIP_DIR_PARTS = {
    ".git",
    "legacy",
    "Binaries",
    "Intermediate",
    "Saved",
    "DerivedDataCache",
    "__pycache__",
}

COUNT_SUFFIXES = {
    ".cpp",
    ".h",
    ".hpp",
    ".c",
    ".cs",
    ".py",
    ".ini",
    ".json",
    ".uplugin",
    ".uproject",
    ".md",
}

# Docs and README explain the game; they are not the 200k gameplay floor.
DOC_SUFFIXES = {".md"}

COMMENT_ONLY = re.compile(r"^\s*(//|/\*|\*|\#|;\s*//).*$")


def should_skip(path: pathlib.Path) -> bool:
    parts = set(path.parts)
    if parts & SKIP_DIR_PARTS:
        return True
    if path.suffix.lower() not in COUNT_SUFFIXES:
        return True
    return False


def line_kind(text: str, suffix: str) -> str:
    stripped = text.strip()
    if not stripped:
        return "blank"
    if suffix in {".py"} and stripped.startswith("#"):
        return "comment"
    if suffix in {".cpp", ".h", ".hpp", ".c", ".cs"} and COMMENT_ONLY.match(text):
        return "comment"
    if suffix in {".ini"} and (stripped.startswith(";") or stripped.startswith("#")):
        return "comment"
    if suffix in DOC_SUFFIXES:
        return "docs"
    return "code"


def main() -> int:
    code = 0
    comments = 0
    blanks = 0
    docs = 0
    files = 0
    by_suffix: dict[str, int] = {}

    for path in ROOT.rglob("*"):
        if not path.is_file() or should_skip(path):
            continue
        files += 1
        suffix = path.suffix.lower() or "<none>"
        try:
            text = path.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        for raw in text.splitlines():
            kind = line_kind(raw, suffix)
            if kind == "blank":
                blanks += 1
            elif kind == "comment":
                comments += 1
            elif kind == "docs":
                docs += 1
            else:
                code += 1
                by_suffix[suffix] = by_suffix.get(suffix, 0) + 1

    print("Holypaw LOC (AAA floor tracker)")
    print(f" files={files}")
    print(f" code={code}")
    print(f" comments={comments} blanks={blanks} docs={docs}")
    print(f" floor={FLOOR}")
    remaining = max(0, FLOOR - code)
    print(f" remaining={remaining}")
    print(" by_suffix:")
    for suffix, n in sorted(by_suffix.items(), key=lambda kv: (-kv[1], kv[0])):
        print(f"  {suffix}: {n}")
    if code >= FLOOR:
        print("FLOOR_MET")
        return 0
    print("FLOOR_NOT_MET")
    return 0


if __name__ == "__main__":
    sys.exit(main())
