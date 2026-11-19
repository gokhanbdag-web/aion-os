#!/usr/bin/env python3
"""Validate AION OS architecture skeleton without third-party dependencies."""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REQUIRED = [
    "boot/x86_64", "boot/aarch64", "arch/x86_64", "arch/aarch64",
    "kernel/memory", "kernel/scheduler", "kernel/process", "kernel/ipc",
    "kernel/security", "drivers", "filesystem", "networking", "graphics",
    "audio", "ai/intelligence", "ai/voice", "ai/agents", "ai/evolution",
    "platform/mobile", "platform/desktop", "applications", "sdk",
    "tests", "tools",
]
def main():
    missing = [path for path in REQUIRED if not (ROOT / path / ".gitkeep").is_file()]
    for path in ("docs/ARCHITECTURE.md", "docs/ROADMAP.md"):
        if not (ROOT / path).is_file():
            missing.append(path)
    if missing:
        raise SystemExit("Missing architecture paths: " + ", ".join(missing))
    print(f"PASS: {len(REQUIRED)} module directories and architecture documents")
if __name__ == "__main__":
    main()
