#include "../kernel/task/task.h"
#include <assert.h>
int main(void) {
    struct aion_task_table t;
    uint64_t id=0, other=0;
    assert(aion_task_table_init(&t)==0);
    assert(aion_task_create(&t,0,4096,&id)==-1);
    assert(aion_task_create(&t,4096,0,&id)==-1);
    assert(aion_task_create(&t,4096,8192,&id)==0);
    assert(id==1 && t.count==1);
    assert(aion_task_find(&t,id)->state==AION_TASK_READY);
    assert(aion_task_transition(&t,id,AION_TASK_BLOCKED)==0);
    assert(aion_task_transition(&t,id,AION_TASK_RUNNING)==-1);
    assert(aion_task_transition(&t,id,AION_TASK_READY)==0);
    assert(aion_task_transition(&t,id,AION_TASK_RUNNING)==0);
    assert(aion_task_transition(&t,id,AION_TASK_EXITED)==0);
    assert(aion_task_transition(&t,id,AION_TASK_READY)==-1);
    assert(aion_task_reap(&t,id)==0);
    assert(aion_task_reap(&t,id)==-1);
    assert(aion_task_find(&t,id)==0);
    for (unsigned i=0;i<AION_TASK_MAX;i++) assert(aion_task_create(&t,4096,8192,&other)==0);
    assert(t.count==AION_TASK_MAX);
    assert(aion_task_create(&t,4096,8192,&other)==-1);
    assert(aion_task_transition(&t,other,AION_TASK_EXITED)==0);
    assert(aion_task_reap(&t,other)==0);
    assert(aion_task_create(&t,4096,8192,&other)==0);
    return 0;
}
