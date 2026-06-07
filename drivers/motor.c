#include "motor.h"

#define MOTOR_DIR   22
#define MOTOR_EN    23

/*
 * PWM1 on P0.21
 * PCLK = 60MHz, period = 60000 ticks = 1ms → 1kHz PWM
 * Duty 50% = 30000
 */
#define PWM_PERIOD  60000
#define PWM_DUTY    30000
#define TRAY_TIME   1200     /* ms to fully open/close tray */

void DCMotor_Init(void) {
    /* P0.21 → PWM1.1 function (PINSEL1 bits 10:11 = 10) */
    PINSEL1 |= (2 << 10);

    /* P0.22, P0.23 as GPIO output */
    IO0DIR |= (1<<MOTOR_DIR) | (1<<MOTOR_EN);
    IO0CLR  = (1<<MOTOR_DIR);
    IO0SET  = (1<<MOTOR_EN);

    /* PWM1 setup */
    PWM1PR  = 0;
    PWM1MR0 = PWM_PERIOD;   /* period   */
    PWM1MR1 = PWM_DUTY;     /* duty 50% */
    PWM1MCR = (1<<1);       /* reset TC on MR0 match */
    PWM1PCR = (1<<9);       /* enable PWM1 output ch1 */
    PWM1LER = (1<<0)|(1<<1);/* latch MR0 and MR1      */
    PWM1TCR = (1<<0)|(1<<3);/* enable timer + PWM     */
}

void DCMotor_OpenTray(void) {
    IO0SET = (1<<MOTOR_DIR);   /* forward direction */
    IO0SET = (1<<MOTOR_EN);
    Delay_ms(TRAY_TIME);
    DCMotor_Stop();
}

void DCMotor_CloseTray(void) {
    IO0CLR = (1<<MOTOR_DIR);   /* reverse direction */
    IO0SET = (1<<MOTOR_EN);
    Delay_ms(TRAY_TIME);
    DCMotor_Stop();
}

void DCMotor_Stop(void) {
    IO0CLR = (1<<MOTOR_EN);
    Delay_ms(100);
}