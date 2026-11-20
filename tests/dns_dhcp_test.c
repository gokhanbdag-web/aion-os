#include "../kernel/network/dns_dhcp.h"
#include <assert.h>
int main(void){
    uint8_t dns[]={0x12,0x34,0x81,0x80,0,1,0,1,0,0,0,0,3,'w','w','w',7,'e','x','a','m','p','l','e',3,'c','o','m',0,0xc0,12};
    uint8_t dhcp[260]={0};struct aion_dns_header h;struct aion_dhcp_offer offer;
    char name[256];size_t next;size_t i;
    assert(aion_dns_parse_header(dns,sizeof(dns),&h)==0 && h.id==0x1234 && h.answers==1);
    assert(aion_dns_read_name(dns,sizeof(dns),12,name,sizeof(name),&next)==0 && next==29);
    assert(name[0]=='w' && name[3]=='.');
    assert(aion_dns_read_name(dns,sizeof(dns),29,name,sizeof(name),&next)==0 && next==31);
    dns[29]=0xc0;dns[30]=29;assert(aion_dns_read_name(dns,sizeof(dns),29,name,sizeof(name),&next)==AION_DNS_DHCP_ECORRUPT);
    dhcp[0]=2;dhcp[1]=1;dhcp[2]=6;dhcp[4]=1;dhcp[16]=192;dhcp[17]=0;dhcp[18]=2;dhcp[19]=5;
    dhcp[236]=99;dhcp[237]=130;dhcp[238]=83;dhcp[239]=99;
    i=240;dhcp[i++]=53;dhcp[i++]=1;dhcp[i++]=2;
    dhcp[i++]=1;dhcp[i++]=4;dhcp[i++]=255;dhcp[i++]=255;dhcp[i++]=255;dhcp[i++]=0;
    dhcp[i++]=255;
    assert(aion_dhcp_parse_reply(dhcp,i,&offer)==0 && offer.message_type==2 && offer.offered_ip[0]==192 && offer.has_subnet_mask);
    dhcp[236]=0;assert(aion_dhcp_parse_reply(dhcp,i,&offer)==AION_DNS_DHCP_ECORRUPT);
    assert(aion_dns_parse_header(0,12,&h)==AION_DNS_DHCP_EINVAL);
    return 0;
}
