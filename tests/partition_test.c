#include "../kernel/storage/partition.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
static uint8_t sector[512];
static int read_sector(void *ctx,uint64_t first,uint32_t count,void *out) {
    (void)ctx;
    if(first!=0 || count!=1) return AION_BLOCK_EIO;
    memcpy(out,sector,512);
    return 0;
}
static void entry(unsigned i,uint8_t type,uint32_t start,uint32_t count) {
    uint8_t *p=sector+446+i*16;
    p[4]=type;
    p[8]=(uint8_t)start; p[9]=(uint8_t)(start>>8); p[10]=(uint8_t)(start>>16); p[11]=(uint8_t)(start>>24);
    p[12]=(uint8_t)count; p[13]=(uint8_t)(count>>8); p[14]=(uint8_t)(count>>16); p[15]=(uint8_t)(count>>24);
}
int main(void) {
    struct aion_block_device dev={512,1000,0,read_sector,0};
    struct aion_partition_table table;
    uint8_t scratch[512];
    memset(sector,0,sizeof(sector));
    assert(aion_partition_scan_mbr(&dev,&table,scratch,sizeof(scratch))==AION_PARTITION_EFORMAT);
    sector[510]=0x55; sector[511]=0xaa;
    entry(0,0x83,1,100); entry(1,0x0b,101,200);
    assert(aion_partition_scan_mbr(&dev,&table,scratch,sizeof(scratch))==0);
    assert(table.count==2 && table.entries[0].first_block==1 && table.entries[1].block_count==200);
    entry(1,0x0b,50,200);
    assert(aion_partition_scan_mbr(&dev,&table,scratch,sizeof(scratch))==AION_PARTITION_EFORMAT);
    entry(1,0x0b,990,20);
    assert(aion_partition_scan_mbr(&dev,&table,scratch,sizeof(scratch))==AION_PARTITION_EFORMAT);
    entry(1,0xee,101,200);
    assert(aion_partition_scan_mbr(&dev,&table,scratch,sizeof(scratch))==AION_PARTITION_EFORMAT);
    entry(1,0,0,0);
    sector[446]=0x81;
    assert(aion_partition_scan_mbr(&dev,&table,scratch,sizeof(scratch))==AION_PARTITION_EFORMAT);
    sector[446]=0;
    assert(aion_partition_scan_mbr(&dev,&table,scratch,511)==AION_BLOCK_EINVAL);
    assert(aion_partition_scan_mbr(&dev,0,scratch,512)==AION_BLOCK_EINVAL);
    return 0;
}
