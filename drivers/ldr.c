#include "ldr.h"

void LDR_Init(void) {
    /* P0.25 → AIN2 (PINSEL1 bits 18:19 = 01) */
    PINSEL1 |= (1 << 18);

    /* ADC0: channel 2, clk div for PCLK/4, burst=0, PDN=1 */
    AD0CR = (1<<2)          /* SEL: channel 2   */
          | (14<<8)         /* CLKDIV: 60M/15=4M */
          | (1<<21)         /* PDN: operational  */
          | (0<<16)         /* BURST: software   */
          | (0<<24);        /* START: stop       */
}

uint16_t LDR_Read(void) {
    uint32_t result;

    AD0CR |=  (1<<24);               /* start conversion       */
    while(!(AD0DR2 & (1<<31)));      /* wait for DONE bit      */
    result = (AD0DR2 >> 6) & 0x3FF; /* 10-bit result          */
    AD0CR &= ~(1<<24);               /* stop                   */
    return (uint16_t)result;
}

uint8_t LDR_ItemDetected(void) {
    return (LDR_Read() < LDR_THRESHOLD) ? 1 : 0;
}