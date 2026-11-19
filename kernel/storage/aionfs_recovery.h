#ifndef AION_FS_RECOVERY_H
#define AION_FS_RECOVERY_H
#include "aionfs_integrity.h"
#define AION_FS_RECOVERY_EUNRECOVERABLE (-74)
/* Restore a damaged primary from a verified, independently stored replica.
 * Caller owns and reserves all three distinct data-region blocks.
 * Replica checksum is associated with replica_block, not primary_block.
 */
int aion_fs_recover_block(const struct aion_fs *fs,uint64_t primary_block,uint64_t replica_block,uint64_t replica_checksum_block,void *buffer,void *scratch,size_t scratch_size);
#endif
