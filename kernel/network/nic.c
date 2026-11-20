#include "nic.h"
void aion_nic_inventory_init(struct aion_nic_inventory *inventory){
    if(inventory)inventory->count=0;
}
int aion_nic_add_pci(struct aion_nic_inventory *inventory,const struct aion_nic_device *device){
    size_t i;
    if(!inventory || !device || inventory->count>AION_NIC_MAX_DEVICES || device->slot>=32u || device->function>=8u || device->vendor_id==0xffffu)return AION_NIC_EINVAL;
    if(device->class_code!=AION_NIC_PCI_CLASS_NETWORK)return AION_NIC_EINVAL;
    for(i=0;i<inventory->count;i++){
        const struct aion_nic_device *existing=&inventory->devices[i];
        if(existing->bus==device->bus && existing->slot==device->slot && existing->function==device->function)return AION_NIC_EINVAL;
    }
    if(inventory->count==AION_NIC_MAX_DEVICES)return AION_NIC_ENOSPC;
    inventory->devices[inventory->count++]=*device;
    return AION_NIC_OK;
}
const struct aion_nic_device *aion_nic_find_ethernet(const struct aion_nic_inventory *inventory,size_t ordinal){
    size_t i;
    if(!inventory || inventory->count>AION_NIC_MAX_DEVICES)return 0;
    for(i=0;i<inventory->count;i++){
        const struct aion_nic_device *d=&inventory->devices[i];
        if(d->subclass==AION_NIC_PCI_SUBCLASS_ETHERNET){
            if(!ordinal)return d;
            ordinal--;
        }
    }
    return 0;
}
