#ifndef LCD_H
#define LCD_H

#include "LPC214x.h"
#include "delay.h"

/*
 * LCD 16x2 — HD44780 compatible
 * 4-bit parallel mode
 *
 * D4 → P0.8   D5 → P0.9
 * D6 → P0.10  D7 → P0.11
 * RS → P0.12  EN → P0.13
 * RW → GND    VSS/VDD/VEE as per datasheet
 */

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_PrintChar(char c);
void LCD_Print(const char *str);
void LCD_PrintInt(uint32_t num);

#endif /* LCD_H */