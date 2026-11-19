#include "../kernel/security/privilege.h"
#include "../kernel/syscall/syscall.h"
#include "../kernel/ipc/ipc.h"
#include <assert.h>
#include <stdint.h>
int main(void) {
    static struct aion_ipc_bus bus;
    struct aion_security_context user,read_only,kernel,invalid;
    struct aion_syscall_context syscall_ctx;
    struct aion_syscall_frame frame={AION_SYS_GET_VERSION,{0}};
    struct aion_ipc_message message;
    uint64_t yields=0;
    uint8_t payload=42;
    size_t i;
    assert(aion_security_context_init(&user,AION_PRIV_USER,AION_CAP_EXEC|AION_CAP_READ|AION_CAP_WRITE)==0);
    assert(aion_security_context_init(&read_only,AION_PRIV_USER,AION_CAP_READ)==0);
    assert(aion_security_context_init(&kernel,AION_PRIV_KERNEL,AION_CAP_ALL)==0);
    assert(aion_security_context_init(&invalid,AION_PRIV_USER,AION_CAP_ADMIN)!=0);
    assert(!aion_security_can_access(&user,AION_PRIV_KERNEL,AION_CAP_READ));
    assert(!aion_security_can_access(&user,AION_PRIV_KERNEL,AION_CAP_EXEC));
    assert(!aion_security_can_access(&user,AION_PRIV_KERNEL,AION_CAP_WRITE));
    assert(aion_security_can_access(&kernel,AION_PRIV_KERNEL,AION_CAP_ADMIN));
    assert(aion_security_drop(&user,AION_CAP_EXEC)==0);
    assert(!aion_security_can(&user,AION_CAP_EXEC));
    assert(aion_security_drop(&user,AION_CAP_EXEC)==0);
    assert(!aion_security_can(&user,AION_CAP_EXEC));
    syscall_ctx.security=&user; syscall_ctx.task_id=7; syscall_ctx.yield_requests=&yields;
    assert(aion_syscall_dispatch(&syscall_ctx,&frame)==AION_SYSCALL_EPERM);
    syscall_ctx.security=&kernel;
    assert(aion_syscall_dispatch(&syscall_ctx,&frame)==AION_SYSCALL_EPERM);
    syscall_ctx.security=&read_only;
    assert(aion_syscall_dispatch(&syscall_ctx,&frame)==AION_SYSCALL_EPERM);
    assert(aion_ipc_init(&bus)==0);
    assert(aion_ipc_open(&bus,7)==0);
    assert(aion_ipc_send(&bus,&read_only,8,7,&payload,1)==AION_IPC_EPERM);
    assert(aion_ipc_receive(&bus,&user,7,&message)==AION_IPC_EAGAIN);
    for (i=0;i<AION_IPC_QUEUE_DEPTH;i++) assert(aion_ipc_send(&bus,&user,8,7,&payload,1)==0);
    assert(aion_ipc_send(&bus,&user,8,7,&payload,1)==AION_IPC_EAGAIN);
    for (i=0;i<AION_IPC_QUEUE_DEPTH;i++) {
        assert(aion_ipc_receive(&bus,&user,7,&message)==0);
        assert(message.sender==8 && message.length==1 && message.data[0]==42);
    }
    assert(aion_ipc_close(&bus,7)==0);
    assert(aion_ipc_open(&bus,7)==0);
    assert(aion_ipc_receive(&bus,&user,7,&message)==AION_IPC_EAGAIN);
    return 0;
}
