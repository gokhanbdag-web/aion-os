#include "physical.h"
#include <stdint.h>
#define MAX_U64 UINT64_MAX
static int overlap(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    return a < d && c < b;
}
static int push(struct aion_pmm *p, uint64_t start, uint64_t length, uint32_t kind) {
    if (!length) return 0;
    if (p->count == AION_PMM_MAX_REGIONS) return -1;
    p->regions[p->count++] = (struct aion_memory_region){start,length,kind};
    return 0;
}
int aion_pmm_init(struct aion_pmm *p, const struct aion_memory_region *map, size_t count) {
    if (!p || (!map && count) || count > AION_PMM_MAX_REGIONS) return -1;
    p->count = 0; p->free_pages = 0;
    for (size_t i=0; i<count; ++i) {
        uint64_t start=map[i].start, length=map[i].length;
        if (!length || start > MAX_U64-length) return -1;
        for (size_t j=0; j<i; ++j)
            if (overlap(start,start+length,map[j].start,map[j].start+map[j].length)) return -1;
        if (map[i].kind != AION_PMM_FREE && map[i].kind != AION_PMM_RESERVED) return -1;
        if (map[i].kind == AION_PMM_FREE) {
            if (start > MAX_U64-(AION_PAGE_SIZE-1)) return -1;
            uint64_t aligned = (start + (AION_PAGE_SIZE-1)) & ~(AION_PAGE_SIZE-1);
            uint64_t end = (start+length) & ~(AION_PAGE_SIZE-1);
            if (end <= aligned) continue;
            start = aligned; length = end-aligned;
            p->free_pages += length / AION_PAGE_SIZE;
        }
        if (push(p,start,length,map[i].kind)) return -1;
    }
    return 0;
}
int aion_pmm_reserve(struct aion_pmm *p, uint64_t start, uint64_t length) {
    if (!p || !length || start % AION_PAGE_SIZE || length % AION_PAGE_SIZE || start > MAX_U64-length) return -1;
    uint64_t end=start+length;
    /* Atomic validation: only a single free region can cover a reservation. */
    for (size_t i=0; i<p->count; ++i) {
        struct aion_memory_region r=p->regions[i];
        if (r.kind != AION_PMM_FREE || start < r.start || end > r.start+r.length) continue;
        size_t extra=(start>r.start)+(end<r.start+r.length);
        if (p->count+extra > AION_PMM_MAX_REGIONS) return -1;
        p->regions[i]=(struct aion_memory_region){start,length,AION_PMM_RESERVED};
        if (start>r.start) push(p,r.start,start-r.start,AION_PMM_FREE);
        if (end<r.start+r.length) push(p,end,r.start+r.length-end,AION_PMM_FREE);
        p->free_pages-=length/AION_PAGE_SIZE;
        return 0;
    }
    return -1;
}
int aion_pmm_alloc_page(struct aion_pmm *p, uint64_t *address) {
    if (!p || !address) return -1;
    for (size_t i=0; i<p->count; ++i) {
        struct aion_memory_region r=p->regions[i];
        if (r.kind != AION_PMM_FREE || r.length < AION_PAGE_SIZE) continue;
        if (r.length == AION_PAGE_SIZE) {
            p->regions[i].kind=AION_PMM_RESERVED;
        } else {
            if (p->count == AION_PMM_MAX_REGIONS) return -1;
            p->regions[i].start+=AION_PAGE_SIZE;
            p->regions[i].length-=AION_PAGE_SIZE;
            if (push(p,r.start,AION_PAGE_SIZE,AION_PMM_RESERVED)) return -1;
        }
        p->free_pages--;
        *address=r.start;
        return 0;
    }
    return -1;
}
