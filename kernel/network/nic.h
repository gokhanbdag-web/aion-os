#ifndef AION_NIC_H
#define AION_NIC_H
#include <stddef.h>
#include <stdint.h>
#define AION_NIC_OK 0
#define AION_NIC_EINVAL (-22)
#define AION_NIC_ENOSPC (-28)
#define AION_NIC_MAX_DEVICES 32u
#define AION_NIC_PCI_CLASS_NETWORK 0x02u
#define AION_NIC_PCI_SUBCLASS_ETHERNET 0x00u
struct aion_nic_device {
    uint16_t vendor_id,device_id;
    uint8_t bus,slot,function;
    uint8_t class_code,subclass,programming_interface;
};
struct aion_nic_inventory {
    size_t count;
    struct aion_nic_device devices[AION_NIC_MAX_DEVICES];
};
void aion_nic_inventory_init(struct aion_nic_inventory *inventory);
int aion_nic_add_pci(struct aion_nic_inventory *inventory,const struct aion_nic_device *device);
const struct aion_nic_device *aion_nic_find_ethernet(const struct aion_nic_inventory *inventory,size_t ordinal);
#endif
