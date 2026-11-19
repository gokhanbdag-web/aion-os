#ifndef AION_CPU_H
#define AION_CPU_H
#include <stdint.h>
enum aion_cpu_arch { AION_CPU_X86_64 = 1, AION_CPU_AARCH64 = 2 };
struct aion_cpu_info {
    uint32_t arch;
    uint32_t logical_id;
    uint64_t feature_bits;
};
#define AION_CPU_FEATURE_LONG_MODE (1ULL << 0)
#define AION_CPU_FEATURE_SSE2 (1ULL << 1)
#define AION_CPU_FEATURE_FP (1ULL << 2)
#define AION_CPU_FEATURE_ASIMD (1ULL << 3)
int aion_cpu_probe(struct aion_cpu_info *info);
#endif
