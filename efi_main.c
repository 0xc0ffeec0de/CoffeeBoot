#include <efi.h>
#include <efilib.h>
#include "include/gop.h"
#include "include/loader.h"
#include "include/console.h"

#define KERNEL_ADDR 0x100000

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
    Print(L"gop_init: Failed to init gop.\n");
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
        Print(L"gop_get_mode_info: Failed to choose video mode.\n");
        return status;
      }
    }
  }

  console_clear_screen();
  console_set_cursor_position(0, 0);

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
  status = uefi_call_wrapper(BS->LocateProtocol, 3,
                             &gEfiSimpleFileSystemProtocolGuid,
                             NULL,
                             &fs);
  if(EFI_ERROR(status))
  {
    Print(L"LocateProtocol: Failed to locate protocol.\n");
    return status;
  }

  EFI_FILE_PROTOCOL *root;
  status = uefi_call_wrapper(fs->OpenVolume, 2, fs, &root);
  if(EFI_ERROR(status))
  {
    Print(L"OpenVolume: Failed to open volume");
    return status;
  }

  Print(L"Loading \\KERNEL\\KERNEL.ELF...\n");

  EFI_FILE_PROTOCOL *kernel_file;
  status = uefi_call_wrapper(root->Open, 5,
                             &kernel_file,
                             L"\\KERNEL\\KERNEL.ELF",
                             EFI_FILE_MODE_READ,
                             EFI_FILE_READ_ONLY);
  if(EFI_ERROR(status))
  {
    Print(L"Open: Failed to open kernel file.\n");
    return status;
  }

  status = loader_load_kernel(kernel_file, (VOID *) KERNEL_ADDR);
  if(EFI_ERROR(status))
  {
    Print(L"loader_boot_kernel: Failed to boot kernel.\n");
    return status;
  }

  uefi_call_wrapper(root->Close, 1, root);
  uefi_call_wrapper(kernel_file->Close, 1, kernel_file);

  status = gop_set_video_mode(gop, mode);
  if(EFI_ERROR(status))
  {
    Print(L"gop_set_video_mode: Failed to video mode.\n");
    return status;
  }

  while(1);
  return EFI_SUCCESS;
}
