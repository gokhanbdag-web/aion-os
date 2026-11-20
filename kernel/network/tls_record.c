#include "tls_record.h"
static int valid_type(uint8_t type){
    return type==AION_TLS_TYPE_CHANGE_CIPHER_SPEC || type==AION_TLS_TYPE_ALERT ||
           type==AION_TLS_TYPE_HANDSHAKE || type==AION_TLS_TYPE_APPLICATION_DATA;
}
int aion_tls_record_parse(const void *data,size_t length,struct aion_tls_record *out,size_t *consumed){
    const uint8_t *p=(const uint8_t *)data;size_t payload;
    if(!p || !out || !consumed)return AION_TLS_EINVAL;
    if(length<AION_TLS_HEADER_SIZE)return AION_TLS_ECORRUPT;
    if(!valid_type(p[0]) || p[1]!=3 || p[2]>3)return AION_TLS_ECORRUPT;
    payload=((size_t)p[3]<<8)|p[4];
    if(payload>AION_TLS_MAX_CIPHERTEXT || payload>length-AION_TLS_HEADER_SIZE)return AION_TLS_ECORRUPT;
    out->content_type=p[0];out->legacy_version=(uint16_t)(((uint16_t)p[1]<<8)|p[2]);
    out->fragment=p+AION_TLS_HEADER_SIZE;out->fragment_length=payload;
    *consumed=AION_TLS_HEADER_SIZE+payload;
    return AION_TLS_OK;
}
int aion_tls_record_header_write(void *data,size_t capacity,uint8_t type,uint16_t legacy_version,size_t fragment_length){
    uint8_t *p=(uint8_t *)data;
    if(!p)return AION_TLS_EINVAL;
    if(capacity<AION_TLS_HEADER_SIZE || !valid_type(type) || (legacy_version>>8)!=3u ||
       (legacy_version&255u)>3u || fragment_length>AION_TLS_MAX_CIPHERTEXT ||
       fragment_length>capacity-AION_TLS_HEADER_SIZE)return AION_TLS_EINVAL;
    p[0]=type;p[1]=(uint8_t)(legacy_version>>8);p[2]=(uint8_t)legacy_version;
    p[3]=(uint8_t)(fragment_length>>8);p[4]=(uint8_t)fragment_length;
    return AION_TLS_OK;
}
