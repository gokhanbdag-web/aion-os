/* Freestanding, allocation-free diagnostic output for UEFI smoke stages.
 * x86_64 QEMU debugcon port 0xe9; ARM64 QEMU virt PL011 UART0.
 * Both are development-machine transports, not general hardware drivers.
 */
#include "diagnostics.h"
static void aion_diag_char(char c) {
#if defined(__x86_64__)
    __asm__ volatile("outb %0, %1" : : "a"(c), "Nd"((unsigned short)0xe9));
#elif defined(__aarch64__)
    volatile uint32_t *const dr = (volatile uint32_t *)0x09000000UL;
    volatile uint32_t *const fr = (volatile uint32_t *)0x09000018UL;
    uint32_t attempts = 1000000;
    while ((*fr & (1u << 5)) && --attempts) { }
    if (attempts) *dr = (uint32_t)(unsigned char)c;
#else
#error Unsupported architecture
#endif
}
void aion_diag_write(const char *message) {
    if (!message) return;
    while (*message) aion_diag_char(*message++);
}
void aion_diag_event(const char *component, const char *event) {
    aion_diag_write("AION_DIAG:");
    aion_diag_write(component);
    aion_diag_write(":");
    aion_diag_write(event);
    aion_diag_write("\n");
}
void aion_diag_hex(uint64_t value) {
    static const char digits[] = "0123456789abcdef";
    aion_diag_write("0x");
    for (int i = 15; i >= 0; --i)
        aion_diag_char(digits[(value >> (i * 4)) & 15]);
}
