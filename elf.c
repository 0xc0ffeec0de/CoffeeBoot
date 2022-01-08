#include "include/elf.h"

INTN elf_is_elf(elf_header_t *elf_header)
{
  return (*((UINT32 *)elf_header->ident) == 0x464c457f);
}
