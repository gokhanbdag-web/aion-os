#include "aionfs.h"
#include <stddef.h>
static void put32(uint8_t *p,uint32_t n) { unsigned i;for(i=0;i<4;i++)p[i]=(uint8_t)(n>>(8*i)); }
static void put64(uint8_t *p,uint64_t n) { unsigned i;for(i=0;i<8;i++)p[i]=(uint8_t)(n>>(8*i)); }
static uint32_t get32(const uint8_t *p) { uint32_t n=0;int i;for(i=3;i>=0;i--)n=(n<<8)|p[i];return n; }
static uint64_t get64(const uint8_t *p) { uint64_t n=0;int i;for(i=7;i>=0;i--)n=(n<<8)|p[i];return n; }
static int bounds(const struct aion_block_device *d,uint64_t first,uint64_t count,void *scratch,size_t size) {
    if(aion_block_validate(d)!=0 || !scratch || d->block_size<512 || size<d->block_size || count<2 || first>=d->block_count || count>d->block_count-first) return AION_BLOCK_EINVAL;
    return 0;
}
int aion_fs_format(const struct aion_block_device *d,uint64_t first,uint64_t count,void *scratch,size_t size) {
    uint8_t *p=(uint8_t *)scratch;
    uint32_t i;
    int rc=bounds(d,first,count,scratch,size);
    if(rc) return rc;
    for(i=0;i<d->block_size;i++)p[i]=0;
    put32(p,AION_FS_MAGIC);put32(p+4,AION_FS_VERSION);put64(p+8,count);put64(p+16,1);
    return aion_block_write(d,first,1,p);
}
int aion_fs_mount(struct aion_fs *fs,const struct aion_block_device *d,uint64_t first,uint64_t count,void *scratch,size_t size) {
    const uint8_t *p=(const uint8_t *)scratch;
    int rc;
    if(!fs)return AION_BLOCK_EINVAL;
    fs->device=0;fs->first_block=0;fs->block_count=0;fs->data_start=0;
    rc=bounds(d,first,count,scratch,size);if(rc)return rc;
    rc=aion_block_read(d,first,1,scratch);if(rc)return rc;
    if(get32(p)!=AION_FS_MAGIC || get32(p+4)!=AION_FS_VERSION)return AION_FS_EFORMAT;
    if(get64(p+8)!=count || get64(p+16)!=1)return AION_FS_ECORRUPT;
    fs->device=d;fs->first_block=first;fs->block_count=count;fs->data_start=1;
    return 0;
}
static int data_lba(const struct aion_fs *fs,uint64_t index,uint64_t *lba) {
    if(!fs || !fs->device || !lba)return AION_BLOCK_EINVAL;
    if(index>=fs->block_count-fs->data_start)return AION_BLOCK_ERANGE;
    *lba=fs->first_block+fs->data_start+index;
    return 0;
}
int aion_fs_read_data(const struct aion_fs *fs,uint64_t index,void *buffer) {
    uint64_t lba;int rc=data_lba(fs,index,&lba);if(rc)return rc;
    return aion_block_read(fs->device,lba,1,buffer);
}
int aion_fs_write_data(const struct aion_fs *fs,uint64_t index,const void *buffer) {
    uint64_t lba;int rc=data_lba(fs,index,&lba);if(rc)return rc;
    return aion_block_write(fs->device,lba,1,buffer);
}
