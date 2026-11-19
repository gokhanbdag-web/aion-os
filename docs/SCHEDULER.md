# AION OS 3.2 — Round-robin scheduler decisions

The scheduler uses the 3.1 task registry to select READY tasks in circular slot order. It transitions the selected task to RUNNING, returns the previous running task to READY on the next dispatch, and supports block, wake, and exit operations. A monotonically increasing dispatch counter helps diagnostics.

Host tests cover fairness, idle result, blocking, waking, and exit behavior; freestanding compilation targets x86-64 and ARM64.

**Scope:** The scheduler chooses tasks but does not yet save/restore CPU registers, perform timer-driven preemption, allocate per-task kernel stacks, or run tasks on hardware. These require future context-switch and interrupt integration.
