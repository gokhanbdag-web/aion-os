# AION OS 2.2 — Physical memory

AION PMM is an allocation-free fixed-capacity region manager. It ingests a non-overlapping, overflow-checked memory map, aligns free memory to 4 KiB frames, reserves pages, and allocates physical frames. Reservation and allocation never return reserved frames.

Tests exercise alignment, overlapping maps, overflow, double reservations, allocation exhaustion, and freestanding cross-compilation.

This milestone provides the **physical frame manager**, not firmware memory-map ingestion, page-table mapping, a virtual allocator, or DMA. Firmware ownership and kernel handoff will need explicit integration before using it on real hardware.
