#include "../kernel/syscall/syscall.h"
#include <assert.h>
#include <stdint.h>
int main(void) {
    struct aion_security_context user,limited,kernel;
    assert(aion_security_context_init(&user,AION_PRIV_USER,AION_CAP_EXEC|AION_CAP_READ)==0);
    assert(aion_security_context_init(&limited,AION_PRIV_USER,AION_CAP_READ)==0);
    assert(aion_security_context_init(&kernel,AION_PRIV_KERNEL,AION_CAP_ALL)==0);
    uint64_t yields=0;
    struct aion_syscall_context ctx={&user,42,&yields};
    struct aion_syscall_frame frame={AION_SYS_GET_VERSION,{0}};
    assert(aion_syscall_dispatch(&ctx,&frame)==AION_SYSCALL_VERSION);
    frame.number=AION_SYS_GET_TASK_ID;
    assert(aion_syscall_dispatch(&ctx,&frame)==42);
    frame.number=AION_SYS_YIELD;
    assert(aion_syscall_dispatch(&ctx,&frame)==0 && yields==1);
    frame.number=999;
    assert(aion_syscall_dispatch(&ctx,&frame)==AION_SYSCALL_ENOSYS);
    ctx.security=&limited;
    assert(aion_syscall_dispatch(&ctx,&frame)==AION_SYSCALL_EPERM);
    ctx.security=&kernel;
    assert(aion_syscall_dispatch(&ctx,&frame)==AION_SYSCALL_EPERM);
    ctx.security=&user;
    ctx.task_id=0;
    frame.number=AION_SYS_GET_TASK_ID;
    assert(aion_syscall_dispatch(&ctx,&frame)==AION_SYSCALL_EINVAL);
    ctx.task_id=UINT64_MAX;
    assert(aion_syscall_dispatch(&ctx,&frame)==AION_SYSCALL_EINVAL);
    ctx.task_id=42;
    ctx.yield_requests=0;
    frame.number=AION_SYS_YIELD;
    assert(aion_syscall_dispatch(&ctx,&frame)==AION_SYSCALL_EINVAL);
    assert(aion_syscall_dispatch(0,&frame)==AION_SYSCALL_EINVAL);
    assert(aion_syscall_dispatch(&ctx,0)==AION_SYSCALL_EINVAL);
    return 0;
}
