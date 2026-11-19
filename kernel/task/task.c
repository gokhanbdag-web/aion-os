/* Freestanding task descriptor table; context switching is a later milestone. */
#include "task.h"
#include <stdint.h>
const struct aion_task *aion_task_find(const struct aion_task_table *t, uint64_t id) {
    if (!t || !id) return 0;
    for (size_t i=0;i<AION_TASK_MAX;i++)
        if (t->tasks[i].state!=AION_TASK_UNUSED && t->tasks[i].id==id) return &t->tasks[i];
    return 0;
}
int aion_task_table_init(struct aion_task_table *t) {
    if (!t) return -1;
    for (size_t i=0;i<AION_TASK_MAX;i++) t->tasks[i]=(struct aion_task){0,0,0,0};
    t->count=0;
    t->next_id=1;
    return 0;
}
int aion_task_create(struct aion_task_table *t, uintptr_t entry, uintptr_t stack_top, uint64_t *out_id) {
    if (!t || !out_id || !entry || !stack_top || t->count>=AION_TASK_MAX || t->next_id==UINT64_MAX) return -1;
    for (size_t i=0;i<AION_TASK_MAX;i++) {
        if (t->tasks[i].state!=AION_TASK_UNUSED) continue;
        uint64_t id=t->next_id++;
        t->tasks[i]=(struct aion_task){id,AION_TASK_READY,entry,stack_top};
        t->count++;
        *out_id=id;
        return 0;
    }
    return -1;
}
int aion_task_transition(struct aion_task_table *t, uint64_t id, uint32_t state) {
    if (!t || state<AION_TASK_READY || state>AION_TASK_EXITED) return -1;
    for (size_t i=0;i<AION_TASK_MAX;i++) {
        struct aion_task *task=&t->tasks[i];
        if (task->state==AION_TASK_UNUSED || task->id!=id) continue;
        uint32_t old=task->state;
        int allowed=(old==AION_TASK_READY && (state==AION_TASK_RUNNING || state==AION_TASK_BLOCKED || state==AION_TASK_EXITED))
          || (old==AION_TASK_RUNNING && (state==AION_TASK_READY || state==AION_TASK_BLOCKED || state==AION_TASK_EXITED))
          || (old==AION_TASK_BLOCKED && (state==AION_TASK_READY || state==AION_TASK_EXITED));
        if (!allowed) return -1;
        task->state=state;
        return 0;
    }
    return -1;
}
int aion_task_reap(struct aion_task_table *t, uint64_t id) {
    if (!t) return -1;
    for (size_t i=0;i<AION_TASK_MAX;i++) {
        if (t->tasks[i].id!=id || t->tasks[i].state!=AION_TASK_EXITED) continue;
        t->tasks[i]=(struct aion_task){0,0,0,0};
        t->count--;
        return 0;
    }
    return -1;
}
