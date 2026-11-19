/* Read-only bootstrap CPU discovery. No privileged control-register writes. */
#include "cpu.h"
int aion_cpu_probe(struct aion_cpu_info *info) {
    if (!info) return -1;
    info->arch = 0;
    info->logical_id = 0;
    info->feature_bits = 0;
#if defined(__x86_64__)
    unsigned int eax, ebx, ecx, edx;
    info->arch = AION_CPU_X86_64;
    eax = 1; ecx = 0;
    __asm__ volatile("cpuid" : "+a"(eax), "=b"(ebx), "+c"(ecx), "=d"(edx));
    if (edx & (1u << 26)) info->feature_bits |= AION_CPU_FEATURE_SSE2;
    eax = 0x80000000u; ecx = 0;
    __asm__ volatile("cpuid" : "+a"(eax), "=b"(ebx), "+c"(ecx), "=d"(edx));
    if (eax >= 0x80000001u) {
        eax = 0x80000001u; ecx = 0;
        __asm__ volatile("cpuid" : "+a"(eax), "=b"(ebx), "+c"(ecx), "=d"(edx));
        if (edx & (1u << 29)) info->feature_bits |= AION_CPU_FEATURE_LONG_MODE;
    }
    return (info->feature_bits & (AION_CPU_FEATURE_LONG_MODE | AION_CPU_FEATURE_SSE2))
        == (AION_CPU_FEATURE_LONG_MODE | AION_CPU_FEATURE_SSE2) ? 0 : -2;
#elif defined(__aarch64__)
    uint64_t mpidr, pfr0;
    __asm__ volatile("mrs %0, mpidr_el1" : "=r"(mpidr));
    __asm__ volatile("mrs %0, id_aa64pfr0_el1" : "=r"(pfr0));
    info->arch = AION_CPU_AARCH64;
    info->logical_id = (uint32_t)(mpidr & 0xffu);
    if (((pfr0 >> 16) & 15u) != 15u) info->feature_bits |= AION_CPU_FEATURE_FP;
    if (((pfr0 >> 20) & 15u) != 15u) info->feature_bits |= AION_CPU_FEATURE_ASIMD;
    return 0;
#else
#error Unsupported AION CPU architecture
#endif
}
