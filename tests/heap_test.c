#include "../kernel/memory/heap.h"
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#if defined(__GNUC__)
static unsigned char arena[4096] __attribute__((aligned(16)));
#else
static _Alignas(16) unsigned char arena[4096];
#endif
int main(void) {
    struct aion_heap h;
    assert(aion_heap_init(&h,arena,sizeof arena)==0);
    assert(aion_heap_free_bytes(&h)==4096);
    void *a=aion_heap_alloc(&h,1), *b=aion_heap_alloc(&h,31), *c=aion_heap_alloc(&h,48);
    assert(a && b && c && a!=b && b!=c);
    assert((uintptr_t)a%16==0 && (uintptr_t)b%16==0);
    assert(aion_heap_free(&h,b)==0);
    assert(aion_heap_free(&h,b)==-2);
    assert(aion_heap_alloc(&h,32)==b);
    assert(aion_heap_free(&h,a)==0);
    assert(aion_heap_free(&h,b)==0);
    assert(aion_heap_free(&h,c)==0);
    assert(aion_heap_free_bytes(&h)==4096);
    assert(aion_heap_alloc(&h,4096)==arena);
    assert(aion_heap_alloc(&h,1)==0);
    assert(aion_heap_free(&h,arena+1)==-1);
    assert(aion_heap_free(&h,arena)==0);
    assert(aion_heap_alloc(&h,0)==0);
    assert(aion_heap_alloc(&h,SIZE_MAX)==0);
    return 0;
}
