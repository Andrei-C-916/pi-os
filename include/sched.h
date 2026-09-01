#pragma once
#include <stdint.h>

typedef struct {
    uint64_t sp;
    char stack[4096] __attribute__((aligned(16))); // sp must be 16-byte aligned
} task_struct;

