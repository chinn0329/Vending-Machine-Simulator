#include "LPC214x.h"
#include "delay.h"
#include "keypad.h"
#include "lcd.h"
#include "seg7.h"
#include "stepper.h"
#include "elevator.h"
#include "motor.h"
#include "ldr.h"
#include "dac.h"
#include "inventory.h"
#include "state_machine.h"

/* --------------------------------------------------------- PLL Init */
static void PLL_Init(void) {
    PLLCFG  = (1<<5) | 4;       /* M=5, P=2 → 60MHz from 12MHz */
    PLLCON  = 0x01;
    PLLFEED = 0xAA; PLLFEED = 0x55;
    while(!(PLLSTAT & (1<<10)));
    PLLCON  = 0x03;
    PLLFEED = 0xAA; PLLFEED = 0x55;
    VPBDIV  = 0x01;              /* PCLK = CCLK = 60MHz          */
}

/* --------------------------------- EINT1 ISR — Refund button P0.14 */
void EINT1_IRQHandler(void) __irq {
    StateMachine_SetRefund();
    EXTINT |= (1<<1);            /* clear EINT1 flag             */
    VICVectAddr = 0;
}

/* ------------------------------------------------------- EINT Setup */
static void EINT_Init(void) {
    PINSEL0 |= (1<<29);          /* P0.14 → EINT1                */
    EXTMODE |= (1<<1);           /* edge sensitive               */
    EXTPOLAR&= ~(1<<1);          /* falling edge (button GND)    */
    VICVectAddr1 = (uint32_t)EINT1_IRQHandler;
    VICVectCntl1 = (1<<5) | 15; /* enable, EINT1 = VIC slot 15  */
    VICIntEnable |= (1<<15);
}

/* ------------------------------------------------------- Entry point */
int main(void) {
    PLL_Init();

    /* Init all modules */
    Keypad_Init();
    LCD_Init();
    Seg7_Init();
    Stepper_Init();
    Elevator_Init();
    DCMotor_Init();
    LDR_Init();
    DAC_Init();
    Inventory_Init();
    EINT_Init();
    StateMachine_Init();

    /* Splash screen */
    LCD_SetCursor(0, 0); LCD_Print("Vending Machine");
    LCD_SetCursor(1, 0); LCD_Print("  LPC2148  v1.0");
    DAC_PlayBeep();
    Delay_ms(2000);

    while(1) {
        StateMachine_Run();
    }

    return 0;
}