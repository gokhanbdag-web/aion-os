# AION OS 1.2 — Cross-platform toolchain

This milestone prepares build tooling only; it does not claim a bootable operating system.

## Requirements
- Rust nightly pinned in `rust-toolchain.toml` for reproducible development.
- `rust-src` and `llvm-tools-preview` components.
- Rust bare-metal targets `x86_64-unknown-none` and `aarch64-unknown-none`.
- LLVM/Clang including `clang` and `ld.lld`.
- Python 3 and GNU Make for developer checks.

## Setup
```sh
rustup toolchain install nightly-2026-11-01 --profile minimal --component rust-src --component llvm-tools-preview
rustup target add x86_64-unknown-none aarch64-unknown-none --toolchain nightly-2026-11-01
make check-targets
```

No third-party OS kernel or pretrained AI model is included.
