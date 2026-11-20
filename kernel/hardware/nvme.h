#ifndef AION_NVME_H
#define AION_NVME_H
#include <stddef.h>
#include <stdint.h>
#define AION_NVME_OK 0
#define AION_NVME_EINVAL (-22)
#define AION_NVME_EUNSUPPORTED (-95)
#define AION_NVME_IDENTIFY_BYTES 4096u
struct aion_nvme_controller {
    uint16_t vendor_id,subsystem_vendor_id;
    uint32_t namespace_count;
    uint8_t serial[20],model[40],firmware[8];
};
struct aion_nvme_namespace {
    uint64_t block_count;
    uint32_t block_size;
    uint8_t lba_format;
};
int aion_nvme_parse_controller(const void *identify,size_t length,struct aion_nvme_controller *out);
int aion_nvme_parse_namespace(const void *identify,size_t length,struct aion_nvme_namespace *out);
#endif
