/* Fixed-metadata first-fit heap. Caller owns backing memory. */
#include "heap.h"
#include <stdint.h>
static size_t align_up(size_t n) {
    return (n + AION_HEAP_ALIGNMENT - 1u) & ~(size_t)(AION_HEAP_ALIGNMENT - 1u);
}
int aion_heap_init(struct aion_heap *h, void *memory, size_t capacity) {
    if (!h || !memory || !capacity || ((uintptr_t)memory % AION_HEAP_ALIGNMENT)) return -1;
    capacity &= ~(size_t)(AION_HEAP_ALIGNMENT - 1u);
    if (!capacity) return -1;
    h->base = (unsigned char *)memory;
    h->capacity = capacity;
    h->count = 1;
    h->blocks[0] = (struct aion_heap_block){0,capacity,1};
    return 0;
}
void *aion_heap_alloc(struct aion_heap *h, size_t bytes) {
    if (!h || !bytes || bytes > SIZE_MAX-(AION_HEAP_ALIGNMENT-1u)) return 0;
    size_t size=align_up(bytes);
    for (size_t i=0; i<h->count; ++i) {
        struct aion_heap_block *b=&h->blocks[i];
        if (!b->free || b->length<size) continue;
        if (b->length>size) {
            if (h->count==AION_HEAP_MAX_BLOCKS) continue;
            for (size_t j=h->count; j>i+1; --j) h->blocks[j]=h->blocks[j-1];
            h->blocks[i+1]=(struct aion_heap_block){b->offset+size,b->length-size,1};
            h->count++;
            b=&h->blocks[i];
            b->length=size;
        }
        b->free=0;
        return h->base+b->offset;
    }
    return 0;
}
int aion_heap_free(struct aion_heap *h, void *ptr) {
    if (!h || !ptr) return -1;
    uintptr_t base=(uintptr_t)h->base, address=(uintptr_t)ptr;
    if (address<base || address-base>=h->capacity) return -1;
    size_t offset=(size_t)(address-base);
    for (size_t i=0; i<h->count; ++i) {
        if (h->blocks[i].offset!=offset) continue;
        if (h->blocks[i].free) return -2;
        h->blocks[i].free=1;
        if (i+1<h->count && h->blocks[i+1].free) {
            h->blocks[i].length+=h->blocks[i+1].length;
            for (size_t j=i+1; j+1<h->count; ++j) h->blocks[j]=h->blocks[j+1];
            h->count--;
        }
        if (i>0 && h->blocks[i-1].free) {
            h->blocks[i-1].length+=h->blocks[i].length;
            for (size_t j=i; j+1<h->count; ++j) h->blocks[j]=h->blocks[j+1];
            h->count--;
        }
        return 0;
    }
    return -1;
}
size_t aion_heap_free_bytes(const struct aion_heap *h) {
    if (!h) return 0;
    size_t sum=0;
    for (size_t i=0;i<h->count;++i) if (h->blocks[i].free) sum+=h->blocks[i].length;
    return sum;
}
