# include <uart.h>
# include <stdint.h>

void exceptionHandler(void) {
    uart_puts("There has been an exception\n");
    
    uint64_t esr;
    uint64_t elr;

    asm volatile("mrs %0, ESR_EL1" : "=r"(esr));
    asm volatile("mrs %0, ELR_EL1" : "=r"(elr));

    uart_puts("Cause: ");
    uart_puthex(esr);
    uart_puts("\n");
    uart_puts("From: ");
    uart_puthex(elr);
    uart_puts("\n");
}