#include "inventory.h"

static Item inventory[MAX_ITEMS] = {
    {"Coca Cola",    20, 5,  0},
    {"Pepsi",        18, 5,  1},
    {"Sprite",       18, 3,  2},
    {"Water 500ml",  10, 8,  3},
    {"Lays Classic", 15, 4,  4},
    {"KitKat",       20, 6,  5},
    {"Dairy Milk",   25, 4,  6},
    {"Kurkure",      15, 5,  7},
    {"Nimbooz",      20, 3,  8},
    {"Red Bull",     60, 2,  9},
    {"5Star",        20, 6, 10},
    {"Frooti",       15, 7, 11}
};

void Inventory_Init(void) {
    /* Nothing to init — table is statically defined */
}

Item* Inventory_GetItem(uint8_t slot) {
    if(slot >= MAX_ITEMS) return 0;
    return &inventory[slot];
}

uint8_t Inventory_InStock(uint8_t slot) {
    if(slot >= MAX_ITEMS) return 0;
    return (inventory[slot].stock > 0) ? 1 : 0;
}

uint16_t Inventory_GetPrice(uint8_t slot) {
    if(slot >= MAX_ITEMS) return 0;
    return inventory[slot].price;
}

void Inventory_Decrement(uint8_t slot) {
    if(slot >= MAX_ITEMS) return;
    if(inventory[slot].stock > 0)
        inventory[slot].stock--;
}

/*
 * Key to slot mapping:
 * Row key  A/B/C  → row 0/1/2
 * Col key  1/2/3/4 → col 0/1/2/3
 * slot = row*4 + col
 */
uint8_t Inventory_SlotFromKey(char row_key, char col_key) {
    uint8_t row, col;
    switch(row_key) {
        case 'A': row = 0; break;
        case 'B': row = 1; break;
        case 'C': row = 2; break;
        default:  return 0xFF;   /* invalid */
    }
    switch(col_key) {
        case '1': col = 0; break;
        case '2': col = 1; break;
        case '3': col = 2; break;
        case '4': col = 3; break;
        default:  return 0xFF;   /* invalid */
    }
    return (row * 4) + col;
}