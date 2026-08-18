#pragma once

#include <stdint.h>

/*
 * Peripheral base address.
 *
 * The BCM2837 datasheet lists peripherals at bus addresses (0x7Exxxxxx),
 * written from the VideoCore GPU's point of view. The ARM cores see the
 * same registers at 0x3Fxxxxxx.
 */
#define MMIO_BASE 0x3F000000UL

/*
 * All hardware access goes through these two functions.
 *
 * The volatile qualifier is what stops the optimizer from treating a
 * peripheral register as ordinary memory: eliding a store nobody reads
 * back, hoisting a load out of a poll loop, or reordering a sequence the
 * hardware requires. Keeping the casts here means no driver can forget it.
 */
static inline void mmio_write(uintptr_t reg, uint32_t val) {
    *(volatile uint32_t *)reg = val;
}

static inline uint32_t mmio_read(uintptr_t reg) {
    return *(volatile uint32_t *)reg;
}
