#ifndef AION_PARTITION_H
#define AION_PARTITION_H
#include "block.h"
#include <stdint.h>
#define AION_PARTITION_MAX 4u
#define AION_PARTITION_EFORMAT (-84)
struct aion_partition {
    uint8_t type;
    uint64_t first_block;
    uint64_t block_count;
};
struct aion_partition_table {
    struct aion_partition entries[AION_PARTITION_MAX];
    uint32_t count;
};
int aion_partition_scan_mbr(const struct aion_block_device *device,struct aion_partition_table *table,void *scratch,size_t scratch_size);
#endif
