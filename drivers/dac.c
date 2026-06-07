#include "dac.h"

/* 32-sample sine table (0-1023 range for 10-bit DAC) */
static const uint16_t sine_table[32] = {
    512, 612, 707, 793, 868, 927, 968, 990,
    1023, 990, 968, 927, 868, 793, 707, 612,
    512, 412, 317, 231, 156,  97,  56,  34,
      0,  34,  56,  97, 156, 231, 317, 412
};

void DAC_Init(void) {
    /* P0.26 → AOUT (PINSEL1 bits 20:21 = 10) */
    PINSEL1 |= (2 << 20);
    DACR = (512 << 6);   /* mid-rail idle */
}

static void DAC_PlayTone(uint16_t freq_hz, uint16_t duration_ms) {
    /*
     * freq_hz  : desired tone frequency
     * Each sine cycle = 32 samples
     * sample_delay_us = 1000000 / (freq_hz * 32)
     */
    uint32_t sample_delay_us = 1000000UL / ((uint32_t)freq_hz * 32);
    uint32_t total_samples   = (uint32_t)duration_ms * freq_hz * 32 / 1000;
    uint32_t i;

    for(i = 0; i < total_samples; i++) {
        DACR = (uint32_t)(sine_table[i % 32]) << 6;
        Delay_us(sample_delay_us);
    }
    DACR = (512 << 6);   /* return to mid-rail */
}

void DAC_PlayBeep(void) {
    DAC_PlayTone(1000, 100);    /* 1kHz, 100ms */
}

void DAC_PlayCoinBeep(void) {
    DAC_PlayTone(1500, 80);     /* 1.5kHz, 80ms */
    Delay_ms(50);
    DAC_PlayTone(2000, 80);     /* 2kHz, 80ms   */
}

void DAC_PlayJingle(void) {
    /* Simple 3-note jingle */
    DAC_PlayTone(523, 200);     /* C5 */
    Delay_ms(50);
    DAC_PlayTone(659, 200);     /* E5 */
    Delay_ms(50);
    DAC_PlayTone(784, 400);     /* G5 */
}