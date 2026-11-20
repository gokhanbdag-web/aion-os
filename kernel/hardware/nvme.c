#include "nvme.h"
static uint16_t le16(const uint8_t *p){return (uint16_t)((uint16_t)p[0]|((uint16_t)p[1]<<8));}
static uint32_t le32(const uint8_t *p){return (uint32_t)le16(p)|((uint32_t)le16(p+2)<<16);}
static uint64_t le64(const uint8_t *p){return (uint64_t)le32(p)|((uint64_t)le32(p+4)<<32);}
int aion_nvme_parse_controller(const void *identify,size_t length,struct aion_nvme_controller *out){
    const uint8_t *p=(const uint8_t *)identify;unsigned i;
    if(!p || !out || length<AION_NVME_IDENTIFY_BYTES)return AION_NVME_EINVAL;
    out->vendor_id=le16(p);out->subsystem_vendor_id=le16(p+2);
    for(i=0;i<20;i++)out->serial[i]=p[4+i];
    for(i=0;i<40;i++)out->model[i]=p[24+i];
    for(i=0;i<8;i++)out->firmware[i]=p[64+i];
    out->namespace_count=le32(p+516);
    if(out->namespace_count==0)return AION_NVME_EUNSUPPORTED;
    return AION_NVME_OK;
}
int aion_nvme_parse_namespace(const void *identify,size_t length,struct aion_nvme_namespace *out){
    const uint8_t *p=(const uint8_t *)identify;
    uint8_t format,number,exponent;uint16_t metadata;
    if(!p || !out || length<AION_NVME_IDENTIFY_BYTES)return AION_NVME_EINVAL;
    out->block_count=le64(p);
    number=(uint8_t)((p[25]&0x0fu)+1u);
    format=(uint8_t)((p[26]&0x0fu)|((p[26]&0x60u)>>1));
    if(!out->block_count || format>=number || format>=16)return AION_NVME_EUNSUPPORTED;
    metadata=le16(p+128+4u*format);
    exponent=p[128+4u*format+2];
    if(metadata || exponent<9 || exponent>16)return AION_NVME_EUNSUPPORTED;
    out->block_size=(uint32_t)1u<<exponent;out->lba_format=format;
    return AION_NVME_OK;
}
