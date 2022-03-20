CC=gcc
LD=ld

OVMF_DIR=/usr/share/edk2-ovmf/x64/

CFLAGS=-mno-red-zone -fno-stack-protector -fpic -fshort-wchar -I include -I /usr/include/efi/ -I /usr/include/efi/x86_64/ -DEPI_FUNCTION_WRAPPER
LDFLAGS=-nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib -l:libgnuefi.a -l:libefi.a

COFFEE_OBJS=efi-main.o loader.o gop.o console.o
COFFEE_IMG=CoffeeBoot.img

$(COFFEE_IMG): bootx64.efi
	dd if=/dev/zero of=$@ bs=1k count=2880
	mformat -i $@ -f 2880 ::
	mmd -i $@ ::/efi
	mmd -i $@ ::/efi/boot
	mmd -i $@ ::/kernel
	mcopy -i $@ $< ::/efi/boot/$<

bootx64.efi: bootx64.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 $< $@

bootx64.so: $(COFFEE_OBJS)
	$(LD) $^ /usr/lib/crt0-efi-x86_64.o $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	qemu-system-x86_64 -L $(OVMF_DIR) -bios OVMF.fd -drive file=$(COFFEE_IMG),format=raw
clean:
	rm $(COFFEE_OBJS) bootx64.so bootx64.efi $(COFFEE_IMG)

.PHONY: all clean run

