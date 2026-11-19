#!/usr/bin/env python3
"""Run a bounded firmware boot smoke test; never mistake a timeout for success."""
import argparse
import pathlib
import shutil
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--arch", choices=("x86_64", "aarch64"), required=True)
    parser.add_argument("--timeout", type=int, default=20)
    args = parser.parse_args()
    arch = args.arch
    exe = "qemu-system-x86_64" if arch == "x86_64" else "qemu-system-aarch64"
    if not shutil.which(exe):
        parser.error(f"Missing {exe}")
    efi = ROOT / "build" / arch / "EFI" / "BOOT" / ("BOOTX64.EFI" if arch == "x86_64" else "BOOTAA64.EFI")
    if not efi.is_file():
        parser.error(f"Missing {efi}; run scripts/build_uefi.py first")
    with tempfile.TemporaryDirectory() as tmp:
        esp = pathlib.Path(tmp) / "EFI" / "BOOT"
        esp.mkdir(parents=True)
        shutil.copy2(efi, esp / efi.name)
        if arch == "x86_64":
            firmware = next((path for path in (
                "/usr/share/OVMF/OVMF_CODE_4M.fd",
                "/usr/share/OVMF/OVMF_CODE.fd",
            ) if pathlib.Path(path).exists()), None)
            if not firmware:
                parser.error("Install ovmf firmware")
            cmd = [exe, "-machine", "q35", "-m", "256M", "-nographic",
                   "-monitor", "none", "-serial", "stdio",
                   "-bios", firmware, "-drive", f"format=raw,file=fat:rw:{tmp}"]
        else:
            firmware = "/usr/share/AAVMF/AAVMF_CODE.fd"
            if not pathlib.Path(firmware).exists():
                parser.error("Install AAVMF firmware")
            cmd = [exe, "-machine", "virt", "-cpu", "cortex-a57", "-m", "512M",
                   "-nographic", "-monitor", "none", "-serial", "stdio",
                   "-bios", firmware, "-drive", f"if=none,id=esp,format=raw,file=fat:rw:{tmp}",
                   "-device", "virtio-blk-device,drive=esp"]
        try:
            result = subprocess.run(cmd, capture_output=True, text=True,
                                    timeout=args.timeout, check=False)
        except subprocess.TimeoutExpired as exc:
            # A firmware run that hangs without an explicit success signal is NOT a pass.
            print(f"INCONCLUSIVE: {arch} QEMU timed out without guest success signal")
            raise SystemExit(1) from exc
        print((result.stdout + result.stderr)[-3000:])
        if result.returncode:
            raise SystemExit(f"FAIL: QEMU exited {result.returncode}")
        print(f"PASS: {arch} QEMU process exited successfully")

if __name__ == "__main__":
    main()
