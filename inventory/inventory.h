#ifndef INVENTORY_H
#define INVENTORY_H

#include "LPC214x.h"
#include <stdint.h>

#define MAX_ITEMS   12

typedef struct {
    char     name[16];
    uint16_t price;
    uint8_t  stock;
    uint8_t  slot;
} Item;

void     Inventory_Init(void);
Item*    Inventory_GetItem(uint8_t slot);
uint8_t  Inventory_InStock(uint8_t slot);
uint16_t Inventory_GetPrice(uint8_t slot);
void     Inventory_Decrement(uint8_t slot);
uint8_t  Inventory_SlotFromKey(char row, char col);

#endif /* INVENTORY_H */