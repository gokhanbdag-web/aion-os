#ifndef AION_FS_METADATA_H
#define AION_FS_METADATA_H
#include "aionfs.h"
#define AION_FS_META_MAGIC 0x4154454du
#define AION_FS_META_MAX 8u
#define AION_FS_NAME_MAX 24u
#define AION_FS_META_ENOENT (-2)
#define AION_FS_META_ENOSPC (-28)
struct aion_fs_entry {
    char name[AION_FS_NAME_MAX];
    uint64_t first_data_block;
    uint64_t block_count;
    uint8_t kind;
};
struct aion_fs_directory {
    uint32_t count;
    struct aion_fs_entry entries[AION_FS_META_MAX];
};
int aion_fs_metadata_load(const struct aion_fs *fs,uint64_t metadata_block,struct aion_fs_directory *directory,void *scratch,size_t size);
int aion_fs_metadata_save(const struct aion_fs *fs,uint64_t metadata_block,const struct aion_fs_directory *directory,void *scratch,size_t size);
int aion_fs_metadata_add(struct aion_fs_directory *directory,const char *name,uint8_t kind,uint64_t first,uint64_t count);
int aion_fs_metadata_find(const struct aion_fs_directory *directory,const char *name,struct aion_fs_entry *entry);
#endif
