# AION OS 1.4 — QEMU firmware boot testing

This stage adds reproducible QEMU launch and a strict timeout-based smoke harness for x86-64 OVMF and ARM64 AAVMF. A timeout is **inconclusive/failure**, never counted as proof of boot.

Prerequisites: `qemu-system-x86`, `qemu-system-arm`, `ovmf`, `clang`, `lld` and AAVMF firmware.

```sh
python3 scripts/build_uefi.py --arch all
python3 scripts/qemu_uefi_smoke.py --arch x86_64
python3 scripts/qemu_uefi_smoke.py --arch aarch64
```

The 1.4 EFI entrypoint emits `AION_UEFI_ENTRY_OK` via x86-64 QEMU debugcon or ARM64 PL011 UART. The harness requires that marker even if firmware remains running until timeout. This verifies firmware executed our EFI entrypoint, **not** kernel startup, graphics, or an installed OS.
