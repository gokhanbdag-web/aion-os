#ifndef AION_TASK_H
#define AION_TASK_H
#include <stddef.h>
#include <stdint.h>
#define AION_TASK_MAX 128
#define AION_TASK_UNUSED 0u
#define AION_TASK_READY 1u
#define AION_TASK_RUNNING 2u
#define AION_TASK_BLOCKED 3u
#define AION_TASK_EXITED 4u
struct aion_task {
    uint64_t id;
    uint32_t state;
    uintptr_t entry;
    uintptr_t stack_top;
};
struct aion_task_table {
    struct aion_task tasks[AION_TASK_MAX];
    size_t count;
    uint64_t next_id;
};
int aion_task_table_init(struct aion_task_table *table);
int aion_task_create(struct aion_task_table *table, uintptr_t entry, uintptr_t stack_top, uint64_t *out_id);
int aion_task_transition(struct aion_task_table *table, uint64_t id, uint32_t new_state);
int aion_task_reap(struct aion_task_table *table, uint64_t id);
const struct aion_task *aion_task_find(const struct aion_task_table *table, uint64_t id);
#endif
