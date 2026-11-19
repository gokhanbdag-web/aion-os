#include "usb.h"
static uint16_t le16(const uint8_t *p){return (uint16_t)((uint16_t)p[0]|((uint16_t)p[1]<<8));}
int aion_usb_parse_device(const void *bytes,size_t length,struct aion_usb_device_descriptor *out){
    const uint8_t *p=(const uint8_t *)bytes;
    if(!p || !out || length<18)return AION_USB_EINVAL;
    if(p[0]<18 || p[1]!=AION_USB_DESCRIPTOR_DEVICE)return AION_USB_ECORRUPT;
    out->usb_version=le16(p+2);
    out->device_class=p[4];out->device_subclass=p[5];out->device_protocol=p[6];out->max_packet_size0=p[7];
    out->vendor_id=le16(p+8);out->product_id=le16(p+10);out->num_configurations=p[17];
    if(!out->max_packet_size0 || !out->num_configurations)return AION_USB_ECORRUPT;
    return AION_USB_OK;
}
int aion_usb_parse_endpoints(const void *bytes,size_t length,struct aion_usb_endpoints *out){
    const uint8_t *p=(const uint8_t *)bytes;size_t pos=0,limit=length;
    if(!p || !out || length<9)return AION_USB_EINVAL;
    out->count=0;
    if(p[1]!=AION_USB_DESCRIPTOR_CONFIGURATION || p[0]<9 || p[0]>length)return AION_USB_ECORRUPT;
    limit=le16(p+2);
    if(limit<9 || limit>length)return AION_USB_ECORRUPT;
    while(pos<limit){
        unsigned size,type;
        struct aion_usb_endpoint *ep;
        if(limit-pos<2)return AION_USB_ECORRUPT;
        size=p[pos];type=p[pos+1];
        if(size<2 || size>limit-pos)return AION_USB_ECORRUPT;
        if(type==AION_USB_DESCRIPTOR_ENDPOINT){
            if(size<7)return AION_USB_ECORRUPT;
            if(out->count==AION_USB_MAX_ENDPOINTS)return AION_USB_ENOSPC;
            ep=&out->entries[out->count++];
            ep->address=p[pos+2];ep->attributes=p[pos+3];
            ep->max_packet_size=le16(p+pos+4);ep->interval=p[pos+6];
            if(!ep->max_packet_size)return AION_USB_ECORRUPT;
        }
        pos+=size;
    }
    return AION_USB_OK;
}
