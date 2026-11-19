#include "privilege.h"
int aion_security_context_init(struct aion_security_context *ctx, uint32_t privilege, uint32_t capabilities) {
    if (!ctx || (privilege != AION_PRIV_KERNEL && privilege != AION_PRIV_USER) || (capabilities & ~AION_CAP_ALL)) return -1;
    if (privilege == AION_PRIV_USER && (capabilities & (AION_CAP_DEVICE | AION_CAP_ADMIN))) return -1;
    ctx->privilege=privilege;
    ctx->capabilities=capabilities;
    return 0;
}
int aion_security_can(const struct aion_security_context *ctx, uint32_t required) {
    if (!ctx || !required || (required & ~AION_CAP_ALL)) return 0;
    return (ctx->capabilities & required)==required;
}
int aion_security_can_access(const struct aion_security_context *ctx, uint32_t resource_privilege, uint32_t required) {
    if (!ctx || (resource_privilege != AION_PRIV_KERNEL && resource_privilege != AION_PRIV_USER)) return 0;
    if (resource_privilege == AION_PRIV_KERNEL && ctx->privilege != AION_PRIV_KERNEL) return 0;
    return aion_security_can(ctx,required);
}
int aion_security_drop(struct aion_security_context *ctx, uint32_t capabilities) {
    if (!ctx || (capabilities & ~AION_CAP_ALL)) return -1;
    ctx->capabilities &= ~capabilities;
    return 0;
}
