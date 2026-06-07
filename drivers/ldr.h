#ifndef LDR_H
#define LDR_H

#include "LPC214x.h"
#include "delay.h"

/*
 * LDR — light dependent resistor
 * ADC0 Channel 2 → P0.25 (AIN2)
 * Used to detect when item lands in tray (light change)
 *
 * Calibrate LDR_THRESHOLD based on your environment
 */

#define LDR_THRESHOLD   512     /* 0-1023 range, adjust as needed */

void    LDR_Init(void);
uint16_t LDR_Read(void);
uint8_t  LDR_ItemDetected(void);

#endif /* LDR_H */