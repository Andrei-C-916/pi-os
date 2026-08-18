CLANG   = /opt/homebrew/opt/llvm/bin/clang
OBJCOPY = /opt/homebrew/opt/llvm/bin/llvm-objcopy
LD      = ld.lld
CARD    = /Volumes/APPLE-SDXC

CFLAGS = --target=aarch64-elf -mcpu=cortex-a53 -ffreestanding \
         -mgeneral-regs-only -Wall -Wextra -O2 -Iinclude -MMD -MP

SRCS_C = $(wildcard kernel/*.c drivers/*.c)
SRCS_S = $(wildcard *.S)
OBJS   = $(SRCS_C:.c=.o) $(SRCS_S:.S=.o)
DEPS   = $(OBJS:.o=.d)

all: kernel8.img

%.o: %.c
	$(CLANG) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CLANG) $(CFLAGS) -c $< -o $@

kernel8.elf: linker.ld $(OBJS)
	$(LD) -T linker.ld -o $@ $(OBJS)

kernel8.img: kernel8.elf
	$(OBJCOPY) -O binary $< $@

install: kernel8.img
	cp kernel8.img $(CARD)/
	diskutil eject $(CARD)

clean:
	rm -f $(OBJS) $(DEPS) kernel8.elf kernel8.img

-include $(DEPS)

.PHONY: all clean install
