#include <coffee/console.h>

VOID console_clear_screen(VOID)
{
  uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
}

VOID console_set_cursor_position(UINTN x, UINTN y)
{
  uefi_call_wrapper(ST->ConOut->SetCursorPosition, 3,
                    ST->ConOut,
                    x, y);
}

CHAR16 console_wait_for_key(VOID)
{
  UINTN index;
  EFI_INPUT_KEY key;
  EFI_EVENT event = ST->ConIn->WaitForKey;
  uefi_call_wrapper(BS->WaitForEvent, 3, 1,
                    &event,
                    &index);

  uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2,
                    ST->ConIn,
                    &key);
 
 return key.UnicodeChar; 
}
