#include "ip_transport.h"
static uint16_t read16(const uint8_t *p){return (uint16_t)(((uint16_t)p[0]<<8)|p[1]);}
static uint32_t read32(const uint8_t *p){return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];}
static int ipv4_checksum_valid(const uint8_t *p,size_t length){
    uint32_t sum=0;size_t i;
    for(i=0;i<length;i+=2)sum+=read16(p+i);
    while(sum>>16)sum=(sum&0xffffu)+(sum>>16);
    return sum==0xffffu;
}
int aion_ipv4_parse(const void *data,size_t length,struct aion_ipv4_packet *out){
    const uint8_t *p=(const uint8_t *)data;size_t header,total;uint16_t fragments;unsigned i;
    if(!p || !out)return AION_NET_EINVAL;
    if(length<20 || (p[0]>>4)!=4 || (p[0]&15u)<5)return AION_NET_ECORRUPT;
    header=(size_t)(p[0]&15u)*4u;
    if(length<header)return AION_NET_ECORRUPT;
    total=read16(p+2);
    if(total<header || total>length || !ipv4_checksum_valid(p,header))return AION_NET_ECORRUPT;
    fragments=read16(p+6);
    if((fragments&0x8000u)!=0)return AION_NET_ECORRUPT;
    out->identification=read16(p+4);
    out->fragment_offset=(uint16_t)(fragments&0x1fffu);
    out->more_fragments=(uint8_t)((fragments&0x2000u)!=0);
    out->ttl=p[8];out->protocol=p[9];
    for(i=0;i<4;i++){out->source[i]=p[12+i];out->destination[i]=p[16+i];}
    out->payload=p+header;out->payload_length=total-header;
    return AION_NET_OK;
}
int aion_udp_parse(const void *data,size_t length,struct aion_udp_segment *out){
    const uint8_t *p=(const uint8_t *)data;size_t declared;
    if(!p || !out)return AION_NET_EINVAL;
    if(length<8)return AION_NET_ECORRUPT;
    declared=read16(p+4);
    if(declared<8 || declared>length)return AION_NET_ECORRUPT;
    out->source_port=read16(p);out->destination_port=read16(p+2);
    out->checksum=read16(p+6);out->payload=p+8;out->payload_length=declared-8;
    return AION_NET_OK;
}
int aion_tcp_parse(const void *data,size_t length,struct aion_tcp_segment *out){
    const uint8_t *p=(const uint8_t *)data;size_t header;
    if(!p || !out)return AION_NET_EINVAL;
    if(length<20)return AION_NET_ECORRUPT;
    header=(size_t)(p[12]>>4)*4u;
    if(header<20 || header>length || (p[12]&1u)!=0)return AION_NET_ECORRUPT;
    out->source_port=read16(p);out->destination_port=read16(p+2);
    out->sequence=read32(p+4);out->acknowledgment=read32(p+8);
    out->flags=p[13];out->window=read16(p+14);out->checksum=read16(p+16);
    out->payload=p+header;out->payload_length=length-header;
    return AION_NET_OK;
}
