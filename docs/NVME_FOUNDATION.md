# AION OS 5.4 — NVMe foundation

Parses NVMe Identify Controller and Identify Namespace 4096-byte responses. Reports controller identifiers, namespace count, namespace block count and active LBA format size. Unsupported namespace metadata formats and unusual LBA sizes are rejected.

This is not yet an operational NVMe block driver: PCI BAR mapping, controller reset, submission/completion queues, DMA, interrupt handling and namespace I/O are not implemented here. Callers must obtain valid Identify buffers from a future transport implementation.
