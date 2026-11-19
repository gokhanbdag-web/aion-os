# AION OS 2.1 — Bootstrap CPU discovery

The architecture-neutral `aion_cpu_probe` API populates a fixed-size CPU information record with architecture, bootstrap CPU ID, and minimal ISA capabilities.

- x86-64: CPUID checks long mode and SSE2.
- ARM64: MPIDR_EL1 and ID_AA64PFR0_EL1 detect bootstrap CPU ID, FP and ASIMD.
- Firmware entry logs `AION_DIAG:cpu:ready` only after a successful probe.

This is **bootstrap CPU discovery**, not yet kernel privilege transitions, exception vector installation, interrupt controller initialization, per-CPU stacks or secondary CPU startup. Those require later kernel milestones. ARM64 EL1 system register access assumes compatible QEMU firmware execution context.
