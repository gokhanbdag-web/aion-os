# AION OS 4.2 — Disk partition discovery

Introduces a bounded, read-only legacy MBR parser on top of the 4.1 block-device abstraction. Validates MBR signature, boot flags, nonempty entries, device bounds, and partition overlap. Supports up to four primary partitions and rejects unsupported extended partitions and protective GPT rather than silently misparsing them.

No GPT parser, extended partition chain, filesystem mount, disk modification, or physical storage driver is included. Input is untrusted and must be validated; this is a foundational discovery interface, not a complete partition manager.
