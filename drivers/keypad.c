#include "keypad.h"

static const char key_map[4][4] = {
    {'A', '1', '2', '3'},
    {'B', '4', '5', '6'},
    {'C', '7', '8', '9'},
    {'D', '*', '0', '#'}
};

void Keypad_Init(void) {
    /* P0.0-P0.3 as output (rows) */
    IO0DIR |=  (0xF << 0);
    /* P0.4-P0.7 as input (cols) */
    IO0DIR &= ~(0xF << 4);
    /* Set all rows HIGH initially */
    IO0SET  =  (0xF << 0);
}

char Keypad_GetKey(void) {
    uint8_t row, col;

    for(row = 0; row < 4; row++) {
        /* Pull current row LOW, rest HIGH */
        IO0SET  =  (0xF << 0);
        IO0CLR  =  (1   << row);
        Delay_us(10);

        for(col = 0; col < 4; col++) {
            /* Column reads LOW when key pressed (external pull-up) */
            if(!(IO0PIN & (1 << (col + 4)))) {
                Delay_ms(20);                          /* debounce */
                if(!(IO0PIN & (1 << (col + 4)))) {
                    /* Wait for key release */
                    while(!(IO0PIN & (1 << (col + 4))));
                    Delay_ms(20);
                    return key_map[row][col];
                }
            }
        }
    }
    return '\0';                                       /* no key pressed */
}