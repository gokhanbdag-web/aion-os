#ifndef AION_FS_H
#define AION_FS_H
#include "block.h"
#include <stdint.h>
#define AION_FS_MAGIC 0x53464e41u
#define AION_FS_VERSION 1u
#define AION_FS_EFORMAT (-84)
#define AION_FS_ECORRUPT (-74)
struct aion_fs {
    const struct aion_block_device *device;
    uint64_t first_block;
    uint64_t block_count;
    uint64_t data_start;
};
int aion_fs_format(const struct aion_block_device *device,uint64_t first,uint64_t count,void *scratch,size_t scratch_size);
int aion_fs_mount(struct aion_fs *fs,const struct aion_block_device *device,uint64_t first,uint64_t count,void *scratch,size_t scratch_size);
int aion_fs_read_data(const struct aion_fs *fs,uint64_t index,void *buffer);
int aion_fs_write_data(const struct aion_fs *fs,uint64_t index,const void *buffer);
#endif
