#ifndef AION_EFI_H
#define AION_EFI_H
#include <stdint.h>
#include <stddef.h>
#if defined(_M_X64) || defined(__x86_64__)
#define EFIAPI __attribute__((ms_abi))
#else
#define EFIAPI
#endif
typedef uintptr_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef struct EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;
#define EFI_SUCCESS ((EFI_STATUS)0)
#define EFI_INVALID_PARAMETER ((EFI_STATUS)2)
#endif
