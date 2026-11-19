#include "../kernel/hardware/pci.h"
#include <assert.h>
#include <string.h>
static uint32_t rd(void *ctx,uint8_t b,uint8_t d,uint8_t f,uint8_t off){
    (void)ctx;
    if(b==0 && d==0 && f==0){
        if(off==0)return 0x12348086u;
        if(off==8)return 0x06000000u;
        if(off==0x0c)return 0;
    }
    if(b==0 && d==1 && f==0){
        if(off==0)return 0x244e8086u;
        if(off==8)return 0x06040000u;
        if(off==0x0c)return 0x00010000u;
        if(off==0x18)return 0x00010100u;
    }
    if(b==1 && d==2 && f==0){
        if(off==0)return 0x100e8086u;
        if(off==8)return 0x02000000u;
        if(off==0x0c)return 0;
    }
    return 0xffffffffu;
}
int main(void){
    struct aion_pci_inventory inventory;
    assert(aion_pci_discover(rd,0,&inventory)==0);
    assert(inventory.count==3);
    assert(inventory.devices[0].vendor_id==0x8086);
    assert(inventory.devices[1].class_code==6);
    assert(inventory.devices[2].bus==1 && inventory.devices[2].device==2);
    assert(inventory.devices[2].class_code==2);
    assert(aion_pci_discover(0,0,&inventory)==AION_PCI_EINVAL);
    assert(aion_pci_discover(rd,0,0)==AION_PCI_EINVAL);
    memset(&inventory,0,sizeof(inventory));
    return 0;
}
