/* AION OS stage 1.3: architecture-neutral UEFI firmware entry.
 * This deliberately does not assume an operating system runtime or libc.
 * Firmware hands us an image handle and system table; later stages
 * validate the table and establish a kernel handoff protocol.
 */
#include "efi.h"
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    if (!image_handle || !system_table)
        return EFI_INVALID_PARAMETER;
    return EFI_SUCCESS;
}
