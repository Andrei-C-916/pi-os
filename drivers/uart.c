#include <uart.h>
#include <gpio.h>
#include <mmio.h>

#define PL011_BASE (MMIO_BASE + 0x201000)

#define DR (PL011_BASE + 0x00) // Data Register
#define FR (PL011_BASE + 0x18) // Flag Register
#define IBRD (PL011_BASE + 0x24) // Integer Baud Rate Divisor
#define FBRD (PL011_BASE + 0x28) // Fractional Baud Rate Divisor
#define LCRH (PL011_BASE + 0x2C) // Line Control Register
#define CR (PL011_BASE + 0x30) // Control Register
#define IMSC (PL011_BASE + 0x38) // Interupt Mask Set Clear Register
#define ICR (PL011_BASE + 0x44) // Interupt Clear Register


void uart_init(void) {
    mmio_write(CR, 0);

    gpio_set_function(14, GPIO_FUNC_ALT0);
    gpio_set_function(15, GPIO_FUNC_ALT0);

    gpio_set_pull(14, GPIO_PULL_NONE);
    gpio_set_pull(15, GPIO_PULL_NONE);

    mmio_write(ICR, 0x7FF);

    /* 48 MHz / (16 * 115200) = 26.0417 -> integer 26, fraction .0417 * 64 = 3 */
    mmio_write(IBRD, 26);
    mmio_write(FBRD, 3);

    /* 8-bit words, FIFOs enabled; parity off and one stop bit are the defaults. */
    mmio_write(LCRH, (1 << 4) | (3 << 5));

    /* Enable the UART, transmit, and receive. */
    mmio_write(CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(char c) {
    while (mmio_read(FR) & (1 << 5)) {} // bit 5 is transmit FIFO full, we wait for not full
    mmio_write(DR, c);
}

char uart_getc(void) {
    while (mmio_read(FR) & (1 << 4)) {} // bit 4 is receive FIFO empty, we wait for not empty
    return mmio_read(DR);
}

void uart_puts(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            uart_putc('\r');
        }
        uart_putc(str[i]);
        i ++;
    }
}

void uart_puthex(uint64_t num) {
    if (num == 0) {
        uart_putc('0');
        return;
    }

    int i = 0;
    int printedStart = 0;
    static const char digits[] = "0123456789ABCDEF";
    while (i < 16) {
        char nibble = (num >> 60) & 0xF;
        char digit = digits[nibble];
        if (nibble == 0 && !printedStart) {
            // skip leading zeros
        }
        else {
            uart_putc(digit);
            printedStart = 1;
        }
        num = num << 4;
        i++;
    }
}

uint64_t uart_gethex(void) {
    return 0;
}