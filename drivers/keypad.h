#ifndef KEYPAD_H
#define KEYPAD_H

#include "LPC214x.h"
#include "delay.h"

/*
 * 4x4 Matrix Keypad
 * Rows    : P0.0 - P0.3  (OUTPUT)
 * Columns : P0.4 - P0.7  (INPUT with pull-up)
 *
 * Key Map:
 *   Col→  0     1     2     3
 * Row 0:  'A'   '1'   '2'   '3'
 * Row 1:  'B'   '4'   '5'   '6'
 * Row 2:  'C'   '7'   '8'   '9'
 * Row 3:  'D'   '*'   '0'   '#'
 *
 * A/B/C/D = row selectors (item row)
 * 1-4     = column selectors (item column)
 * #       = confirm/coin insert
 * *       = cancel
 */

void Keypad_Init(void);
char Keypad_GetKey(void);

#endif /* KEYPAD_H */