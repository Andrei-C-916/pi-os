#include <uart.h> 

void kernel_main(void) {
    uart_init();
    uart_puts("hello, world!\n");
    while (1) {
        char input = uart_getc();
        uart_putc(input);
    }
}