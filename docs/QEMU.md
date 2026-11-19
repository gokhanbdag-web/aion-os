# AION OS 1.4 — QEMU firmware boot testing

This stage adds reproducible QEMU launch and a strict timeout-based smoke harness for x86-64 OVMF and ARM64 AAVMF. A timeout is **inconclusive/failure**, never counted as proof of boot.

Prerequisites: `qemu-system-x86`, `qemu-system-arm`, `ovmf`, `clang`, `lld` and AAVMF firmware.

```sh
python3 scripts/build_uefi.py --arch all
python3 scripts/qemu_uefi_smoke.py --arch x86_64
python3 scripts/qemu_uefi_smoke.py --arch aarch64
```

The current 1.3 EFI entrypoint returns to firmware. It has no serial or other guest-side success handshake; therefore an automated boot **pass cannot be claimed yet**. A follow-up in this milestone must add an explicit guest success signal before enabling passing CI boot tests.
