#ifndef AION_DRIVER_ISOLATION_H
#define AION_DRIVER_ISOLATION_H
#include <stddef.h>
#include <stdint.h>
#define AION_DRIVER_OK 0
#define AION_DRIVER_EINVAL (-22)
#define AION_DRIVER_EPERM (-1)
#define AION_DRIVER_ENOSPC (-28)
#define AION_DRIVER_MAX_REGIONS 16u
#define AION_DRIVER_PERM_READ 1u
#define AION_DRIVER_PERM_WRITE 2u
#define AION_DRIVER_PERM_DMA 4u
struct aion_driver_region { uint64_t base,length; uint32_t permissions; };
struct aion_driver_policy {
    uint32_t owner_id;
    uint8_t enabled;
    uint32_t region_count;
    struct aion_driver_region regions[AION_DRIVER_MAX_REGIONS];
};
int aion_driver_policy_init(struct aion_driver_policy *policy,uint32_t owner_id);
int aion_driver_grant(struct aion_driver_policy *policy,uint64_t base,uint64_t length,uint32_t permissions);
int aion_driver_authorize(const struct aion_driver_policy *policy,uint32_t requester,uint64_t base,uint64_t length,uint32_t permissions);
int aion_driver_revoke_all(struct aion_driver_policy *policy);
#endif
