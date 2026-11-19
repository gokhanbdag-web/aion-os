#include "../kernel/storage/block.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
static uint8_t disk[8][512];
static int read_blocks(void *ctx,uint64_t first,uint32_t count,void *out) {
    (void)ctx;
    memcpy(out,&disk[first][0],(size_t)count*512u);
    return 0;
}
static int write_blocks(void *ctx,uint64_t first,uint32_t count,const void *in) {
    (void)ctx;
    memcpy(&disk[first][0],in,(size_t)count*512u);
    return 0;
}
int main(void) {
    struct aion_block_device device={512,8,0,read_blocks,write_blocks};
    uint8_t input[1024],output[1024];
    size_t i;
    for(i=0;i<sizeof(input);i++) input[i]=(uint8_t)i;
    assert(aion_block_validate(&device)==0);
    assert(aion_block_write(&device,2,2,input)==0);
    assert(aion_block_read(&device,2,2,output)==0);
    assert(memcmp(input,output,sizeof(input))==0);
    assert(aion_block_read(&device,7,2,output)==AION_BLOCK_ERANGE);
    assert(aion_block_read(&device,UINT64_MAX,1,output)==AION_BLOCK_ERANGE);
    assert(aion_block_read(&device,0,0,output)==AION_BLOCK_EINVAL);
    assert(aion_block_read(&device,0,1,0)==AION_BLOCK_EINVAL);
    device.write=0;
    assert(aion_block_write(&device,0,1,input)==AION_BLOCK_EROFS);
    device.block_size=3;
    assert(aion_block_validate(&device)==AION_BLOCK_EINVAL);
    device.block_size=512;
    device.read=0;
    assert(aion_block_validate(&device)==AION_BLOCK_EINVAL);
    return 0;
}
