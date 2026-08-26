#pragma once

#include <stdint.h>

void uart_init(void);

void uart_putc(char c);

char uart_getc(void);

void uart_puts(const char *str);

void uart_puthex(uint64_t num);

uint64_t uart_gethex(void);