#ifndef DELAY_H
#define DELAY_H

#include "LPC214x.h"
#include <stdint.h>

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

#endif