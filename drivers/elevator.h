#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "LPC214x.h"
#include "delay.h"

/*
 * Elevator — relay controlled
 * Relay IN → P0.20
 * Active HIGH → relay energizes → item drops
 */

void Elevator_Init(void);
void Elevator_Drop(void);

#endif /* ELEVATOR_H */