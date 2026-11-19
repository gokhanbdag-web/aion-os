#ifndef AION_PRIVILEGE_H
#define AION_PRIVILEGE_H
#include <stdint.h>
#define AION_PRIV_KERNEL 0u
#define AION_PRIV_USER 3u
#define AION_CAP_READ  (1u << 0)
#define AION_CAP_WRITE (1u << 1)
#define AION_CAP_EXEC  (1u << 2)
#define AION_CAP_DEVICE (1u << 3)
#define AION_CAP_ADMIN (1u << 4)
#define AION_CAP_ALL (AION_CAP_READ | AION_CAP_WRITE | AION_CAP_EXEC | AION_CAP_DEVICE | AION_CAP_ADMIN)
struct aion_security_context {
    uint32_t privilege;
    uint32_t capabilities;
};
int aion_security_context_init(struct aion_security_context *ctx, uint32_t privilege, uint32_t capabilities);
int aion_security_can(const struct aion_security_context *ctx, uint32_t required);
int aion_security_can_access(const struct aion_security_context *ctx, uint32_t resource_privilege, uint32_t required);
int aion_security_drop(struct aion_security_context *ctx, uint32_t capabilities);
#endif
