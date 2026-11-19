#ifndef AION_USB_H
#define AION_USB_H
#include <stddef.h>
#include <stdint.h>
#define AION_USB_OK 0
#define AION_USB_EINVAL (-22)
#define AION_USB_ECORRUPT (-74)
#define AION_USB_ENOSPC (-28)
#define AION_USB_MAX_ENDPOINTS 16u
#define AION_USB_DESCRIPTOR_DEVICE 1u
#define AION_USB_DESCRIPTOR_CONFIGURATION 2u
#define AION_USB_DESCRIPTOR_INTERFACE 4u
#define AION_USB_DESCRIPTOR_ENDPOINT 5u
struct aion_usb_device_descriptor {
    uint16_t vendor_id,product_id,usb_version;
    uint8_t device_class,device_subclass,device_protocol,max_packet_size0,num_configurations;
};
struct aion_usb_endpoint {
    uint8_t address,attributes,interval;
    uint16_t max_packet_size;
};
struct aion_usb_endpoints {
    uint32_t count;
    struct aion_usb_endpoint entries[AION_USB_MAX_ENDPOINTS];
};
int aion_usb_parse_device(const void *bytes,size_t length,struct aion_usb_device_descriptor *out);
int aion_usb_parse_endpoints(const void *bytes,size_t length,struct aion_usb_endpoints *out);
#endif
