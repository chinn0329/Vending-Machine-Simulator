#ifndef DAC_H
#define DAC_H

#include "LPC214x.h"
#include "delay.h"

/*
 * DAC — audio output
 * AOUT → P0.26
 * 10-bit DAC, DACR bits [15:6] hold value
 */

void DAC_Init(void);
void DAC_PlayBeep(void);
void DAC_PlayCoinBeep(void);
void DAC_PlayJingle(void);

#endif /* DAC_H */