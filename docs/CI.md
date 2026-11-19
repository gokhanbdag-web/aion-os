# AION OS 1.6 — Integrated CI

The `AION integrated CI` workflow provides one branch-protection target: the `gate` job.

1. **source**: repository layout, diagnostics unit tests, Python syntax, and CI contract.
2. **firmware**: independent x86-64 and ARM64 matrix jobs build UEFI images and verify guest execution in QEMU.
3. **gate**: fails if source or any firmware matrix job does not succeed, including cancellation or skip.

Existing focused workflows remain in place as compatibility checks. This workflow does not claim full operating-system boot, only the tested UEFI entrypoint.

Repository administrators must configure GitHub branch protection to require `AION integrated CI / gate` (and optionally other existing checks). A workflow alone cannot enforce branch protection.
