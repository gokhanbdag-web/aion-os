/* SMP CPU registry and lifecycle; AP bring-up is not yet implemented. */
#include "smp.h"
static struct aion_smp_cpu *find_cpu(struct aion_smp *smp, uint64_t id) {
    if (!smp) return 0;
    for (size_t i=0;i<smp->count;++i)
        if (smp->cpus[i].hardware_id==id) return &smp->cpus[i];
    return 0;
}
int aion_smp_init(struct aion_smp *smp, uint64_t bsp) {
    if (!smp) return -1;
    smp->count=1;
    smp->online_count=1;
    smp->cpus[0]=(struct aion_smp_cpu){bsp,AION_SMP_ONLINE};
    return 0;
}
int aion_smp_register(struct aion_smp *smp, uint64_t id) {
    if (!smp || smp->count==0 || smp->count>=AION_SMP_MAX_CPUS || find_cpu(smp,id)) return -1;
    smp->cpus[smp->count++]=(struct aion_smp_cpu){id,AION_SMP_OFFLINE};
    return 0;
}
int aion_smp_mark_starting(struct aion_smp *smp, uint64_t id) {
    struct aion_smp_cpu *cpu=find_cpu(smp,id);
    if (!cpu || cpu->state!=AION_SMP_OFFLINE) return -1;
    cpu->state=AION_SMP_STARTING;
    return 0;
}
int aion_smp_mark_online(struct aion_smp *smp, uint64_t id) {
    struct aion_smp_cpu *cpu=find_cpu(smp,id);
    if (!cpu || cpu->state!=AION_SMP_STARTING) return -1;
    cpu->state=AION_SMP_ONLINE;
    smp->online_count++;
    return 0;
}
int aion_smp_mark_offline(struct aion_smp *smp, uint64_t id) {
    struct aion_smp_cpu *cpu=find_cpu(smp,id);
    if (!cpu || cpu==&smp->cpus[0] || cpu->state==AION_SMP_OFFLINE) return -1;
    if (cpu->state==AION_SMP_ONLINE) smp->online_count--;
    cpu->state=AION_SMP_OFFLINE;
    return 0;
}
