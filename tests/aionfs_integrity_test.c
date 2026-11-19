#include "../kernel/storage/aionfs_integrity.h"
#include <assert.h>
#include <string.h>
static unsigned char disk[16][512];
static int rd(void *ctx,uint64_t first,uint32_t count,void *out){(void)ctx;if(first>=16 || count!=1)return AION_BLOCK_EIO;memcpy(out,disk[first],512);return 0;}
static int wr(void *ctx,uint64_t first,uint32_t count,const void *in){(void)ctx;if(first>=16 || count!=1)return AION_BLOCK_EIO;memcpy(disk[first],in,512);return 0;}
int main(void){
    struct aion_block_device dev={512,16,0,rd,wr};
    struct aion_fs fs;unsigned char data[512],out[512],scratch[512];
    assert(aion_fs_crc32("123456789",9)==0xcbf43926u);
    memset(data,0x5a,512);
    assert(aion_fs_format(&dev,2,10,scratch,512)==0);
    assert(aion_fs_mount(&fs,&dev,2,10,scratch,512)==0);
    assert(aion_fs_integrity_write(&fs,1,2,data,scratch,512)==0);
    assert(aion_fs_integrity_read(&fs,1,2,out,scratch,512)==0);
    assert(memcmp(data,out,512)==0);
    disk[4][0]^=1;
    assert(aion_fs_integrity_read(&fs,1,2,out,scratch,512)==AION_FS_INTEGRITY_ECHECKSUM);
    disk[4][0]^=1;
    disk[5][0]^=1;
    assert(aion_fs_integrity_read(&fs,1,2,out,scratch,512)==AION_FS_INTEGRITY_ECHECKSUM);
    assert(aion_fs_integrity_write(&fs,1,1,data,scratch,512)==AION_BLOCK_EINVAL);
    assert(aion_fs_integrity_write(&fs,9,2,data,scratch,512)==AION_BLOCK_ERANGE);
    assert(aion_fs_integrity_write(&fs,1,2,data,scratch,511)==AION_BLOCK_EINVAL);
    return 0;
}
