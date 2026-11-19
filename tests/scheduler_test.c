#include "../kernel/task/scheduler.h"
#include <assert.h>
#include <stdint.h>
int main(void) {
    struct aion_task_table table;
    struct aion_scheduler s;
    uint64_t ids[3],next=999;
    assert(aion_task_table_init(&table)==0);
    assert(aion_scheduler_init(&s,&table)==0);
    assert(aion_scheduler_next(&s,&next)==1 && next==0);
    for (unsigned i=0;i<3;i++) assert(aion_task_create(&table,4096,8192,&ids[i])==0);
    for (unsigned i=0;i<90;i++) {
        assert(aion_scheduler_next(&s,&next)==0);
        assert(next==ids[i%3]);
        assert(aion_task_find(&table,next)->state==AION_TASK_RUNNING);
    }
    assert(s.dispatch_count==90);
    assert(aion_scheduler_block_current(&s)==0);
    assert(aion_task_find(&table,ids[2])->state==AION_TASK_BLOCKED);
    for (unsigned i=0;i<10;i++) {
        assert(aion_scheduler_next(&s,&next)==0);
        assert(next!=ids[2]);
    }
    assert(aion_scheduler_wake(&s,ids[2])==0);
    unsigned seen=0;
    for (unsigned i=0;i<6;i++) {
        assert(aion_scheduler_next(&s,&next)==0);
        if (next==ids[2]) seen++;
    }
    assert(seen==2);
    assert(aion_scheduler_exit_current(&s)==0);
    assert(aion_task_find(&table,next)->state==AION_TASK_EXITED);
    assert(aion_task_reap(&table,next)==0);
    for (unsigned i=0;i<20;i++) {
        assert(aion_scheduler_next(&s,&next)==0);
        assert(next!=ids[2]);
    }
    return 0;
}
