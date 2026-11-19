#include "diagnostics.h"
/* AION OS 1.4: firmware entry emits an explicit guest boot marker.
 * x86-64: QEMU debugcon at I/O port 0xe9.
 * ARM64 QEMU virt: PL011 UART0 MMIO at 0x09000000.
 * This is a firmware-entry smoke test, not a kernel boot.
 */
#include "efi.h"
#define BOOT_MARKER "AION_UEFI_ENTRY_OK"
static void emit(char c) {
#if defined(__x86_64__)
    __asm__ volatile ("outb %0, %1" : : "a"(c), "Nd"((unsigned short)0xe9));
#elif defined(__aarch64__)
    volatile unsigned int *uart_dr = (volatile unsigned int *)0x09000000UL;
    volatile unsigned int *uart_fr = (volatile unsigned int *)0x09000018UL;
    unsigned int limit = 1000000;
    while ((*uart_fr & (1u << 5)) && --limit) { }
    if (limit) *uart_dr = (unsigned int)(unsigned char)c;
#endif
}
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    if (!image_handle || !system_table)
        return EFI_INVALID_PARAMETER;
    aion_diag_event("uefi", "entry");
    const char *p = BOOT_MARKER;
    while (*p) emit(*p++);
    emit('\n');
    aion_diag_event("uefi", "ready");
    return EFI_SUCCESS;
}
