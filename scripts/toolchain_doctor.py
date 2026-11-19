#!/usr/bin/env python3
"""Validate host tooling for the original x86-64 and ARM64 bare-metal targets."""
import shutil
import subprocess
import sys

REQUIRED = ("rustup", "rustc", "cargo", "clang", "ld.lld")
TARGETS = ("x86_64-unknown-none", "aarch64-unknown-none")

def main():
    missing = [tool for tool in REQUIRED if shutil.which(tool) is None]
    if missing:
        print("Missing tools: " + ", ".join(missing), file=sys.stderr)
        return 1
    try:
        installed = subprocess.check_output(
            ["rustup", "target", "list", "--installed"], text=True, timeout=20
        ).splitlines()
    except (subprocess.CalledProcessError, OSError, subprocess.TimeoutExpired) as exc:
        print(f"Cannot inspect Rust targets: {exc}", file=sys.stderr)
        return 1
    absent = [target for target in TARGETS if target not in installed]
    if absent:
        print("Missing Rust targets: " + ", ".join(absent), file=sys.stderr)
        return 1
    print("PASS: Rust, Cargo, LLVM linker and x86-64/ARM64 bare-metal targets")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
