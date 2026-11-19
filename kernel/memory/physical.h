#ifndef AION_PHYSICAL_MEMORY_H
#define AION_PHYSICAL_MEMORY_H
#include <stdint.h>
#include <stddef.h>
#define AION_PAGE_SIZE 4096ULL
#define AION_PMM_MAX_REGIONS 128
#define AION_PMM_FREE 1u
#define AION_PMM_RESERVED 2u
struct aion_memory_region {
    uint64_t start;
    uint64_t length;
    uint32_t kind;
};
struct aion_pmm {
    struct aion_memory_region regions[AION_PMM_MAX_REGIONS];
    size_t count;
    uint64_t free_pages;
};
int aion_pmm_init(struct aion_pmm *pmm, const struct aion_memory_region *map, size_t count);
int aion_pmm_reserve(struct aion_pmm *pmm, uint64_t start, uint64_t length);
int aion_pmm_alloc_page(struct aion_pmm *pmm, uint64_t *physical_address);
#endif
