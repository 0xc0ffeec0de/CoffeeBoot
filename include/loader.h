#ifndef LOADER_H
#define LOADER_H

#include <efi.h>
#include <efilib.h>

EFI_STATUS loader_load_kernel(EFI_FILE_PROTOCOL *, VOID *);

#endif
