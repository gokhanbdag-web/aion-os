#include "../kernel/network/ethernet.h"
#include <assert.h>
int main(void){
    uint8_t buf[64]={0};uint8_t dst[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    uint8_t src[6]={0,1,2,3,4,5};uint8_t payload[3]={1,2,3};
    struct aion_ethernet_frame f;size_t written=0;
    assert(aion_ethernet_build(buf,sizeof(buf),dst,src,0x0800,payload,3,&written)==0 && written==17);
    assert(aion_ethernet_parse(buf,written,&f)==0);
    assert(f.ethertype==0x0800 && f.payload_length==3 && f.payload[2]==3 && !f.tagged);
    assert(aion_ethernet_parse(buf,13,&f)==AION_ETHERNET_ECORRUPT);
    assert(aion_ethernet_build(buf,16,dst,src,0x0800,payload,3,&written)==AION_ETHERNET_EINVAL);
    buf[12]=0x81;buf[13]=0;buf[14]=0xa0;buf[15]=0x2a;buf[16]=0x08;buf[17]=0x06;
    assert(aion_ethernet_parse(buf,18,&f)==0 && f.tagged && f.vlan_id==42 && f.priority==5 && f.ethertype==0x0806);
    assert(aion_ethernet_parse(buf,17,&f)==AION_ETHERNET_ECORRUPT);
    buf[16]=0x81;buf[17]=0;assert(aion_ethernet_parse(buf,18,&f)==AION_ETHERNET_ECORRUPT);
    assert(aion_ethernet_parse(0,18,&f)==AION_ETHERNET_EINVAL);
    return 0;
}
