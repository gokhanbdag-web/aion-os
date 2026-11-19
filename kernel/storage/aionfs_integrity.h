#ifndef AION_FS_INTEGRITY_H
#define AION_FS_INTEGRITY_H
#include "aionfs.h"
#include <stddef.h>
#include <stdint.h>
#define AION_FS_INTEGRITY_MAGIC 0x47455449u
#define AION_FS_INTEGRITY_VERSION 1u
#define AION_FS_INTEGRITY_ECHECKSUM (-74)
uint32_t aion_fs_crc32(const void *buffer,size_t length);
int aion_fs_integrity_write(const struct aion_fs *fs,uint64_t data_block,uint64_t checksum_block,const void *data,void *scratch,size_t scratch_size);
int aion_fs_integrity_read(const struct aion_fs *fs,uint64_t data_block,uint64_t checksum_block,void *data,void *scratch,size_t scratch_size);
#endif
