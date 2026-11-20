#include "../kernel/hardware/driver_isolation.h"
#include <assert.h>
#include <stdint.h>
int main(void){
    struct aion_driver_policy p;
    assert(aion_driver_policy_init(&p,7)==0);
    assert(aion_driver_grant(&p,0x1000,0x100,AION_DRIVER_PERM_READ|AION_DRIVER_PERM_WRITE)==0);
    assert(aion_driver_authorize(&p,7,0x1000,0x100,AION_DRIVER_PERM_READ)==0);
    assert(aion_driver_authorize(&p,7,0x10ff,1,AION_DRIVER_PERM_WRITE)==0);
    assert(aion_driver_authorize(&p,8,0x1000,1,AION_DRIVER_PERM_READ)==AION_DRIVER_EPERM);
    assert(aion_driver_authorize(&p,7,0x1000,1,AION_DRIVER_PERM_DMA)==AION_DRIVER_EPERM);
    assert(aion_driver_authorize(&p,7,0x10ff,2,AION_DRIVER_PERM_READ)==AION_DRIVER_EPERM);
    assert(aion_driver_grant(&p,0x10ff,2,AION_DRIVER_PERM_READ)==AION_DRIVER_EPERM);
    assert(aion_driver_grant(&p,UINT64_MAX,2,AION_DRIVER_PERM_READ)==AION_DRIVER_EINVAL);
    assert(aion_driver_authorize(&p,7,UINT64_MAX,2,AION_DRIVER_PERM_READ)==AION_DRIVER_EINVAL);
    assert(aion_driver_grant(&p,0x2000,0x100,AION_DRIVER_PERM_DMA)==0);
    assert(aion_driver_authorize(&p,7,0x2000,1,AION_DRIVER_PERM_DMA)==0);
    assert(aion_driver_revoke_all(&p)==0);
    assert(aion_driver_authorize(&p,7,0x1000,1,AION_DRIVER_PERM_READ)==AION_DRIVER_EPERM);
    assert(aion_driver_grant(&p,0x3000,1,AION_DRIVER_PERM_READ)==AION_DRIVER_EINVAL);
    assert(aion_driver_policy_init(&p,0)==AION_DRIVER_EINVAL);
    return 0;
}
