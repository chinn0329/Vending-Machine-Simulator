#ifndef SEG7_H
#define SEG7_H

#include "LPC214x.h"
#include "delay.h"

/*
 * 7-Segment Display (2 digits, multiplexed)
 * Segments A-G : P1.16 - P1.22  (common cathode)
 * Digit Select : P1.23 (D0), P1.24 (D1)
 *
 *  Segment encoding (active HIGH, common cathode):
 *        A
 *       ---
 *   F |   | B
 *       -G-
 *   E |   | C
 *       ---
 *        D
 */

void Seg7_Init(void);
void Seg7_DisplayDigit(uint8_t digit, uint8_t pos);
void Seg7_DisplayNumber(uint8_t num);  /* 0-99 */
void Seg7_Clear(void);

#endif /* SEG7_H */