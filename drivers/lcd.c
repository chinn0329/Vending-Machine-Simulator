#include "lcd.h"

#define LCD_D4   8
#define LCD_D5   9
#define LCD_D6   10
#define LCD_D7   11
#define LCD_RS   12
#define LCD_EN   13

static void LCD_Pulse(void) {
    IO0SET = (1 << LCD_EN);
    Delay_us(1);
    IO0CLR = (1 << LCD_EN);
    Delay_us(100);
}

static void LCD_SendNibble(uint8_t nibble) {
    /* Write upper 4 bits to D4-D7 */
    if(nibble & 0x01) IO0SET = (1<<LCD_D4); else IO0CLR = (1<<LCD_D4);
    if(nibble & 0x02) IO0SET = (1<<LCD_D5); else IO0CLR = (1<<LCD_D5);
    if(nibble & 0x04) IO0SET = (1<<LCD_D6); else IO0CLR = (1<<LCD_D6);
    if(nibble & 0x08) IO0SET = (1<<LCD_D7); else IO0CLR = (1<<LCD_D7);
    LCD_Pulse();
}

static void LCD_SendByte(uint8_t rs, uint8_t data) {
    if(rs) IO0SET = (1<<LCD_RS);
    else   IO0CLR = (1<<LCD_RS);

    LCD_SendNibble(data >> 4);   /* high nibble first */
    LCD_SendNibble(data & 0x0F); /* low nibble        */
    Delay_us(50);
}

void LCD_Init(void) {
    /* All LCD pins as output */
    IO0DIR |= (1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7)
             |(1<<LCD_RS)|(1<<LCD_EN);
    IO0CLR  = (1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7)
             |(1<<LCD_RS)|(1<<LCD_EN);

    Delay_ms(20);

    /* HD44780 4-bit init sequence */
    LCD_SendNibble(0x03); Delay_ms(5);
    LCD_SendNibble(0x03); Delay_us(150);
    LCD_SendNibble(0x03); Delay_us(150);
    LCD_SendNibble(0x02); Delay_us(150);  /* set 4-bit mode */

    LCD_SendByte(0, 0x28);  /* 4-bit, 2 lines, 5x8 font */
    LCD_SendByte(0, 0x0C);  /* display ON, cursor OFF    */
    LCD_SendByte(0, 0x06);  /* entry mode: increment     */
    LCD_SendByte(0, 0x01);  /* clear display             */
    Delay_ms(2);
}

void LCD_Clear(void) {
    LCD_SendByte(0, 0x01);
    Delay_ms(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 1) ? (0x80 + col) : (0xC0 + col);
    LCD_SendByte(0, addr);
}

void LCD_PrintChar(char c) {
    LCD_SendByte(1, (uint8_t)c);
}

void LCD_Print(const char *str) {
    while(*str) {
        LCD_PrintChar(*str++);
    }
}

void LCD_PrintInt(uint32_t num) {
    char buf[11];
    uint8_t i = 0;
    if(num == 0) { LCD_PrintChar('0'); return; }
    while(num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    while(i > 0) {
        LCD_PrintChar(buf[--i]);
    }
}