#include "../kernel/hardware/nvme.h"
#include <assert.h>
#include <string.h>
int main(void){
    unsigned char ctrl[4096]={0},ns[4096]={0};
    struct aion_nvme_controller c;struct aion_nvme_namespace n;
    ctrl[0]=0x86;ctrl[1]=0x80;ctrl[516]=1;memcpy(ctrl+24,"AION TEST NVME",14);
    assert(aion_nvme_parse_controller(ctrl,sizeof(ctrl),&c)==0);
    assert(c.vendor_id==0x8086 && c.namespace_count==1);
    ns[0]=0x00;ns[1]=0x10;ns[25]=0;ns[26]=0;ns[130]=9;
    assert(aion_nvme_parse_namespace(ns,sizeof(ns),&n)==0);
    assert(n.block_count==4096 && n.block_size==512);
    ns[130]=12;assert(aion_nvme_parse_namespace(ns,sizeof(ns),&n)==0 && n.block_size==4096);
    ns[128]=8;assert(aion_nvme_parse_namespace(ns,sizeof(ns),&n)==AION_NVME_EUNSUPPORTED);
    ns[128]=0;ns[26]=2;assert(aion_nvme_parse_namespace(ns,sizeof(ns),&n)==AION_NVME_EUNSUPPORTED);
    assert(aion_nvme_parse_namespace(ns,512,&n)==AION_NVME_EINVAL);
    assert(aion_nvme_parse_controller(0,4096,&c)==AION_NVME_EINVAL);
    return 0;
}
