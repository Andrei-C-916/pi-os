#pragma once
#include <stdint.h>

#define GPIO_FUNC_INPUT  0x0
#define GPIO_FUNC_OUTPUT 0x1
#define GPIO_FUNC_ALT0   0x4
#define GPIO_FUNC_ALT1   0x5
#define GPIO_FUNC_ALT2   0x6
#define GPIO_FUNC_ALT3   0x7
#define GPIO_FUNC_ALT4   0x3
#define GPIO_FUNC_ALT5   0x2

#define GPIO_PULL_NONE 0x0
#define GPIO_PULL_DOWN 0x1
#define GPIO_PULL_UP   0x2

void gpio_set_function(uint32_t pin, uint32_t func);

void gpio_set_pull(uint32_t pin, uint32_t pull);
