# AION OS 4.1 — Block device abstraction

Minimal freestanding synchronous block I/O interface with read/write callbacks, explicit block geometry, read-only support, range validation, and transfer-size overflow checks. Tests use an in-memory mock device and UBSan. Both x86-64 and ARM64 freestanding object compilation are checked.

No hardware disk driver, persistence, filesystem, asynchronous I/O, locking, or DMA is implemented in this milestone. Device drivers and higher-level storage layers will integrate later.
