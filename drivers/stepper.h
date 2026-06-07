#ifndef STEPPER_H
#define STEPPER_H

#include "LPC214x.h"
#include "delay.h"

/*
 * Stepper Motor — ULN2003 driver
 * IN1 → P0.16   IN2 → P0.17
 * IN3 → P0.18   IN4 → P0.19
 *
 * Half-step sequence (8 steps per cycle)
 * 12 slots on carousel → steps_per_slot calculated at init
 */

#define STEPPER_CW    1
#define STEPPER_CCW   0
#define TOTAL_SLOTS   12

void Stepper_Init(void);
void Stepper_Step(uint8_t dir);
void Stepper_MoveSteps(uint16_t steps, uint8_t dir);
void Stepper_MoveToSlot(uint8_t slot);
void Stepper_Home(void);

#endif /* STEPPER_H */