# AION OS 3.3 — Privilege boundary policy foundation

This milestone introduces explicit kernel/user privilege labels, capability checks, and one-way capability dropping. User contexts cannot receive administrative or raw device capabilities through this initializer, and kernel-only resources reject user contexts.

Tests cover allowed/denied access, invalid privilege levels, invalid capabilities, kernel-only resources, and dropping permissions. The module compiles freestanding for x86-64 and ARM64.

**Security limitation:** This is a software authorization policy, not yet hardware-enforced user/kernel isolation. Real protection requires x86 ring 3 / ARM64 EL0 transitions, separate user address spaces, MMU page permissions, interrupt/syscall entry validation and CPU context switching. Do not expose untrusted user code until those mechanisms are integrated and tested.
