#include "../kernel/memory/physical.h"
#include <assert.h>
#include <stdint.h>
int main(void) {
    struct aion_pmm p;
    struct aion_memory_region map[] = {
        {0x1003, 0x7ffd, AION_PMM_FREE},
        {0x9000, 0x1000, AION_PMM_RESERVED}
    };
    assert(aion_pmm_init(&p,map,2)==0);
    assert(p.free_pages==7);
    assert(aion_pmm_reserve(&p,0x3000,0x2000)==0);
    assert(p.free_pages==5);
    assert(aion_pmm_reserve(&p,0x3000,0x1000)!=0);
    uint64_t addr=0;
    for (int i=0;i<5;++i) {
        assert(aion_pmm_alloc_page(&p,&addr)==0);
        assert(addr%4096==0 && addr!=0x3000 && addr!=0x4000);
    }
    assert(p.free_pages==0);
    assert(aion_pmm_alloc_page(&p,&addr)!=0);
    struct aion_memory_region overlapping[]={{0x1000,0x2000,AION_PMM_FREE},{0x2000,0x2000,AION_PMM_FREE}};
    assert(aion_pmm_init(&p,overlapping,2)!=0);
    struct aion_memory_region overflow[]={{UINT64_MAX-1,16,AION_PMM_FREE}};
    assert(aion_pmm_init(&p,overflow,1)!=0);
    return 0;
}
