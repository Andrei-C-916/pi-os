#include <uart.h>
#include <gpio.h>
#include <mmio.h>

#define PL011_BASE (MMIO_BASE + 0x201000)

#define DR (PL011_BASE + 0x00)
#define FR (PL011_BASE + 0x18)
#define IBRD (PL011_BASE +0x24)
#define FBRD (PL011_BASE + 0x28)
#define LCRH (PL011_BASE + 0x2C)
#define CR (PL011_BASE + 0x30)
#define IMSC (PL011_BASE + 0x38)
#define ICR (PL011_BASE + 0x44)


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
