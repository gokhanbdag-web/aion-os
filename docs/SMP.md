# AION OS 2.5 — SMP foundation

The freestanding CPU registry tracks up to 64 logical CPUs, bootstrap processor identity, registration, and OFFLINE → STARTING → ONLINE state transitions. It prevents duplicate CPU IDs, invalid transitions, and offlining the bootstrap processor.

Host tests verify state transitions, counts, duplicate IDs, and capacity; freestanding x86-64 and ARM64 compile checks are included.

**Not yet hardware SMP:** No x86-64 INIT/SIPI startup, ARM64 PSCI CPU_ON, topology discovery, AP trampoline, per-CPU stacks, barriers, locks, or concurrent execution. The registry must be integrated with actual CPU startup and synchronization before multicore operation is claimed.
