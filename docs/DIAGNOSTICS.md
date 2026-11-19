# AION OS 1.5 — Early-boot diagnostics

AION OS provides a tiny, freestanding diagnostic interface that works before an allocator, scheduler, kernel, or filesystem exists.

- `aion_diag_write(text)`: bounded only by caller-supplied null termination; call with trusted static strings.
- `aion_diag_event(component,event)`: machine-readable `AION_DIAG:<component>:<event>` line.
- `aion_diag_hex(value)`: fixed-width 64-bit hexadecimal output.

Development transports: x86-64 QEMU debugcon I/O port 0xe9 and ARM64 QEMU virt PL011 UART0. Neither transport is suitable as a generic real-device driver.

The UEFI entrypoint emits both the legacy `AION_UEFI_ENTRY_OK` marker and structured diagnostic events so that 1.4 boot tests remain backward compatible. Tests check structured output for both architectures.
