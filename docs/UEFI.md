# AION OS 1.3 — UEFI entrypoint

AION OS now defines a minimal freestanding firmware entry for both x86-64 and ARM64. It receives an EFI image handle and opaque system table pointer, validates they are non-null, and returns an EFI status. It does not yet initialize a kernel, access the framebuffer, exit boot services, or claim successful boot in QEMU.

## Build
Install LLVM Clang and lld, then run:
```sh
python3 scripts/build_uefi.py --arch all
```

Outputs are `build/x86_64/EFI/BOOT/BOOTX64.EFI` and `build/aarch64/EFI/BOOT/BOOTAA64.EFI`. Build artifacts are not committed. Stage 1.4 will execute these in QEMU/UEFI firmware and validate the boot path.

The entrypoint has no Linux, BSD, or Android kernel dependency.
