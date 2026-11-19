#include "../kernel/memory/virtual.h"
#include <assert.h>
#include <stdint.h>
int main(void) {
    struct aion_vmm vmm;
    aion_vmm_init(&vmm);
    uint64_t pa=0;
    uint32_t flags=0;
    assert(aion_vmm_map(&vmm,0x4000,0x9000,AION_VMM_READ|AION_VMM_WRITE)==0);
    assert(aion_vmm_translate(&vmm,0x4123,&pa,&flags)==0);
    assert(pa==0x9123 && flags==(AION_VMM_READ|AION_VMM_WRITE));
    assert(aion_vmm_map(&vmm,0x4000,0xa000,AION_VMM_READ)==-2);
    assert(aion_vmm_map(&vmm,0x4100,0xa000,AION_VMM_READ)==-1);
    assert(aion_vmm_map(&vmm,0x5000,0xa100,AION_VMM_READ)==-1);
    assert(aion_vmm_map(&vmm,0x5000,0xa000,AION_VMM_WRITE|AION_VMM_EXEC)==-1);
    assert(aion_vmm_map(&vmm,0x5000,0xa000,0)==-1);
    assert(aion_vmm_unmap(&vmm,0x4000)==0);
    assert(aion_vmm_translate(&vmm,0x4000,&pa,&flags)==-2);
    assert(aion_vmm_unmap(&vmm,0x4000)==-2);
    for (uint64_t i=0;i<AION_VMM_MAX_MAPPINGS;++i)
        assert(aion_vmm_map(&vmm,0x100000+i*4096,0x200000+i*4096,AION_VMM_READ)==0);
    assert(aion_vmm_map(&vmm,0x400000,0x500000,AION_VMM_READ)==-1);
    return 0;
}
