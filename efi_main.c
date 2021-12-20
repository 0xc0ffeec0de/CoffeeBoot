#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *system_table)
{
  InitializeLib(image, system_table);
  uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
  Print(L"Welcome to CoffeBoot");  
  
  while(1);
  return EFI_SUCCESS;
}
