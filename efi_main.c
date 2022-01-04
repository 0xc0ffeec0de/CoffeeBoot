#include <efi.h>
#include <efilib.h>
#include "include/gop.h"
#include "include/loader.h"
#include "include/console.h"

VOID draw_menu(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *mode_info)
{
  console_clear_screen();
  console_set_cursor_position(36, 5);
  Print(L"CoffeeBoot\n");
  
  if(mode_info)
    Print(L"Resolution: (%dx%d)  \n", mode_info->HorizontalResolution, 
                                      mode_info->VerticalResolution);

  Print(L"+------------------------------------------+\n"
         "| 1 - Boot Kernel                          |\n"
         "| 2 - Choose Video Mode                    |\n"
         "+------------------------------------------+\n");
}

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *system_table)
{
  EFI_STATUS status;

  InitializeLib(image, system_table);

  console_clear_screen();

  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
  status = gop_init(&gop);
  if(EFI_ERROR(status))
  {
    Print(L"Failed to init gop.\n");
    return status;
  }
  
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *mode_info = NULL;
  UINTN mode = 0;
  while(1)
  {
    draw_menu(mode_info);
    
    CHAR16 key = console_wait_for_key();

    if(key == L'1')
    {
      break;
    }
    else if(key == L'2')
    {
      mode++;
      gop_get_mode_info(gop, &mode, &mode_info);
      if(EFI_ERROR(status))
      {
        Print(L"Failed to choose video mode.\n");
        return status;
      }
    }
  }

  console_clear_screen();
  console_set_cursor_position(0, 0); 

  status = loader_boot_kernel();
  if(EFI_ERROR(status))
  {
    Print(L"Failed to boot kernel.\n");
    return status;
  }

  status = gop_set_video_mode(gop, mode);
  if(EFI_ERROR(status))
  {
    Print(L"Failed to video mode.\n");
    return status;
  }

  while(1);
  return EFI_SUCCESS;
}
