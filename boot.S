.section ".text.boot"
.global _start

_start:
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    cbz x0, _stack

_park:
    wfe
    b _park

_stack:
    ldr x0, =_start
    mov sp, x0

    ldr x0, =__bss_start
    ldr x1, =__bss_size
    cbz x1, _main

_loop:
    str xzr, [x0], #8
    sub x1, x1, #1
    cbnz x1, _loop

_main:
    bl kernel_main
    b _park
