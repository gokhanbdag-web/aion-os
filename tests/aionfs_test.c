#include "../kernel/storage/aionfs.h"
#include <assert.h>
#include <string.h>
static unsigned char disk[16][512];
static int rd(void *ctx,uint64_t first,uint32_t count,void *out) {
    (void)ctx;if(first>=16 || count!=1)return AION_BLOCK_EIO;
    memcpy(out,disk[first],512);return 0;
}
static int wr(void *ctx,uint64_t first,uint32_t count,const void *in) {
    (void)ctx;if(first>=16 || count!=1)return AION_BLOCK_EIO;
    memcpy(disk[first],in,512);return 0;
}
int main(void) {
    struct aion_block_device dev={512,16,0,rd,wr};
    struct aion_fs fs;
    unsigned char scratch[512],data[512],out[512];
    memset(disk,0,sizeof(disk));memset(data,0x5a,sizeof(data));
    assert(aion_fs_mount(&fs,&dev,2,10,scratch,sizeof(scratch))==AION_FS_EFORMAT);
    assert(aion_fs_format(&dev,2,10,scratch,sizeof(scratch))==0);
    assert(aion_fs_mount(&fs,&dev,2,10,scratch,sizeof(scratch))==0);
    assert(aion_fs_write_data(&fs,0,data)==0);
    assert(aion_fs_read_data(&fs,0,out)==0 && memcmp(data,out,512)==0);
    assert(aion_fs_write_data(&fs,8,data)==0);
    assert(aion_fs_read_data(&fs,9,out)==AION_BLOCK_ERANGE);
    assert(aion_fs_mount(&fs,&dev,2,11,scratch,sizeof(scratch))==AION_FS_ECORRUPT);
    assert(aion_fs_format(&dev,15,10,scratch,sizeof(scratch))==AION_BLOCK_EINVAL);
    assert(aion_fs_format(&dev,2,10,scratch,511)==AION_BLOCK_EINVAL);
    return 0;
}
