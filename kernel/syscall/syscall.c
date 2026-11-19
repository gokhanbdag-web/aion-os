#include "syscall.h"
int64_t aion_syscall_dispatch(const struct aion_syscall_context *ctx, const struct aion_syscall_frame *frame) {
    if (!ctx || !frame || !ctx->security) return AION_SYSCALL_EINVAL;
    if (ctx->security->privilege != AION_PRIV_USER) return AION_SYSCALL_EPERM;
    if (!aion_security_can(ctx->security, AION_CAP_EXEC)) return AION_SYSCALL_EPERM;
    switch (frame->number) {
        case AION_SYS_GET_VERSION:
            return AION_SYSCALL_VERSION;
        case AION_SYS_GET_TASK_ID:
            if (!ctx->task_id || ctx->task_id > INT64_MAX) return AION_SYSCALL_EINVAL;
            return (int64_t)ctx->task_id;
        case AION_SYS_YIELD:
            if (!ctx->yield_requests || *ctx->yield_requests == UINT64_MAX) return AION_SYSCALL_EINVAL;
            ++*ctx->yield_requests;
            return 0;
        default:
            return AION_SYSCALL_ENOSYS;
    }
}
