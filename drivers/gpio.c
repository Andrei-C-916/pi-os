#include <gpio.h>
#include <mmio.h>

#define GPIO_BASE (MMIO_BASE + 0x200000)

#define GPFSEL0 (GPIO_BASE + 0x00)

#define GPPUD     (GPIO_BASE + 0x94)
#define GPPUDCLK0 (GPIO_BASE + 0x98)

static void delay(uint32_t cycles) {
    while (cycles--) {
        __asm__ volatile("nop");
    }
}

void gpio_set_function(uint32_t pin, uint32_t func) {
    uintptr_t reg   = GPFSEL0 + (pin / 10) * 4;
    uint32_t  shift = (pin % 10) * 3;

    uint32_t val = mmio_read(reg);
    val &= ~(0x7u << shift);
    val |= (func & 0x7u) << shift;
    mmio_write(reg, val);
}

void gpio_set_pull(uint32_t pin, uint32_t pull) {
    uintptr_t clock = GPPUDCLK0 + (pin / 32) * 4;

    mmio_write(GPPUD, pull & 0x3u);
    delay(150);

    mmio_write(clock, 1u << (pin % 32));
    delay(150);

    mmio_write(GPPUD, 0);
    mmio_write(clock, 0);
}
