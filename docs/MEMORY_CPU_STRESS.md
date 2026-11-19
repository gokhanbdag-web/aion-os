# AION OS 2.6 — Memory and CPU stress testing

The host stress harness uses a fixed-seed xorshift32 PRNG and runs 90,000 total deterministic operations across the 2.4 heap allocator, 2.3 virtual-memory mapping ledger, and 2.5 SMP CPU registry.

- Heap: randomized allocate/free with alignment and free-space invariants; all allocations are freed and coalescence checked at the end.
- VMM: randomized map/translate/unmap with offset translation and capacity invariants.
- SMP: repeated legal lifecycle transitions with online-count consistency and bootstrap CPU invariant.

GitHub Actions builds with AddressSanitizer and UndefinedBehaviorSanitizer. These are single-threaded **host model** stress tests. They do not prove actual multicore execution, hardware paging, physical frame behavior or thread safety; those require separate kernel and hardware tests.
