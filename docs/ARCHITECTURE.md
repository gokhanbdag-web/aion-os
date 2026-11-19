# AION OS — Architecture v0.1

## Mission
Build an original AI-native operating system with one shared source architecture for x86-64 desktops and ARM64 mobile devices. No Linux, Windows, BSD or Android kernel dependency.

## Trust boundaries
- Kernel: deterministic privileged code; memory isolation, scheduling, IPC, interrupt handling, capability checks.
- Userspace services: drivers where feasible, storage, networking, graphics, audio, application runtime.
- Intelligence: unprivileged AI agents invoke audited, capability-scoped system APIs only.
- Evolution: isolated builds and tests; independently authorized, signed A/B updates with rollback.

## Boot targets
1. x86_64 UEFI + QEMU.
2. aarch64 QEMU virtual platform; board-specific boot adapters.
3. Selected real hardware only after virtual targets pass.

## Core modules
- `boot/`: platform boot entry and handoff.
- `arch/`: CPU-specific startup, interrupts and paging primitives.
- `kernel/`: scheduling, memory, IPC, process isolation and capability enforcement.
- `drivers/`: device discovery and driver interfaces.
- `filesystem/`: storage and filesystem services.
- `networking/`: network stack and transport services.
- `graphics/`, `audio/`: presentation and audio services.
- `platform/mobile/`, `platform/desktop/`: platform-specific UX and integration.
- `applications/`, `sdk/`: native application platform and developer APIs.
- `ai/`: AI integration interfaces; model training and inference live in separate aion-intelligence repository.
- `tests/`, `tools/`: test harnesses and developer tooling.

## Dependency direction
Architecture adapters -> kernel interfaces; userspace services -> system APIs; applications and AI -> capability-brokered APIs. Kernel MUST NOT depend on model inference, networking, or a userspace agent.

## Non-goals for milestone 1.1
No claims of a bootable kernel, hardware support, functional AI, or Windows application compatibility.

## Milestone acceptance
Documented module boundaries, both CPU targets, threat boundaries, and repeatable structure validation. Subsequent milestone 1.2 establishes build toolchains.
