# AION OS 2.4 — Kernel heap allocator

The freestanding allocator uses a caller-provided, 16-byte aligned memory arena and fixed-size metadata. It implements first-fit allocation, 16-byte alignment, free, adjacent-block coalescing and double-free detection. No host allocator or libc allocation dependency is required.

Host tests exercise reuse, alignment, exhaustion, coalescing, invalid pointers, zero sizes and overflow; both CPU targets are cross-compiled.

**Limitations:** No locking, guard pages, arena expansion, physical frame provisioning, or hardware memory mapping yet. This is a single-threaded bootstrap heap suitable for future kernel integration, not a production hardened allocator.
