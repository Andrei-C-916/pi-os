#include <gpio.h>
#include <mmio.h>

#define GPIO_BASE (MMIO_BASE + 0x200000)

/* Function select: 6 registers, 3 bits per pin, 10 pins per register. */
#define GPFSEL0 (GPIO_BASE + 0x00)

/* Pull-up/down control, and the clock registers that latch it per pin. */
#define GPPUD     (GPIO_BASE + 0x94)
#define GPPUDCLK0 (GPIO_BASE + 0x98)

/*
 * Busy-wait. The pull-up/down sequence needs a short settling time between
 * steps, and at this stage there is no timer to ask. asm volatile stops the
 * compiler from deleting a loop that has no observable effect.
 */
static void delay(uint32_t cycles) {
    while (cycles--) {
        __asm__ volatile("nop");
    }
}

void gpio_set_function(uint32_t pin, uint32_t func) {
    uintptr_t reg   = GPFSEL0 + (pin / 10) * 4;
    uint32_t  shift = (pin % 10) * 3;

    /*
     * Read-modify-write: this register holds nine other pins, some of them
     * driving the SD card. A blind store would reset them all.
     */
    uint32_t val = mmio_read(reg);
    val &= ~(0x7u << shift);
    val |= (func & 0x7u) << shift;
    mmio_write(reg, val);
}

void gpio_set_pull(uint32_t pin, uint32_t pull) {
    uintptr_t clock = GPPUDCLK0 + (pin / 32) * 4;

    /*
     * GPPUD holds the value to apply; GPPUDCLK selects which pins receive
     * it. The datasheet requires a 150-cycle wait after each of the first
     * two writes to let the control signal settle.
     */
    mmio_write(GPPUD, pull & 0x3u);
    delay(150);

    mmio_write(clock, 1u << (pin % 32));
    delay(150);

    mmio_write(GPPUD, 0);
    mmio_write(clock, 0);
}
