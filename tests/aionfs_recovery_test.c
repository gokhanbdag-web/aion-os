#include "../kernel/storage/aionfs_recovery.h"
#include <assert.h>
#include <string.h>
static unsigned char disk[16][512];
static int rd(void *ctx,uint64_t first,uint32_t count,void *out){(void)ctx;if(first>=16 || count!=1)return AION_BLOCK_EIO;memcpy(out,disk[first],512);return 0;}
static int wr(void *ctx,uint64_t first,uint32_t count,const void *in){(void)ctx;if(first>=16 || count!=1)return AION_BLOCK_EIO;memcpy(disk[first],in,512);return 0;}
int main(void){
    struct aion_block_device dev={512,16,0,rd,wr};struct aion_fs fs;
    unsigned char data[512],buffer[512],scratch[512],out[512];
    memset(data,0x5a,512);
    assert(aion_fs_format(&dev,2,10,scratch,512)==0);
    assert(aion_fs_mount(&fs,&dev,2,10,scratch,512)==0);
    assert(aion_fs_integrity_write(&fs,2,3,data,scratch,512)==0);
    assert(aion_fs_write_data(&fs,1,data)==0);
    disk[4][0]^=1; /* primary index 1 is LBA 4 */
    assert(aion_fs_recover_block(&fs,1,2,3,buffer,scratch,512)==0);
    assert(aion_fs_read_data(&fs,1,out)==0 && memcmp(out,data,512)==0);
    disk[5][0]^=1; /* corrupt replica */
    disk[4][0]=0xa5;
    assert(aion_fs_recover_block(&fs,1,2,3,buffer,scratch,512)==AION_FS_INTEGRITY_ECHECKSUM);
    assert(disk[4][0]==0xa5); /* never overwrite with corrupt replica */
    assert(aion_fs_recover_block(&fs,1,1,3,buffer,scratch,512)==AION_BLOCK_EINVAL);
    assert(aion_fs_recover_block(&fs,1,2,1,buffer,scratch,512)==AION_BLOCK_EINVAL);
    assert(aion_fs_recover_block(&fs,9,2,3,buffer,scratch,512)==AION_BLOCK_ERANGE);
    return 0;
}
