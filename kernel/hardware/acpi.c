#include "acpi.h"
static uint32_t le32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint64_t le64(const uint8_t *p){return (uint64_t)le32(p)|((uint64_t)le32(p+4)<<32);}
static int checksum(const uint8_t *p,size_t n){uint8_t sum=0;size_t i;for(i=0;i<n;i++)sum=(uint8_t)(sum+p[i]);return sum==0;}
int aion_acpi_parse_rsdp(const void *buffer,size_t length,struct aion_acpi_rsdp *out){
    const uint8_t *p=(const uint8_t *)buffer;
    static const uint8_t signature[8]={'R','S','D',' ','P','T','R',' '};
    size_t i;uint32_t extended_length;
    if(!p || !out || length<20)return AION_ACPI_EINVAL;
    for(i=0;i<8;i++)if(p[i]!=signature[i])return AION_ACPI_ECORRUPT;
    if(!checksum(p,20))return AION_ACPI_ECORRUPT;
    out->revision=p[15];out->use_xsdt=0;out->root_address=le32(p+16);
    if(p[15]>=2){
        if(length<36)return AION_ACPI_EINVAL;
        extended_length=le32(p+20);
        if(extended_length<36 || extended_length>length || !checksum(p,extended_length))return AION_ACPI_ECORRUPT;
        if(le64(p+24)!=0){out->use_xsdt=1;out->root_address=le64(p+24);}
    }
    if(!out->root_address)return AION_ACPI_ENOENT;
    return AION_ACPI_OK;
}
int aion_acpi_parse_sdt(const void *buffer,size_t length,struct aion_acpi_sdt *out){
    const uint8_t *p=(const uint8_t *)buffer;uint32_t declared;unsigned i;
    if(!p || !out || length<36)return AION_ACPI_EINVAL;
    declared=le32(p+4);
    if(declared<36 || declared>length || !checksum(p,declared))return AION_ACPI_ECORRUPT;
    for(i=0;i<4;i++)out->signature[i]=p[i];
    out->length=declared;out->revision=p[8];
    return AION_ACPI_OK;
}
