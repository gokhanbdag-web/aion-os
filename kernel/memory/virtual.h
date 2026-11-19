#ifndef AION_VIRTUAL_MEMORY_H
#define AION_VIRTUAL_MEMORY_H
#include <stdint.h>
#include <stddef.h>
#define AION_VMM_PAGE_SIZE 4096ULL
#define AION_VMM_MAX_MAPPINGS 256
#define AION_VMM_READ (1u << 0)
#define AION_VMM_WRITE (1u << 1)
#define AION_VMM_EXEC (1u << 2)
struct aion_vmm_mapping {
    uint64_t virtual_address;
    uint64_t physical_address;
    uint32_t flags;
};
struct aion_vmm {
    struct aion_vmm_mapping entries[AION_VMM_MAX_MAPPINGS];
    size_t count;
};
void aion_vmm_init(struct aion_vmm *vmm);
int aion_vmm_map(struct aion_vmm *vmm, uint64_t virtual_address, uint64_t physical_address, uint32_t flags);
int aion_vmm_unmap(struct aion_vmm *vmm, uint64_t virtual_address);
int aion_vmm_translate(const struct aion_vmm *vmm, uint64_t virtual_address, uint64_t *physical_address, uint32_t *flags);
#endif
