#include "ethernet.h"
static uint16_t be16(const uint8_t *p){return (uint16_t)(((uint16_t)p[0]<<8)|p[1]);}
int aion_ethernet_parse(const void *buffer,size_t length,struct aion_ethernet_frame *out){
    const uint8_t *p=(const uint8_t *)buffer;size_t header=AION_ETHERNET_HEADER_SIZE;unsigned i;uint16_t type;
    if(!p || !out)return AION_ETHERNET_EINVAL;
    if(length<AION_ETHERNET_HEADER_SIZE || length>AION_ETHERNET_MAX_FRAME)return AION_ETHERNET_ECORRUPT;
    type=be16(p+12);
    out->tagged=0;out->vlan_id=0;out->priority=0;
    if(type==AION_ETHERTYPE_VLAN || type==AION_ETHERTYPE_QINQ){
        uint16_t tci;
        if(length<AION_ETHERNET_VLAN_HEADER_SIZE)return AION_ETHERNET_ECORRUPT;
        tci=be16(p+14);
        out->tagged=1;out->vlan_id=(uint16_t)(tci&0x0fffu);out->priority=(uint8_t)(tci>>13);
        type=be16(p+16);header=AION_ETHERNET_VLAN_HEADER_SIZE;
    }
    /* IEEE 802.3 length fields and stacked VLANs require separate handling. */
    if(type<0x0600u || type==AION_ETHERTYPE_VLAN || type==AION_ETHERTYPE_QINQ)return AION_ETHERNET_ECORRUPT;
    for(i=0;i<6;i++){out->destination[i]=p[i];out->source[i]=p[6+i];}
    out->ethertype=type;out->payload=p+header;out->payload_length=length-header;
    return AION_ETHERNET_OK;
}
int aion_ethernet_build(void *buffer,size_t capacity,const uint8_t destination[6],const uint8_t source[6],uint16_t ethertype,const void *payload,size_t payload_length,size_t *written){
    uint8_t *p=(uint8_t *)buffer;size_t i;
    if(!p || !destination || !source || !written || (!payload && payload_length))return AION_ETHERNET_EINVAL;
    if(ethertype<0x0600u || ethertype==AION_ETHERTYPE_VLAN || ethertype==AION_ETHERTYPE_QINQ)return AION_ETHERNET_EINVAL;
    if(payload_length>AION_ETHERNET_MAX_FRAME-AION_ETHERNET_HEADER_SIZE || capacity<AION_ETHERNET_HEADER_SIZE || payload_length>capacity-AION_ETHERNET_HEADER_SIZE)return AION_ETHERNET_EINVAL;
    for(i=0;i<6;i++){p[i]=destination[i];p[i+6]=source[i];}
    p[12]=(uint8_t)(ethertype>>8);p[13]=(uint8_t)ethertype;
    for(i=0;i<payload_length;i++)p[14+i]=((const uint8_t *)payload)[i];
    *written=14+payload_length;
    return AION_ETHERNET_OK;
}
