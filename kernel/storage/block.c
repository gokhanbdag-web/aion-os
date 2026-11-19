#include "block.h"
int aion_block_validate(const struct aion_block_device *device) {
    if (!device || !device->block_size || !device->block_count || !device->read) return AION_BLOCK_EINVAL;
    if ((device->block_size & (device->block_size-1u)) != 0u) return AION_BLOCK_EINVAL;
    return AION_BLOCK_OK;
}
static int check_range(const struct aion_block_device *device,uint64_t first,uint32_t count,const void *buffer) {
    if (aion_block_validate(device)!=AION_BLOCK_OK || !buffer || !count) return AION_BLOCK_EINVAL;
    if (first>=device->block_count || (uint64_t)count>device->block_count-first) return AION_BLOCK_ERANGE;
    if ((uint64_t)count > (uint64_t)SIZE_MAX / device->block_size) return AION_BLOCK_ERANGE;
    return AION_BLOCK_OK;
}
int aion_block_read(const struct aion_block_device *device,uint64_t first_block,uint32_t count,void *buffer) {
    int rc=check_range(device,first_block,count,buffer);
    if (rc!=AION_BLOCK_OK) return rc;
    return device->read(device->context,first_block,count,buffer);
}
int aion_block_write(const struct aion_block_device *device,uint64_t first_block,uint32_t count,const void *buffer) {
    int rc=check_range(device,first_block,count,buffer);
    if (rc!=AION_BLOCK_OK) return rc;
    if (!device->write) return AION_BLOCK_EROFS;
    return device->write(device->context,first_block,count,buffer);
}
