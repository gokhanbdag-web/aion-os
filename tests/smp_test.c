#include "../kernel/arch/smp.h"
#include <assert.h>
int main(void) {
    struct aion_smp smp;
    assert(aion_smp_init(&smp,0)==0);
    assert(smp.count==1 && smp.online_count==1);
    assert(aion_smp_register(&smp,1)==0);
    assert(aion_smp_register(&smp,1)==-1);
    assert(aion_smp_register(&smp,0)==-1);
    assert(aion_smp_mark_online(&smp,1)==-1);
    assert(aion_smp_mark_starting(&smp,1)==0);
    assert(aion_smp_mark_starting(&smp,1)==-1);
    assert(aion_smp_mark_online(&smp,1)==0);
    assert(smp.online_count==2);
    assert(aion_smp_mark_online(&smp,1)==-1);
    assert(aion_smp_mark_offline(&smp,0)==-1);
    assert(aion_smp_mark_offline(&smp,1)==0);
    assert(smp.online_count==1);
    assert(aion_smp_mark_offline(&smp,1)==-1);
    for (unsigned i=2;i<64;i++) assert(aion_smp_register(&smp,i)==0);
    assert(smp.count==64);
    assert(aion_smp_register(&smp,64)==-1);
    return 0;
}
