#include "aionfs_recovery.h"
int aion_fs_recover_block(const struct aion_fs *fs,uint64_t primary,uint64_t replica,uint64_t checksum,void *buffer,void *scratch,size_t size){
    int rc;
    if(!fs || !fs->device || !buffer || !scratch || buffer==scratch || primary==replica || primary==checksum || replica==checksum)return AION_BLOCK_EINVAL;
    if(fs->data_start>fs->block_count)return AION_BLOCK_EINVAL;
    if(primary>=fs->block_count-fs->data_start || replica>=fs->block_count-fs->data_start || checksum>=fs->block_count-fs->data_start)return AION_BLOCK_ERANGE;
    if(size<fs->device->block_size)return AION_BLOCK_EINVAL;
    /* Never overwrite the primary unless the replica has passed checksum verification. */
    rc=aion_fs_integrity_read(fs,replica,checksum,buffer,scratch,size);
    if(rc)return rc;
    return aion_fs_write_data(fs,primary,buffer);
}
