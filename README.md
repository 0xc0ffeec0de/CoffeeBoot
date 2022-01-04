# CoffeeBoot


![Img](imgs/CoffeeBoot.png)

CoffeeBoot is a UEFI bootloader for x86_64 architecture. It looks for an executable elf (KERNEL.ELF) in the /KERNEL directory of a FAT partition. If the kernel is found, then the perks are copied to address `0x100000` and the code is passed to the kernel.

It also provides a framebuffer that will be passed to the kernel video driver. Furthermore, GOP video modes are used with modern solutions.

---
## Features
- [X] Boot UEFI
- [ ] Boot Legacy

## Dependencies

[GNU-EFI](https://wiki.osdev.org/GNU-EFI)

[QEMU](https://www.qemu.org)

[MTOOLS](http://www.gnu.org/software/mtools/)
