#include "../kernel/hardware/usb.h"
#include <assert.h>
int main(void){
    const unsigned char device[18]={18,1,0x00,0x02,0,0,0,64,0x34,0x12,0x78,0x56,0,1,2,3,0,1};
    unsigned char config[25]={9,2,25,0,1,1,0,0x80,50,9,4,0,0,1,3,1,1,0,7,5,0x81,3,8,0,10};
    struct aion_usb_device_descriptor d;struct aion_usb_endpoints e;
    assert(aion_usb_parse_device(device,18,&d)==0);
    assert(d.vendor_id==0x1234 && d.product_id==0x5678 && d.max_packet_size0==64);
    assert(aion_usb_parse_endpoints(config,25,&e)==0);
    assert(e.count==1 && e.entries[0].address==0x81 && e.entries[0].max_packet_size==8);
    config[18]=0;assert(aion_usb_parse_endpoints(config,25,&e)==AION_USB_ECORRUPT);
    config[18]=7;config[2]=26;assert(aion_usb_parse_endpoints(config,25,&e)==AION_USB_ECORRUPT);
    assert(aion_usb_parse_device(device,17,&d)==AION_USB_EINVAL);
    assert(aion_usb_parse_endpoints(0,0,&e)==AION_USB_EINVAL);
    return 0;
}
