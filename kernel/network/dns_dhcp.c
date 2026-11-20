#include "dns_dhcp.h"
static uint16_t u16(const uint8_t *p){return (uint16_t)(((uint16_t)p[0]<<8)|p[1]);}
static uint32_t u32(const uint8_t *p){return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];}
int aion_dns_parse_header(const void *data,size_t length,struct aion_dns_header *out){
    const uint8_t *p=(const uint8_t *)data;
    if(!p || !out)return AION_DNS_DHCP_EINVAL;
    if(length<12)return AION_DNS_DHCP_ECORRUPT;
    out->id=u16(p);out->flags=u16(p+2);out->questions=u16(p+4);
    out->answers=u16(p+6);out->authorities=u16(p+8);out->additionals=u16(p+10);
    return AION_DNS_DHCP_OK;
}
int aion_dns_read_name(const void *data,size_t length,size_t offset,char *name,size_t capacity,size_t *next_offset){
    const uint8_t *p=(const uint8_t *)data;size_t pos=offset,out=0,consumed=offset,hops=0;int jumped=0;
    if(!p || !name || !next_offset || !capacity)return AION_DNS_DHCP_EINVAL;
    if(offset>=length)return AION_DNS_DHCP_ECORRUPT;
    for(;;){
        uint8_t label;size_t i;
        if(pos>=length || ++hops>length)return AION_DNS_DHCP_ECORRUPT;
        label=p[pos++];
        if((label&0xc0u)==0xc0u){
            size_t target;
            if(pos>=length)return AION_DNS_DHCP_ECORRUPT;
            target=(size_t)(((unsigned)(label&0x3fu)<<8)|p[pos++]);
            if(!jumped){consumed=pos;jumped=1;}
            if(target>=length)return AION_DNS_DHCP_ECORRUPT;
            pos=target;continue;
        }
        if((label&0xc0u)!=0 || label>63)return AION_DNS_DHCP_ECORRUPT;
        if(!label)break;
        if((size_t)label>length-pos || out+(size_t)label+1>capacity || out+(size_t)label+1>AION_DNS_MAX_NAME)return AION_DNS_DHCP_ECORRUPT;
        if(out)name[out++]='.';
        for(i=0;i<label;i++){uint8_t c=p[pos++];if(c<33 || c>126)return AION_DNS_DHCP_ECORRUPT;name[out++]=(char)c;}
    }
    if(out>=capacity)return AION_DNS_DHCP_ECORRUPT;
    name[out]=0;*next_offset=jumped?consumed:pos;
    return AION_DNS_DHCP_OK;
}
int aion_dhcp_parse_reply(const void *data,size_t length,struct aion_dhcp_offer *out){
    const uint8_t *p=(const uint8_t *)data;size_t i;
    if(!p || !out)return AION_DNS_DHCP_EINVAL;
    if(length<240 || p[0]!=2 || p[1]!=1 || p[2]!=6 || p[236]!=99 || p[237]!=130 || p[238]!=83 || p[239]!=99)return AION_DNS_DHCP_ECORRUPT;
    out->transaction_id=u32(p+4);out->message_type=0;
    out->has_subnet_mask=0;out->has_router=0;out->has_dns_server=0;out->has_server_identifier=0;
    for(i=0;i<4;i++){out->offered_ip[i]=p[16+i];out->server_ip[i]=p[20+i];}
    for(i=0;i<6;i++)out->client_mac[i]=p[28+i];
    i=240;
    while(i<length){
        uint8_t code=p[i++],size;const uint8_t *value;size_t j;
        if(code==255)break;
        if(code==0)continue;
        if(i>=length)return AION_DNS_DHCP_ECORRUPT;
        size=p[i++];
        if((size_t)size>length-i)return AION_DNS_DHCP_ECORRUPT;
        value=p+i;
        if(code==53 && size==1)out->message_type=value[0];
        if(code==1 && size==4){for(j=0;j<4;j++)out->subnet_mask[j]=value[j];out->has_subnet_mask=1;}
        if(code==3 && size>=4 && size%4==0){for(j=0;j<4;j++)out->router[j]=value[j];out->has_router=1;}
        if(code==6 && size>=4 && size%4==0){for(j=0;j<4;j++)out->dns_server[j]=value[j];out->has_dns_server=1;}
        if(code==54 && size==4){for(j=0;j<4;j++)out->server_identifier[j]=value[j];out->has_server_identifier=1;}
        i+=(size_t)size;
    }
    if(out->message_type!=2 && out->message_type!=5)return AION_DNS_DHCP_ECORRUPT;
    return AION_DNS_DHCP_OK;
}
