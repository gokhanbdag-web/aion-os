# AION OS 3.6 — Isolation policy regression tests

Tests cover kernel-only resource denial, user privilege limitations, capability dropping, syscall authorization, IPC read/write checks, bounded queues and mailbox reset on reuse. Runs with UBSan on the host, plus freestanding x86-64/ARM64 compilation.

## Threat model and remaining work

**These tests validate software policy decisions only, not actual process isolation.** Current scheduler does not context-switch user tasks; there is no CPU privilege transition, memory page enforcement for untrusted tasks, trusted task identity binding to IPC, synchronization for concurrent mailbox use, or hardware syscall entry. IPC callers can supply arbitrary sender and recipient IDs. Therefore untrusted processes must not be launched or granted access to these APIs. True hardware isolation requires separate implementation and end-to-end QEMU/hardware negative tests before a security claim can be made.
