#!/usr/bin/env python3
"""Cross-compile minimal PE/COFF UEFI entrypoints for x86-64 and ARM64."""
import argparse
import pathlib
import shutil
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]
TARGETS = {
    "x86_64": ("x86_64-pc-windows-msvc", "BOOTX64.EFI"),
    "aarch64": ("aarch64-pc-windows-msvc", "BOOTAA64.EFI"),
}

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--arch", choices=(*TARGETS, "all"), default="all")
    args = parser.parse_args()
    for tool in ("clang", "lld-link"):
        if not shutil.which(tool):
            parser.error(f"missing tool: {tool}")
    selected = TARGETS if args.arch == "all" else {args.arch: TARGETS[args.arch]}
    for arch, (target, name) in selected.items():
        output = ROOT / "build" / arch / "EFI" / "BOOT" / name
        output.parent.mkdir(parents=True, exist_ok=True)
        cmd = [
            "clang", f"--target={target}", "-ffreestanding",
            "-fshort-wchar", "-fno-stack-protector", "-fno-builtin",
            "-c", str(ROOT / "boot/uefi/main.c"),
            "-o", str(output.with_suffix(".obj")),
        ]
        subprocess.run(cmd, check=True)
        subprocess.run([
            "lld-link", "/nologo", "/subsystem:efi_application",
            "/entry:efi_main", "/nodefaultlib",
            f"/out:{output}", str(output.with_suffix(".obj")),
        ], check=True)
        if output.read_bytes()[:2] != b"MZ":
            raise SystemExit(f"Invalid PE image: {output}")
        print(f"PASS: {arch} UEFI PE/COFF image {output.relative_to(ROOT)}")

if __name__ == "__main__":
    main()
