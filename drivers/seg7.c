#include "seg7.h"

/*
 * Segment encoding for 0-9 (gfedcba on bits 0-6)
 * P1.16=a, P1.17=b, P1.18=c, P1.19=d,
 * P1.20=e, P1.21=f, P1.22=g
 */
static const uint8_t seg_code[10] = {
    0x3F, /* 0: abcdef   */
    0x06, /* 1: bc       */
    0x5B, /* 2: abdeg    */
    0x4F, /* 3: abcdg    */
    0x66, /* 4: bcfg     */
    0x6D, /* 5: acdfg    */
    0x7D, /* 6: acdefg   */
    0x07, /* 7: abc      */
    0x7F, /* 8: abcdefg  */
    0x6F  /* 9: abcdfg   */
};

void Seg7_Init(void) {
    /* P1.16-P1.24 as output */
    IO1DIR |= (0x1FF << 16);
    IO1CLR  = (0x1FF << 16);
}

void Seg7_DisplayDigit(uint8_t digit, uint8_t pos) {
    uint8_t code;
    if(digit > 9) return;
    code = seg_code[digit];

    /* Clear all segment pins first */
    IO1CLR = (0x7F << 16);

    /* Select digit position (pos 0 = units, pos 1 = tens) */
    if(pos == 0) {
        IO1SET = (1<<23); IO1CLR = (1<<24);
    } else {
        IO1CLR = (1<<23); IO1SET = (1<<24);
    }

    /* Write segment data to P1.16-P1.22 */
    IO1SET = ((uint32_t)(code & 0x7F) << 16);
    Delay_ms(5);
}

void Seg7_DisplayNumber(uint8_t num) {
    if(num > 99) num = 99;
    Seg7_DisplayDigit(num / 10, 1);   /* tens  */
    Seg7_DisplayDigit(num % 10, 0);   /* units */
}

void Seg7_Clear(void) {
    IO1CLR = (0x1FF << 16);
}