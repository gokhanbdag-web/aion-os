#include "../kernel/network/ip_transport.h"
#include <assert.h>
int main(void){
    uint8_t ip[32]={0x45,0,0,32,0,1,0,0,64,17,0,0,192,0,2,1,198,51,100,2,0x12,0x34,0,53,0,12,0,0,1,2,3,4};
    uint8_t tcp[24]={0,80,0x12,0x34,0,0,0,1,0,0,0,2,0x50,0x12,0,64,0,0,0,0,9,8,7,6};
    struct aion_ipv4_packet v4;struct aion_udp_segment udp;struct aion_tcp_segment t;
    unsigned sum=0;size_t i;
    for(i=0;i<20;i+=2)sum+=((unsigned)ip[i]<<8)|ip[i+1];
    while(sum>>16)sum=(sum&65535u)+(sum>>16);
    ip[10]=(uint8_t)((~sum)>>8);ip[11]=(uint8_t)(~sum);
    assert(aion_ipv4_parse(ip,sizeof(ip),&v4)==0);
    assert(v4.protocol==17 && v4.payload_length==12 && v4.source[0]==192);
    assert(aion_udp_parse(v4.payload,v4.payload_length,&udp)==0);
    assert(udp.source_port==0x1234 && udp.destination_port==53 && udp.payload_length==4 && udp.payload[3]==4);
    assert(aion_ipv4_parse(ip,31,&v4)==AION_NET_ECORRUPT);
    ip[8]=63;assert(aion_ipv4_parse(ip,32,&v4)==AION_NET_ECORRUPT);ip[8]=64;
    assert(aion_tcp_parse(tcp,sizeof(tcp),&t)==0 && t.flags==0x12 && t.sequence==1 && t.payload_length==4);
    tcp[12]=0x40;assert(aion_tcp_parse(tcp,sizeof(tcp),&t)==AION_NET_ECORRUPT);
    assert(aion_udp_parse(ip+20,7,&udp)==AION_NET_ECORRUPT);
    assert(aion_tcp_parse(0,24,&t)==AION_NET_EINVAL);
    return 0;
}
