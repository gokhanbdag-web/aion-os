#include "../kernel/network/nic.h"
#include <assert.h>
int main(void){
    struct aion_nic_inventory inventory;
    struct aion_nic_device eth={0x8086,0x100e,0,3,0,2,0,0};
    struct aion_nic_device wifi={0x8086,0x1234,0,4,0,2,0x80,0};
    struct aion_nic_device bad={0x1234,0x5678,0,5,0,3,0,0};
    size_t i;
    aion_nic_inventory_init(&inventory);
    assert(aion_nic_add_pci(&inventory,&eth)==0);
    assert(aion_nic_add_pci(&inventory,&eth)==AION_NIC_EINVAL);
    assert(aion_nic_add_pci(&inventory,&wifi)==0);
    assert(aion_nic_add_pci(&inventory,&bad)==AION_NIC_EINVAL);
    assert(inventory.count==2);
    assert(aion_nic_find_ethernet(&inventory,0)->device_id==0x100e);
    assert(aion_nic_find_ethernet(&inventory,1)==0);
    for(i=2;i<AION_NIC_MAX_DEVICES;i++){eth.slot=(uint8_t)(i+3);eth.bus=(uint8_t)(i/20);if(eth.slot>=32)eth.slot=(uint8_t)(i%20);assert(aion_nic_add_pci(&inventory,&eth)==0);}
    assert(inventory.count==AION_NIC_MAX_DEVICES);
    eth.bus=9;eth.slot=1;assert(aion_nic_add_pci(&inventory,&eth)==AION_NIC_ENOSPC);
    assert(aion_nic_add_pci(0,&eth)==AION_NIC_EINVAL);
    return 0;
}
