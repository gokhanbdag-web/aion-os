#ifndef AION_SMP_H
#define AION_SMP_H
#include <stddef.h>
#include <stdint.h>
#define AION_SMP_MAX_CPUS 64
#define AION_SMP_OFFLINE 0u
#define AION_SMP_STARTING 1u
#define AION_SMP_ONLINE 2u
struct aion_smp_cpu {
    uint64_t hardware_id;
    uint32_t state;
};
struct aion_smp {
    struct aion_smp_cpu cpus[AION_SMP_MAX_CPUS];
    size_t count;
    size_t online_count;
};
int aion_smp_init(struct aion_smp *smp, uint64_t bootstrap_hardware_id);
int aion_smp_register(struct aion_smp *smp, uint64_t hardware_id);
int aion_smp_mark_starting(struct aion_smp *smp, uint64_t hardware_id);
int aion_smp_mark_online(struct aion_smp *smp, uint64_t hardware_id);
int aion_smp_mark_offline(struct aion_smp *smp, uint64_t hardware_id);
#endif
