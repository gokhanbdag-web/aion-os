/* Round-robin scheduling decisions; actual CPU context switching is separate. */
#include "scheduler.h"
int aion_scheduler_init(struct aion_scheduler *s, struct aion_task_table *table) {
    if (!s || !table) return -1;
    s->table=table;
    s->cursor=AION_TASK_MAX-1;
    s->running_id=0;
    s->dispatch_count=0;
    return 0;
}
int aion_scheduler_next(struct aion_scheduler *s, uint64_t *out_id) {
    if (!s || !s->table || !out_id) return -1;
    struct aion_task_table *t=s->table;
    if (s->running_id) {
        if (aion_task_transition(t,s->running_id,AION_TASK_READY)!=0) return -1;
        s->running_id=0;
    }
    for (size_t n=0;n<AION_TASK_MAX;n++) {
        size_t i=(s->cursor+1+n)%AION_TASK_MAX;
        if (t->tasks[i].state!=AION_TASK_READY) continue;
        uint64_t id=t->tasks[i].id;
        if (aion_task_transition(t,id,AION_TASK_RUNNING)!=0) return -1;
        s->cursor=i;
        s->running_id=id;
        s->dispatch_count++;
        *out_id=id;
        return 0;
    }
    *out_id=0;
    return 1; /* no runnable task */
}
int aion_scheduler_block_current(struct aion_scheduler *s) {
    if (!s || !s->table || !s->running_id) return -1;
    if (aion_task_transition(s->table,s->running_id,AION_TASK_BLOCKED)!=0) return -1;
    s->running_id=0;
    return 0;
}
int aion_scheduler_exit_current(struct aion_scheduler *s) {
    if (!s || !s->table || !s->running_id) return -1;
    if (aion_task_transition(s->table,s->running_id,AION_TASK_EXITED)!=0) return -1;
    s->running_id=0;
    return 0;
}
int aion_scheduler_wake(struct aion_scheduler *s, uint64_t id) {
    if (!s || !s->table) return -1;
    return aion_task_transition(s->table,id,AION_TASK_READY);
}
