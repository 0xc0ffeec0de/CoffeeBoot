#ifndef GOP_H
#define GOP_H

#include <efi.h>
#include <efilib.h>

EFI_STATUS gop_init(EFI_GRAPHICS_OUTPUT_PROTOCOL **);
EFI_STATUS gop_get_mode_info(EFI_GRAPHICS_OUTPUT_PROTOCOL *, UINTN *, EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **);
EFI_STATUS gop_set_video_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL *, UINTN);

#endif
