# AION OS 3.5 — IPC mailbox foundation

Fixed-size bounded per-task mailboxes with FIFO messages, capacity limits, deterministic errors, capability checks and queue clearing on close. Tests cover queue overflow, empty queue, invalid input, permission denial, and mailbox reuse. Host tests use UBSan; freestanding compilation covers x86-64 and ARM64.

**Security and integration limits:** This is an in-kernel single-threaded data structure. The sender and recipient task IDs are caller-supplied and not authenticated; receiving is not yet bound to the current task. Do not expose it to untrusted userspace. Task identity must be derived from trusted scheduler state, mailbox creation/close must be authorized, and concurrent access must be synchronized before production use. Syscall entry and hardware-enforced isolation remain future work.
