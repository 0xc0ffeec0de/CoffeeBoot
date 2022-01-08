#ifndef ELF_H
#define ELF_H

#include <efi.h>
#include <efilib.h>

struct elf_header_s
{
  UINT8   ident[16];
  UINT16  type;
  UINT16  machine;
  UINT32  version;
  UINT64  entry;
  UINT64  phoff;
  UINT64  shoff;
  UINT32  flags;
  UINT16  ehsize;
  UINT16  phentsize;
  UINT16  phnum;
  UINT16  shentsize;
  UINT16  shnum;
  UINT16  shstrndx;
}__attribute__((packed));
typedef struct elf_header_s elf_header_t;

INTN elf_is_elf(elf_header_t *);

#endif
