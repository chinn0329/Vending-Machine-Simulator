#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "LPC214x.h"
#include "keypad.h"
#include "lcd.h"
#include "seg7.h"
#include "stepper.h"
#include "elevator.h"
#include "motor.h"
#include "ldr.h"
#include "dac.h"
#include "inventory.h"
#include "delay.h"

typedef enum {
    STATE_IDLE,
    STATE_ITEM_SELECTED,
    STATE_PAYMENT,
    STATE_DISPENSING,
    STATE_COLLECT,
    STATE_REFUND
} VM_State;

void StateMachine_Init(void);
void StateMachine_Run(void);
void StateMachine_SetRefund(void);   /* called from EINT ISR */

#endif /* STATE_MACHINE_H */