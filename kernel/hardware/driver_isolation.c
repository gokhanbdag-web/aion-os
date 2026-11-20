#include "driver_isolation.h"
#include <stdint.h>
static int valid_range(uint64_t base,uint64_t length){
    return length!=0 && base<=UINT64_MAX-(length-1u);
}
static int valid_permissions(uint32_t permissions){
    return permissions!=0 && (permissions & ~(AION_DRIVER_PERM_READ|AION_DRIVER_PERM_WRITE|AION_DRIVER_PERM_DMA))==0;
}
int aion_driver_policy_init(struct aion_driver_policy *policy,uint32_t owner_id){
    unsigned i;
    if(!policy || !owner_id)return AION_DRIVER_EINVAL;
    policy->owner_id=owner_id;policy->enabled=1;policy->region_count=0;
    for(i=0;i<AION_DRIVER_MAX_REGIONS;i++){
        policy->regions[i].base=0;policy->regions[i].length=0;policy->regions[i].permissions=0;
    }
    return AION_DRIVER_OK;
}
int aion_driver_grant(struct aion_driver_policy *policy,uint64_t base,uint64_t length,uint32_t permissions){
    uint32_t i;
    uint64_t end;
    if(!policy || !policy->enabled || !valid_range(base,length) || !valid_permissions(permissions))return AION_DRIVER_EINVAL;
    end=base+length-1u;
    for(i=0;i<policy->region_count;i++){
        const struct aion_driver_region *r=&policy->regions[i];
        uint64_t existing_end=r->base+r->length-1u;
        if(base<=existing_end && r->base<=end)return AION_DRIVER_EPERM;
    }
    if(policy->region_count==AION_DRIVER_MAX_REGIONS)return AION_DRIVER_ENOSPC;
    policy->regions[policy->region_count].base=base;
    policy->regions[policy->region_count].length=length;
    policy->regions[policy->region_count].permissions=permissions;
    policy->region_count++;
    return AION_DRIVER_OK;
}
int aion_driver_authorize(const struct aion_driver_policy *policy,uint32_t requester,uint64_t base,uint64_t length,uint32_t permissions){
    uint32_t i;
    uint64_t end;
    if(!policy || !valid_range(base,length) || !valid_permissions(permissions))return AION_DRIVER_EINVAL;
    if(!policy->enabled || requester!=policy->owner_id)return AION_DRIVER_EPERM;
    end=base+length-1u;
    for(i=0;i<policy->region_count;i++){
        const struct aion_driver_region *r=&policy->regions[i];
        uint64_t region_end=r->base+r->length-1u;
        if(base>=r->base && end<=region_end && (permissions&r->permissions)==permissions)return AION_DRIVER_OK;
    }
    return AION_DRIVER_EPERM;
}
int aion_driver_revoke_all(struct aion_driver_policy *policy){
    unsigned i;
    if(!policy)return AION_DRIVER_EINVAL;
    policy->enabled=0;policy->region_count=0;
    for(i=0;i<AION_DRIVER_MAX_REGIONS;i++){
        policy->regions[i].base=0;policy->regions[i].length=0;policy->regions[i].permissions=0;
    }
    return AION_DRIVER_OK;
}
