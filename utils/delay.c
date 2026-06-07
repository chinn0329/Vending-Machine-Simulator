#include "delay.h"

/*
 * Timer0 setup for delay
 * PCLK = 60 MHz
 * PR   = 59  → timer ticks every 1 µs  (60MHz / 60 = 1MHz)
 */

void Delay_us(uint32_t us) {
    T0TCR = 0x02;          // reset Timer0
    T0PR  = 59;            // prescaler: 1 tick = 1 µs at 60MHz PCLK
    T0MR0 = us;            // match value = desired delay in µs
    T0MCR = 0x04;          // stop timer on MR0 match (bit2), no interrupt
    T0TCR = 0x01;          // start timer
    while(T0TCR & 0x01);   // wait until timer stops (MR0 match stops it)
}

void Delay_ms(uint32_t ms) {
    uint32_t i;
    for(i = 0; i < ms; i++) {
        Delay_us(1000);    // 1ms = 1000 µs
    }
}