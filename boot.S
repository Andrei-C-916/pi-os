.section ".text.boot"
.global _start

_start:
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    cbz x0, stack

park:
    wfe
    b park

stack:
    ldr x0, =_start
    mov sp, x0

    ldr x0, =__bss_start
    ldr x1, =__bss_size
    cbz x1, exception_level

loop:
    str xzr, [x0], #8
    sub x1, x1, #1
    cbnz x1, loop

exception_level:
    // RW bit for HCR resets to 0 (32 bit), need it at 1 (64 bit)
    mrs x0, HCR_EL2
    orr x0, x0, #0x80000000
    msr HCR_EL2, x0

    // for SPSR: set [3:0] to EL1h, set all of [9:6] for now
    mov x0, #0x3c5
    msr SPSR_EL2, x0

    adr x0, main
    msr ELR_EL2, x0
    mov x0, sp
    msr SP_EL1, x0
    eret

main:
    // vector table
    adr x0, vectors
    msr VBAR_EL1, x0

    // unmask interrupt requests
    msr daifclr, #2

    bl kernel_main
    b park

.macro save_regs
    stp x0, x1, [sp, #-16]!
    stp x2, x3, [sp, #-16]!
    stp x4, x5, [sp, #-16]!
    stp x6, x7, [sp, #-16]!
    stp x8, x9, [sp, #-16]!
    stp x10, x11, [sp, #-16]!
    stp x12, x13, [sp, #-16]!
    stp x14, x15, [sp, #-16]!
    stp x16, x17, [sp, #-16]!
    stp x18, x30, [sp, #-16]!
.endm

.macro restore_regs
    ldp x18, x30, [sp], #16
    ldp x16, x17, [sp], #16
    ldp x14, x15, [sp], #16
    ldp x12, x13, [sp], #16
    ldp x10, x11, [sp], #16
    ldp x8, x9, [sp], #16
    ldp x6, x7, [sp], #16
    ldp x4, x5, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16
.endm

exception_stub:
    save_regs
    bl exception_handler
    restore_regs
    eret

irq_exception_stub:
    save_regs
    bl irq_exception_handler
    restore_regs
    eret


.align 11
vectors:
    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b irq_exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub

    .align 7
    b exception_stub
