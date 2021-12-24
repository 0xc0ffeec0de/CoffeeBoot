#include "include/gop.h"
#include "include/console.h"

EFI_STATUS gop_init(EFI_GRAPHICS_OUTPUT_PROTOCOL **gop)
{
  EFI_STATUS status;
  
  EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  status = uefi_call_wrapper(BS->LocateProtocol, 3, &gop_guid, NULL, gop);
  if(EFI_ERROR(status))
  {
    Print(L"Failed to locate protocol\n");
    return status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS gop_get_mode_info(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINTN *mode, EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **mode_info)
{
  EFI_STATUS status;

  if(*mode == gop->Mode->MaxMode)
    *mode = 0; 
  console_set_cursor_position(0, 8);

  status = uefi_call_wrapper(gop->QueryMode, 4,
                             gop,
                             *mode,
                             sizeof(mode_info),
                             mode_info);

  if(EFI_ERROR(status))
  {
    Print(L"Failed to query mode.\n");
    *mode_info = NULL;
    return status;
  }
 
  return EFI_SUCCESS;
}

EFI_STATUS gop_set_video_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINTN mode)
{
  EFI_STATUS status;

  status = uefi_call_wrapper(gop->SetMode, 2, gop, mode);
  if(EFI_ERROR(status))
    return status;

  return EFI_SUCCESS;
}
