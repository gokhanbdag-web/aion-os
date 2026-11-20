#include "../kernel/network/tls_record.h"
#include <assert.h>
int main(void){
    uint8_t data[16]={22,3,3,0,4,1,2,3,4,23,3,3,0,2,5,6};
    uint8_t header[8]={0};struct aion_tls_record r;size_t consumed=0;
    assert(aion_tls_record_parse(data,sizeof(data),&r,&consumed)==AION_TLS_OK);
    assert(consumed==9 && r.content_type==22 && r.legacy_version==0x0303 && r.fragment_length==4 && r.fragment[3]==4);
    assert(aion_tls_record_parse(data+consumed,sizeof(data)-consumed,&r,&consumed)==AION_TLS_OK);
    assert(consumed==7 && r.content_type==23 && r.fragment_length==2);
    assert(aion_tls_record_parse(data,4,&r,&consumed)==AION_TLS_ECORRUPT);
    data[3]=0x7f;assert(aion_tls_record_parse(data,sizeof(data),&r,&consumed)==AION_TLS_ECORRUPT);data[3]=0;
    data[1]=2;assert(aion_tls_record_parse(data,sizeof(data),&r,&consumed)==AION_TLS_ECORRUPT);data[1]=3;
    assert(aion_tls_record_header_write(header,sizeof(header),22,0x0303,3)==AION_TLS_OK);
    assert(header[0]==22 && header[3]==0 && header[4]==3);
    assert(aion_tls_record_header_write(header,7,22,0x0303,3)==AION_TLS_EINVAL);
    assert(aion_tls_record_header_write(header,sizeof(header),99,0x0303,0)==AION_TLS_EINVAL);
    assert(aion_tls_record_parse(0,16,&r,&consumed)==AION_TLS_EINVAL);
    return 0;
}
