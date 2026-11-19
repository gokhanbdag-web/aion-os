#include "aionfs_metadata.h"
static void w32(uint8_t *p,uint32_t n){unsigned i;for(i=0;i<4;i++)p[i]=(uint8_t)(n>>(i*8));}
static void w64(uint8_t *p,uint64_t n){unsigned i;for(i=0;i<8;i++)p[i]=(uint8_t)(n>>(i*8));}
static uint32_t r32(const uint8_t *p){uint32_t n=0;int i;for(i=3;i>=0;i--)n=(n<<8)|p[i];return n;}
static uint64_t r64(const uint8_t *p){uint64_t n=0;int i;for(i=7;i>=0;i--)n=(n<<8)|p[i];return n;}
static int name_length(const char *s){unsigned i;if(!s)return -1;for(i=0;i<AION_FS_NAME_MAX;i++){if(!s[i])return (int)i;}return -1;}
static int same(const char *a,const char *b){unsigned i;for(i=0;i<AION_FS_NAME_MAX;i++){if(a[i]!=b[i])return 0;if(!a[i])return 1;}return 0;}
static int validate(const struct aion_fs *fs,uint64_t block,const struct aion_fs_directory *d){
    uint32_t i,j;
    uint64_t usable;
    if(!fs || !fs->device || !d || fs->block_count<=fs->data_start || fs->device->block_size<512 || d->count>AION_FS_META_MAX)return AION_BLOCK_EINVAL;
    usable=fs->block_count-fs->data_start;
    if(block>=usable)return AION_BLOCK_ERANGE;
    for(i=0;i<d->count;i++){
        const struct aion_fs_entry *e=&d->entries[i];
        if(name_length(e->name)<=0 || !e->kind || !e->block_count || e->first_data_block>=usable || e->block_count>usable-e->first_data_block)return AION_FS_ECORRUPT;
        if(e->first_data_block<=block && block-e->first_data_block<e->block_count)return AION_FS_ECORRUPT;
        for(j=0;j<i;j++){
            const struct aion_fs_entry *o=&d->entries[j];
            if(same(e->name,o->name))return AION_FS_ECORRUPT;
            if(e->first_data_block<o->first_data_block+o->block_count && o->first_data_block<e->first_data_block+e->block_count)return AION_FS_ECORRUPT;
        }
    }
    return 0;
}
int aion_fs_metadata_add(struct aion_fs_directory *d,const char *name,uint8_t kind,uint64_t first,uint64_t count){
    unsigned i,j;int n=name_length(name);
    if(!d || n<=0 || !kind || !count)return AION_BLOCK_EINVAL;
    if(d->count>=AION_FS_META_MAX)return AION_FS_META_ENOSPC;
    for(i=0;i<d->count;i++)if(same(d->entries[i].name,name))return AION_FS_ECORRUPT;
    i=d->count;
    for(j=0;j<AION_FS_NAME_MAX;j++)d->entries[i].name[j]=j<(unsigned)n?name[j]:0;
    d->entries[i].kind=kind;d->entries[i].first_data_block=first;d->entries[i].block_count=count;d->count++;
    return 0;
}
int aion_fs_metadata_find(const struct aion_fs_directory *d,const char *name,struct aion_fs_entry *out){
    uint32_t i;
    if(!d || !out || name_length(name)<=0 || d->count>AION_FS_META_MAX)return AION_BLOCK_EINVAL;
    for(i=0;i<d->count;i++)if(same(d->entries[i].name,name)){*out=d->entries[i];return 0;}
    return AION_FS_META_ENOENT;
}
int aion_fs_metadata_save(const struct aion_fs *fs,uint64_t block,const struct aion_fs_directory *d,void *scratch,size_t size){
    uint8_t *p=(uint8_t *)scratch;uint32_t i,j;int rc=validate(fs,block,d);
    if(rc)return rc;
    if(!scratch || size<fs->device->block_size)return AION_BLOCK_EINVAL;
    for(i=0;i<fs->device->block_size;i++)p[i]=0;
    w32(p,AION_FS_META_MAGIC);w32(p+4,d->count);
    for(i=0;i<d->count;i++){
        uint8_t *e=p+8+i*48;
        for(j=0;j<AION_FS_NAME_MAX;j++)e[j]=(uint8_t)d->entries[i].name[j];
        w64(e+24,d->entries[i].first_data_block);w64(e+32,d->entries[i].block_count);e[40]=d->entries[i].kind;
    }
    return aion_fs_write_data(fs,block,p);
}
int aion_fs_metadata_load(const struct aion_fs *fs,uint64_t block,struct aion_fs_directory *d,void *scratch,size_t size){
    uint8_t *p=(uint8_t *)scratch;uint32_t i,j;int rc;
    if(!fs || !fs->device || !d || !scratch || size<fs->device->block_size)return AION_BLOCK_EINVAL;
    d->count=0;
    rc=aion_fs_read_data(fs,block,p);if(rc)return rc;
    if(r32(p)!=AION_FS_META_MAGIC || r32(p+4)>AION_FS_META_MAX)return AION_FS_EFORMAT;
    d->count=r32(p+4);
    for(i=0;i<d->count;i++){
        uint8_t *e=p+8+i*48;
        for(j=0;j<AION_FS_NAME_MAX;j++)d->entries[i].name[j]=(char)e[j];
        d->entries[i].first_data_block=r64(e+24);d->entries[i].block_count=r64(e+32);d->entries[i].kind=e[40];
    }
    rc=validate(fs,block,d);
    if(rc)d->count=0;
    return rc;
}
