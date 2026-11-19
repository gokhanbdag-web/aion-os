# AION OS 2.3 — Virtual memory mapping model

The VMM implements a fixed-capacity, architecture-neutral ledger of 4 KiB virtual-to-physical page mappings, permission flags, duplicate prevention, unmap and translation. It rejects writable/executable mappings as a conservative W^X default. Unit tests exercise offsets, invalid alignment, duplicate mappings, exhaustion and unmapping. Both x86-64 and ARM64 freestanding compilation is checked in CI.

**Scope limitation:** This is a software mapping model, not hardware-backed paging. It does not yet populate x86-64 page tables or ARM64 translation tables, install CR3/TTBR registers, perform TLB invalidation, establish ASIDs/PCIDs or enforce permissions in hardware. Those integration steps must be implemented and tested before treating these mappings as memory isolation.
