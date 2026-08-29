#include <uart.h> 

void kernel_main(void) {
    uart_init();

    uart_puts("=== PI-OS booted ===\n");

    uint64_t el;
    asm volatile("mrs %0, CurrentEL" : "=r"(el));
    el = (el >> 2) & 0x3;
    uart_puts("Current Exception Level: ");
    uart_puthex(el);
    uart_putc('\r');
    uart_putc('\n');

    uart_puts("=== Welcome to PI-OS! ===\n");

    uart_puts("\n");

    // test exception
    asm volatile("svc #0");
    
    while (1) {
        char input = uart_getc();
        uart_putc(input);
        if (input == '\r') {
            uart_putc('\n');
        }
    }
}