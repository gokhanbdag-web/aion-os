/* Architecture-neutral page mapping ledger; hardware page tables come later. */
#include "virtual.h"
#include <stdint.h>
static int valid_page(uint64_t address) {
    return (address & (AION_VMM_PAGE_SIZE - 1)) == 0;
}
void aion_vmm_init(struct aion_vmm *vmm) {
    if (vmm) vmm->count = 0;
}
int aion_vmm_map(struct aion_vmm *vmm, uint64_t va, uint64_t pa, uint32_t flags) {
    if (!vmm || !valid_page(va) || !valid_page(pa) ||
        !flags || (flags & ~(AION_VMM_READ | AION_VMM_WRITE | AION_VMM_EXEC)) ||
        ((flags & AION_VMM_WRITE) && (flags & AION_VMM_EXEC)) ||
        vmm->count >= AION_VMM_MAX_MAPPINGS) return -1;
    for (size_t i = 0; i < vmm->count; ++i)
        if (vmm->entries[i].virtual_address == va) return -2;
    vmm->entries[vmm->count++] = (struct aion_vmm_mapping){va, pa, flags};
    return 0;
}
int aion_vmm_unmap(struct aion_vmm *vmm, uint64_t va) {
    if (!vmm || !valid_page(va)) return -1;
    for (size_t i = 0; i < vmm->count; ++i) {
        if (vmm->entries[i].virtual_address == va) {
            vmm->entries[i] = vmm->entries[--vmm->count];
            return 0;
        }
    }
    return -2;
}
int aion_vmm_translate(const struct aion_vmm *vmm, uint64_t va, uint64_t *pa, uint32_t *flags) {
    if (!vmm || !pa || !flags) return -1;
    uint64_t base = va & ~(AION_VMM_PAGE_SIZE - 1);
    uint64_t offset = va & (AION_VMM_PAGE_SIZE - 1);
    for (size_t i = 0; i < vmm->count; ++i) {
        if (vmm->entries[i].virtual_address == base) {
            *pa = vmm->entries[i].physical_address + offset;
            *flags = vmm->entries[i].flags;
            return 0;
        }
    }
    return -2;
}
