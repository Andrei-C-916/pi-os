#pragma once

#include <stdint.h>

/*
 * Pin function codes, as written into the GPFSELn registers.
 *
 * Note that ALT4 and ALT5 are not in numeric order - this encoding comes
 * straight from the datasheet, so don't compute a code as ALT0 + n.
 */
#define GPIO_FUNC_INPUT  0x0
#define GPIO_FUNC_OUTPUT 0x1
#define GPIO_FUNC_ALT0   0x4
#define GPIO_FUNC_ALT1   0x5
#define GPIO_FUNC_ALT2   0x6
#define GPIO_FUNC_ALT3   0x7
#define GPIO_FUNC_ALT4   0x3
#define GPIO_FUNC_ALT5   0x2

/* Pull-up/pull-down resistor settings, written into GPPUD. */
#define GPIO_PULL_NONE 0x0
#define GPIO_PULL_DOWN 0x1
#define GPIO_PULL_UP   0x2

/* Point a pin's multiplexer at one of its functions. */
void gpio_set_function(uint32_t pin, uint32_t func);

/* Enable or disable a pin's internal pull-up/pull-down resistor. */
void gpio_set_pull(uint32_t pin, uint32_t pull);
