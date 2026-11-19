/* Deterministic host stress of bootstrap memory and CPU bookkeeping. */
#include "../kernel/memory/heap.h"
#include "../kernel/memory/virtual.h"
#include "../kernel/arch/smp.h"
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

static _Alignas(16) unsigned char arena[65536];
static struct aion_heap heap;
static struct aion_vmm vmm;
static struct aion_smp smp;
static uint32_t rng=0xA10A2026u;
static uint32_t next_random(void) {
    rng^=rng<<13; rng^=rng>>17; rng^=rng<<5;
    return rng;
}
static void stress_heap(void) {
    void *slots[128]={0};
    assert(aion_heap_init(&heap,arena,sizeof arena)==0);
    for (unsigned i=0;i<30000;i++) {
        unsigned slot=next_random()%128;
        if (slots[slot]) {
            assert(aion_heap_free(&heap,slots[slot])==0);
            slots[slot]=0;
        } else {
            size_t bytes=1+(next_random()%512);
            void *p=aion_heap_alloc(&heap,bytes);
            if (p) {
                assert(((uintptr_t)p & 15u)==0);
                slots[slot]=p;
            }
        }
        assert(aion_heap_free_bytes(&heap)<=sizeof arena);
    }
    for (unsigned i=0;i<128;i++) if (slots[i]) assert(aion_heap_free(&heap,slots[i])==0);
    assert(aion_heap_free_bytes(&heap)==sizeof arena);
    assert(heap.count==1);
}
static void stress_vmm(void) {
    unsigned char mapped[128]={0};
    aion_vmm_init(&vmm);
    for (unsigned i=0;i<30000;i++) {
        unsigned slot=next_random()%128;
        uint64_t va=0x100000ull+4096ull*slot;
        uint64_t pa=0x200000ull+4096ull*slot;
        uint64_t actual=0; uint32_t flags=0;
        if (mapped[slot]) {
            assert(aion_vmm_translate(&vmm,va+123,&actual,&flags)==0);
            assert(actual==pa+123 && flags==AION_VMM_READ);
            assert(aion_vmm_unmap(&vmm,va)==0);
            mapped[slot]=0;
        } else {
            assert(aion_vmm_map(&vmm,va,pa,AION_VMM_READ)==0);
            mapped[slot]=1;
        }
        assert(vmm.count<=128);
    }
    for (unsigned slot=0;slot<128;slot++) if (mapped[slot])
        assert(aion_vmm_unmap(&vmm,0x100000ull+4096ull*slot)==0);
    assert(vmm.count==0);
}
static void stress_smp(void) {
    assert(aion_smp_init(&smp,0)==0);
    for (uint64_t id=1;id<64;id++) assert(aion_smp_register(&smp,id)==0);
    for (unsigned i=0;i<30000;i++) {
        uint64_t id=1+(next_random()%63);
        struct aion_smp_cpu *cpu=&smp.cpus[id];
        if (cpu->state==AION_SMP_OFFLINE) {
            assert(aion_smp_mark_starting(&smp,id)==0);
        } else if (cpu->state==AION_SMP_STARTING) {
            assert(aion_smp_mark_online(&smp,id)==0);
        } else {
            assert(aion_smp_mark_offline(&smp,id)==0);
        }
        size_t counted=0;
        for (unsigned j=0;j<smp.count;j++) if (smp.cpus[j].state==AION_SMP_ONLINE) counted++;
        assert(smp.online_count==counted);
        assert(smp.cpus[0].state==AION_SMP_ONLINE);
    }
}
int main(void) {
    stress_heap();
    stress_vmm();
    stress_smp();
    puts("AION_STRESS_OK: heap=30000 vmm=30000 smp=30000");
    return 0;
}
