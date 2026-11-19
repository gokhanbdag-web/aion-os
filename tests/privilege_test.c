#include "../kernel/security/privilege.h"
#include <assert.h>
int main(void) {
    struct aion_security_context kernel,user;
    assert(aion_security_context_init(&kernel,AION_PRIV_KERNEL,AION_CAP_ALL)==0);
    assert(aion_security_context_init(&user,AION_PRIV_USER,AION_CAP_READ|AION_CAP_WRITE)==0);
    assert(aion_security_context_init(&user,1,AION_CAP_READ)!=0);
    assert(aion_security_context_init(&user,AION_PRIV_USER,AION_CAP_ADMIN)!=0);
    assert(aion_security_context_init(&user,AION_PRIV_USER,AION_CAP_READ|AION_CAP_WRITE)==0);
    assert(aion_security_can(&user,AION_CAP_READ));
    assert(!aion_security_can(&user,AION_CAP_EXEC));
    assert(!aion_security_can(&user,AION_CAP_ADMIN));
    assert(!aion_security_can_access(&user,AION_PRIV_KERNEL,AION_CAP_READ));
    assert(aion_security_can_access(&user,AION_PRIV_USER,AION_CAP_READ));
    assert(aion_security_can_access(&kernel,AION_PRIV_KERNEL,AION_CAP_ADMIN));
    assert(aion_security_drop(&user,AION_CAP_WRITE)==0);
    assert(!aion_security_can(&user,AION_CAP_WRITE));
    assert(aion_security_drop(&user,AION_CAP_ADMIN)==0);
    assert(!aion_security_can(&user,0));
    assert(!aion_security_can(&user,0x80000000u));
    assert(!aion_security_can_access(&kernel,2,AION_CAP_READ));
    return 0;
}
