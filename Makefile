CC=gcc
LD=ld

CFLAGS=-mno-red-zone -fno-stack-protector -fpic -fshort-wchar -I /usr/include/efi/ -I /usr/include/efi/x86_64/ -DEPI_FUNCTION_WRAPPER
			 
LDFLAGS=-nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib -l:libgnuefi.a -l:libefi.a

OBJS=efi_main.o

CoffeBoot.img: bootx64.efi
	dd if=/dev/zero of=$@ bs=1k count=2880
	mformat -i $@ -f 2880 ::
	mmd -i $@ ::/efi
	mmd -i $@ ::/efi/boot
	mmd -i $@ ::/kernel
	mcopy -i $@ $< ::/efi/boot/$<

bootx64.efi: bootx64.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 $< $@

bootx64.so: $(OBJS)
	$(LD) $^ /usr/lib/crt0-efi-x86_64.o $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean run

run:
	qemu-system-x86_64 -bios OVMF.fd -hda CoffeBoot.img

clean:
	rm *.o bootx64.so bootx64.efi CoffeBoot.img
