CLANG   = /opt/homebrew/opt/llvm/bin/clang
OBJCOPY = /opt/homebrew/opt/llvm/bin/llvm-objcopy
LD      = ld.lld
CARD    = /Volumes/APPLE-SDXC

CFLAGS = --target=aarch64-elf -mcpu=cortex-a53 -ffreestanding \
         -mgeneral-regs-only -Wall -Wextra -O2

OBJS = boot.o kernel.o

all: kernel8.img

boot.o: boot.S
	$(CLANG) $(CFLAGS) -c boot.S -o boot.o

kernel.o: kernel.c
	$(CLANG) $(CFLAGS) -c kernel.c -o kernel.o

kernel8.elf: linker.ld $(OBJS)
	$(LD) -T linker.ld -o kernel8.elf $(OBJS)

kernel8.img: kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

install: kernel8.img
	cp kernel8.img $(CARD)/
	diskutil eject $(CARD)

clean:
	rm -f *.o *.elf *.img

.PHONY: all clean install