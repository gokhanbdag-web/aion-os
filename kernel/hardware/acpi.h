#ifndef AION_ACPI_H
#define AION_ACPI_H
#include <stddef.h>
#include <stdint.h>
#define AION_ACPI_OK 0
#define AION_ACPI_EINVAL (-22)
#define AION_ACPI_ECORRUPT (-74)
#define AION_ACPI_ENOENT (-2)
struct aion_acpi_rsdp {
    uint8_t revision;
    uint64_t root_address;
    uint8_t use_xsdt;
};
struct aion_acpi_sdt {
    uint8_t signature[4];
    uint32_t length;
    uint8_t revision;
};
/* Buffers must be mapped and accessible; no physical-memory dereferencing. */
int aion_acpi_parse_rsdp(const void *buffer,size_t length,struct aion_acpi_rsdp *result);
int aion_acpi_parse_sdt(const void *buffer,size_t length,struct aion_acpi_sdt *result);
#endif
