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
    cbz x1, _exceptionLevel

_loop:
    str xzr, [x0], #8
    sub x1, x1, #1
    cbnz x1, _loop

_exceptionLevel:
    // RW bit for HCR resets to 0 (32 bit), need it at 1 (64 bit)
    mrs x0, HCR_EL2
    orr x0, x0, #0x80000000
    msr HCR_EL2, x0

    // for SPSR: set [3:0] to EL1h, set all of [9:6] for now
    mov x0, #0x3c5
    msr SPSR_EL2, x0

    adr x0, _main
    msr ELR_EL2, x0
    mov x0, sp
    msr SP_EL1, x0
    eret

_main:
    // vector table
    adr x0, _vectors
    msr VBAR_EL1, x0

    bl kernel_main
    b _park

_exceptionStub:
    bl exceptionHandler
    b _park

.align 11
_vectors:
    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub

    .align 7
    b _exceptionStub
