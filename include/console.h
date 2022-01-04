#ifndef CONSOLE_H
#define CONSOLE_H

#include <efi.h>
#include <efilib.h>

VOID console_clear_screen(VOID);
VOID console_set_cursor_position(UINTN x, UINTN y);
CHAR16 console_wait_for_key(VOID);

#endif
