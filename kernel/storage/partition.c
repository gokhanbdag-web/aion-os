#include "partition.h"
static uint32_t read_le32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24);
}
int aion_partition_scan_mbr(const struct aion_block_device *device,struct aion_partition_table *table,void *scratch,size_t scratch_size) {
    const uint8_t *sector;
    uint32_t i;
    int rc;
    if (!table || !scratch || aion_block_validate(device)!=AION_BLOCK_OK || device->block_size<512u || scratch_size<device->block_size) return AION_BLOCK_EINVAL;
    table->count=0;
    for(i=0;i<AION_PARTITION_MAX;i++) {
        table->entries[i].type=0; table->entries[i].first_block=0; table->entries[i].block_count=0;
    }
    rc=aion_block_read(device,0,1,scratch);
    if (rc!=AION_BLOCK_OK) return rc;
    sector=(const uint8_t *)scratch;
    if (sector[510]!=0x55 || sector[511]!=0xaa) return AION_PARTITION_EFORMAT;
    for(i=0;i<AION_PARTITION_MAX;i++) {
        const uint8_t *p=sector+446u+i*16u;
        uint8_t boot=p[0],type=p[4];
        uint64_t start=read_le32(p+8),count=read_le32(p+12);
        uint32_t j;
        if (boot!=0 && boot!=0x80) return AION_PARTITION_EFORMAT;
        if (!type && !start && !count) continue;
        if (!type || !count || !start || start>=device->block_count || count>device->block_count-start) return AION_PARTITION_EFORMAT;
        /* Extended partitions and protective GPT require separate parsers. */
        if (type==0xee || type==0x05 || type==0x0f || type==0x85) return AION_PARTITION_EFORMAT;
        for(j=0;j<table->count;j++) {
            uint64_t a=table->entries[j].first_block,b=a+table->entries[j].block_count;
            if (start<b && a<start+count) return AION_PARTITION_EFORMAT;
        }
        table->entries[table->count].type=type;
        table->entries[table->count].first_block=start;
        table->entries[table->count].block_count=count;
        table->count++;
    }
    return AION_BLOCK_OK;
}
