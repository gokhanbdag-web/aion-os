# AION OS 5.2 — ACPI discovery

Introduces bounds-checked ACPI RSDP (v1 and v2+) and system-description-table (SDT) header parsing with checksum validation. Returns the physical RSDT/XSDT root address for a platform memory mapper to resolve.

This module does not directly map physical memory, walk XSDT entries, parse MADT/MCFG, or configure interrupts. Those capabilities require integration with platform firmware memory mapping and later hardware milestones.
