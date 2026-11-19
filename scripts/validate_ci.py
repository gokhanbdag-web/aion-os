#!/usr/bin/env python3
"""Check integrated CI source/firmware/gate contract without external YAML packages."""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
workflow = (ROOT / ".github/workflows/ci.yml").read_text()
required = [
    "name: AION integrated CI",
    "  source:",
    "  firmware:",
    "  gate:",
    "    needs: [source, firmware]",
    "        arch: [x86_64, aarch64]",
    "scripts/validate_structure.py",
    "test_diagnostics.py",
    "scripts/build_uefi.py",
    "scripts/qemu_uefi_smoke.py",
    "test \"$SOURCE_RESULT\" = success",
    "test \"$FIRMWARE_RESULT\" = success",
]
missing = [item for item in required if item not in workflow]
if missing:
    raise SystemExit("CI contract missing: " + ", ".join(missing))
print("PASS: integrated CI source, dual-architecture firmware and merge gate")
