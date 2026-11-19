#ifndef AION_BLOCK_H
#define AION_BLOCK_H
#include <stddef.h>
#include <stdint.h>
#define AION_BLOCK_OK 0
#define AION_BLOCK_EINVAL (-22)
#define AION_BLOCK_EIO (-5)
#define AION_BLOCK_ERANGE (-34)
#define AION_BLOCK_EROFS (-30)
struct aion_block_device;
typedef int (*aion_block_read_fn)(void *context,uint64_t first_block,uint32_t count,void *buffer);
typedef int (*aion_block_write_fn)(void *context,uint64_t first_block,uint32_t count,const void *buffer);
struct aion_block_device {
    uint32_t block_size;
    uint64_t block_count;
    void *context;
    aion_block_read_fn read;
    aion_block_write_fn write;
};
int aion_block_validate(const struct aion_block_device *device);
int aion_block_read(const struct aion_block_device *device,uint64_t first_block,uint32_t count,void *buffer);
int aion_block_write(const struct aion_block_device *device,uint64_t first_block,uint32_t count,const void *buffer);
#endif
