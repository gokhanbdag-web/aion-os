#ifndef AION_HEAP_H
#define AION_HEAP_H
#include <stddef.h>
#include <stdint.h>
#define AION_HEAP_MAX_BLOCKS 256
#define AION_HEAP_ALIGNMENT 16u
struct aion_heap_block {
    size_t offset;
    size_t length;
    int free;
};
struct aion_heap {
    unsigned char *base;
    size_t capacity;
    struct aion_heap_block blocks[AION_HEAP_MAX_BLOCKS];
    size_t count;
};
int aion_heap_init(struct aion_heap *heap, void *memory, size_t capacity);
void *aion_heap_alloc(struct aion_heap *heap, size_t bytes);
int aion_heap_free(struct aion_heap *heap, void *ptr);
size_t aion_heap_free_bytes(const struct aion_heap *heap);
#endif
