#ifndef MOTOR_H
#define MOTOR_H

#include "LPC214x.h"
#include "delay.h"

/*
 * DC Motor — L298N driver
 * PWM  → P0.21  (PWM1 channel)
 * DIR  → P0.22  (direction control)
 * EN   → P0.23  (enable, keep HIGH)
 */

void DCMotor_Init(void);
void DCMotor_OpenTray(void);
void DCMotor_CloseTray(void);
void DCMotor_Stop(void);

#endif /* MOTOR_H */