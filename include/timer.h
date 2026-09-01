#pragma once
#include <stdint.h>

void timer_init(void);

void start_countdown(uint64_t ticks);

uint64_t get_timer_freq(void);