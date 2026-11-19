#ifndef AION_SCHEDULER_H
#define AION_SCHEDULER_H
#include "task.h"
#include <stddef.h>
#include <stdint.h>
struct aion_scheduler {
    struct aion_task_table *table;
    size_t cursor;
    uint64_t running_id;
    uint64_t dispatch_count;
};
int aion_scheduler_init(struct aion_scheduler *s, struct aion_task_table *table);
int aion_scheduler_next(struct aion_scheduler *s, uint64_t *out_id);
int aion_scheduler_block_current(struct aion_scheduler *s);
int aion_scheduler_exit_current(struct aion_scheduler *s);
int aion_scheduler_wake(struct aion_scheduler *s, uint64_t id);
#endif
