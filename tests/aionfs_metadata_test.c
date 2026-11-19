#include "../kernel/storage/aionfs_metadata.h"
#include <assert.h>
#include <string.h>
static unsigned char disk[16][512];
static int rd(void *ctx,uint64_t first,uint32_t count,void *out){(void)ctx;if(first>=16 || count!=1)return AION_BLOCK_EIO;memcpy(out,disk[first],512);return 0;}
static int wr(void *ctx,uint64_t first,uint32_t count,const void *in){(void)ctx;if(first>=16 || count!=1)return AION_BLOCK_EIO;memcpy(disk[first],in,512);return 0;}
int main(void){
    struct aion_block_device dev={512,16,0,rd,wr};
    struct aion_fs fs;
    struct aion_fs_directory dir={0},loaded={0};
    struct aion_fs_entry entry;
    unsigned char scratch[512];
    assert(aion_fs_format(&dev,2,10,scratch,512)==0);
    assert(aion_fs_mount(&fs,&dev,2,10,scratch,512)==0);
    assert(aion_fs_metadata_add(&dir,"hello",1,1,2)==0);
    assert(aion_fs_metadata_save(&fs,0,&dir,scratch,512)==0);
    assert(aion_fs_metadata_load(&fs,0,&loaded,scratch,512)==0);
    assert(loaded.count==1);
    assert(aion_fs_metadata_find(&loaded,"hello",&entry)==0 && entry.block_count==2);
    assert(aion_fs_metadata_find(&loaded,"missing",&entry)==AION_FS_META_ENOENT);
    assert(aion_fs_metadata_add(&loaded,"hello",1,3,1)==AION_FS_ECORRUPT);
    assert(aion_fs_metadata_add(&loaded,"overlap",1,2,2)==0);
    assert(aion_fs_metadata_save(&fs,0,&loaded,scratch,512)==AION_FS_ECORRUPT);
    loaded.count=1;
    assert(aion_fs_metadata_add(&loaded,"meta",1,0,1)==0);
    assert(aion_fs_metadata_save(&fs,0,&loaded,scratch,512)==AION_FS_ECORRUPT);
    assert(aion_fs_metadata_load(&fs,9,&loaded,scratch,512)==AION_BLOCK_ERANGE);
    return 0;
}
