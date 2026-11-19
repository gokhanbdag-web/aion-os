#include "aionfs_integrity.h"
static void put32(uint8_t *p,uint32_t n){unsigned i;for(i=0;i<4;i++)p[i]=(uint8_t)(n>>(i*8));}
static void put64(uint8_t *p,uint64_t n){unsigned i;for(i=0;i<8;i++)p[i]=(uint8_t)(n>>(i*8));}
static uint32_t get32(const uint8_t *p){uint32_t n=0;int i;for(i=3;i>=0;i--)n=(n<<8)|p[i];return n;}
static uint64_t get64(const uint8_t *p){uint64_t n=0;int i;for(i=7;i>=0;i--)n=(n<<8)|p[i];return n;}
uint32_t aion_fs_crc32(const void *buffer,size_t length){
    const uint8_t *p=(const uint8_t *)buffer;uint32_t crc=0xffffffffu;size_t i;unsigned bit;
    if(!p)return 0;
    for(i=0;i<length;i++){crc^=p[i];for(bit=0;bit<8;bit++)crc=(crc>>1)^((crc&1u)?0xedb88320u:0u);}
    return ~crc;
}
static int validate(const struct aion_fs *fs,uint64_t data,uint64_t checksum,const void *buf,const void *scratch,size_t size){
    uint64_t available;
    if(!fs || !fs->device || !buf || !scratch || fs->block_count<=fs->data_start || fs->device->block_size<512 || size<fs->device->block_size)return AION_BLOCK_EINVAL;
    available=fs->block_count-fs->data_start;
    if(data>=available || checksum>=available)return AION_BLOCK_ERANGE;
    if(data==checksum || buf==scratch)return AION_BLOCK_EINVAL;
    return 0;
}
int aion_fs_integrity_write(const struct aion_fs *fs,uint64_t data_block,uint64_t checksum_block,const void *data,void *scratch,size_t size){
    uint8_t *p=(uint8_t *)scratch;uint32_t i,crc;int rc=validate(fs,data_block,checksum_block,data,scratch,size);
    if(rc)return rc;
    crc=aion_fs_crc32(data,fs->device->block_size);
    rc=aion_fs_write_data(fs,data_block,data);if(rc)return rc;
    for(i=0;i<fs->device->block_size;i++)p[i]=0;
    put32(p,AION_FS_INTEGRITY_MAGIC);put32(p+4,AION_FS_INTEGRITY_VERSION);put64(p+8,data_block);put32(p+16,crc);
    return aion_fs_write_data(fs,checksum_block,p);
}
int aion_fs_integrity_read(const struct aion_fs *fs,uint64_t data_block,uint64_t checksum_block,void *data,void *scratch,size_t size){
    const uint8_t *p=(const uint8_t *)scratch;int rc=validate(fs,data_block,checksum_block,data,scratch,size);
    if(rc)return rc;
    rc=aion_fs_read_data(fs,checksum_block,scratch);if(rc)return rc;
    if(get32(p)!=AION_FS_INTEGRITY_MAGIC || get32(p+4)!=AION_FS_INTEGRITY_VERSION || get64(p+8)!=data_block)return AION_FS_INTEGRITY_ECHECKSUM;
    {uint32_t expected=get32(p+16);
      rc=aion_fs_read_data(fs,data_block,data);if(rc)return rc;
      if(aion_fs_crc32(data,fs->device->block_size)!=expected)return AION_FS_INTEGRITY_ECHECKSUM;
    }
    return 0;
}
