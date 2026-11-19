#ifndef AION_PCI_H
#define AION_PCI_H
#include <stdint.h>
#define AION_PCI_MAX_DEVICES 64u
#define AION_PCI_OK 0
#define AION_PCI_EINVAL (-22)
#define AION_PCI_ENOSPC (-28)
typedef uint32_t (*aion_pci_config_read_fn)(void *context,uint8_t bus,uint8_t device,uint8_t function,uint8_t offset);
struct aion_pci_device {
    uint8_t bus,device,function,class_code,subclass,prog_if,header_type;
    uint16_t vendor_id,device_id;
};
struct aion_pci_inventory {
    uint32_t count;
    struct aion_pci_device devices[AION_PCI_MAX_DEVICES];
};
/* Enumerates bus 0 and buses reachable via discovered PCI-to-PCI bridges.
 * Callback must implement aligned 32-bit PCI configuration-space reads.
 */
int aion_pci_discover(aion_pci_config_read_fn read,void *context,struct aion_pci_inventory *inventory);
#endif
