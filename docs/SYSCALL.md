# AION OS 3.4 — Syscall dispatch foundation

Implements a small, versioned syscall dispatcher with explicit security-context checks and deterministic negative error codes. Calls: GET_VERSION, GET_TASK_ID and YIELD (records a scheduling request, does not switch CPU context). The dispatcher rejects kernel-origin contexts, missing EXEC capability, invalid task IDs, null arguments and unknown calls.

Host tests cover success and failure paths, with freestanding x86-64/ARM64 compilation.

**Not a real user/kernel syscall gate yet.** The interface is an in-kernel C dispatcher. CPU trap instructions, user register ABI, validated user pointers, hardware-enforced isolation and actual task rescheduling remain future integration work. Do not execute untrusted userspace through this interface.
