#include <timer.h>

void timer_init(void) {
    uint64_t ticks = get_timer_freq();
    start_countdown(ticks);

    uint64_t enable = 0x1; // bit 0 enables the timer, so set it to 1, bit 1 masks it so leave it unmasked. so we need 0x1
    asm volatile("msr CNTP_CTL_EL0, %0" :: "r"(enable));

    volatile uint32_t *broadcom_reg = (volatile uint32_t *)0x40000040;
    *broadcom_reg |= 0x2;
}

void start_countdown(uint64_t ticks) {
    asm volatile("msr CNTP_TVAL_EL0, %0" :: "r"(ticks));
}

uint64_t get_timer_freq(void) {
    uint64_t freq;
    asm volatile("mrs %0, CNTFRQ_EL0" : "=r"(freq));
    return freq;
}