#ifndef AION_SYSCALL_H
#define AION_SYSCALL_H
#include <stdint.h>
#include "../security/privilege.h"
#define AION_SYS_GET_VERSION 1u
#define AION_SYS_GET_TASK_ID 2u
#define AION_SYS_YIELD 3u
#define AION_SYS_MAX 3u
#define AION_SYSCALL_VERSION 1u
#define AION_SYSCALL_EINVAL (-22)
#define AION_SYSCALL_ENOSYS (-38)
#define AION_SYSCALL_EPERM (-1)
struct aion_syscall_frame {
    uint64_t number;
    uint64_t args[6];
};
struct aion_syscall_context {
    const struct aion_security_context *security;
    uint64_t task_id;
    uint64_t *yield_requests;
};
int64_t aion_syscall_dispatch(const struct aion_syscall_context *ctx, const struct aion_syscall_frame *frame);
#endif
