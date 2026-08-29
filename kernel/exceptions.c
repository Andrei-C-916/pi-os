# include <uart.h>

void exceptionHandler(void) {
    uart_puts("There has been an exception\n");
}