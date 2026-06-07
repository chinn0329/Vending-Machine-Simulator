#include "state_machine.h"

static VM_State  current_state  = STATE_IDLE;
static uint16_t  balance        = 0;
static uint8_t   selected_slot  = 0xFF;
static char      row_key        = '\0';

/* Called from EINT1 ISR in main.c */
void StateMachine_SetRefund(void) {
    current_state = STATE_REFUND;
}

void StateMachine_Init(void) {
    current_state = STATE_IDLE;
    balance       = 0;
    selected_slot = 0xFF;
}

void StateMachine_Run(void) {
    char     key;
    Item    *item;

    switch(current_state) {

        /* -------------------------------------------------- IDLE */
        case STATE_IDLE:
            LCD_Clear();
            LCD_SetCursor(0, 0); LCD_Print("Select Item:");
            LCD_SetCursor(1, 0); LCD_Print("A-C, then 1-4");
            Seg7_Clear();
            row_key       = '\0';
            selected_slot = 0xFF;

            key = Keypad_GetKey();
            if(key == 'A' || key == 'B' || key == 'C') {
                row_key = key;
                current_state = STATE_ITEM_SELECTED;
            }
            break;

        /* ----------------------------------------- ITEM SELECTED */
        case STATE_ITEM_SELECTED:
            LCD_Clear();
            LCD_SetCursor(0, 0); LCD_Print("Col? Press 1-4");

            key = Keypad_GetKey();
            if(key >= '1' && key <= '4') {
                selected_slot = Inventory_SlotFromKey(row_key, key);
                item = Inventory_GetItem(selected_slot);

                if(item == 0 || !Inventory_InStock(selected_slot)) {
                    LCD_Clear();
                    LCD_SetCursor(0, 0); LCD_Print("Out of Stock!");
                    DAC_PlayBeep();
                    Delay_ms(2000);
                    current_state = STATE_IDLE;
                } else {
                    LCD_Clear();
                    LCD_SetCursor(0, 0); LCD_Print(item->name);
                    LCD_SetCursor(1, 0); LCD_Print("Rs.");
                    LCD_PrintInt(item->price);
                    Seg7_DisplayNumber((uint8_t)item->price);
                    DAC_PlayBeep();
                    Delay_ms(1500);
                    balance = 0;
                    current_state = STATE_PAYMENT;
                }
            } else if(key == '*') {
                current_state = STATE_IDLE;
            }
            break;

        /* -------------------------------------------- PAYMENT */
        case STATE_PAYMENT:
            item = Inventory_GetItem(selected_slot);
            LCD_Clear();
            LCD_SetCursor(0, 0); LCD_Print("Insert Coins");
            LCD_SetCursor(1, 0); LCD_Print("# = +Rs.5  *=cancel");

            key = Keypad_GetKey();
            if(key == '#') {
                balance += 5;
                DAC_PlayCoinBeep();
                Seg7_DisplayNumber((uint8_t)(balance > 99 ? 99 : balance));
                LCD_SetCursor(1, 0);
                LCD_Print("Bal: Rs.");
                LCD_PrintInt(balance);
                LCD_Print("   ");

                if(item && balance >= item->price) {
                    Delay_ms(500);
                    current_state = STATE_DISPENSING;
                }
            } else if(key == '*') {
                current_state = STATE_REFUND;
            }
            break;

        /* ----------------------------------------- DISPENSING */
        case STATE_DISPENSING:
            LCD_Clear();
            LCD_SetCursor(0, 0); LCD_Print("Dispensing...");

            Stepper_MoveToSlot(selected_slot);

            LCD_SetCursor(1, 0); LCD_Print("Dropping...");
            Elevator_Drop();

            LCD_SetCursor(1, 0); LCD_Print("Opening tray ");
            DCMotor_OpenTray();

            /* Wait for item to land in tray */
            LCD_SetCursor(1, 0); LCD_Print("Detecting...  ");
            while(!LDR_ItemDetected()) {
                Delay_ms(100);
            }

            DAC_PlayJingle();
            Inventory_Decrement(selected_slot);
            balance = 0;
            Seg7_Clear();

            current_state = STATE_COLLECT;
            break;

        /* -------------------------------------------- COLLECT */
        case STATE_COLLECT:
            LCD_Clear();
            LCD_SetCursor(0, 0); LCD_Print("Please Collect");
            LCD_SetCursor(1, 0); LCD_Print("Thank You!");
            Delay_ms(4000);
            DCMotor_CloseTray();
            Stepper_Home();
            current_state = STATE_IDLE;
            break;

        /* --------------------------------------------- REFUND */
        case STATE_REFUND:
            LCD_Clear();
            LCD_SetCursor(0, 0); LCD_Print("Refunding...");
            LCD_SetCursor(1, 0); LCD_Print("Rs.");
            LCD_PrintInt(balance);
            Seg7_DisplayNumber((uint8_t)(balance > 99 ? 99 : balance));
            DAC_PlayBeep();
            Delay_ms(2000);
            balance = 0;
            Seg7_Clear();
            current_state = STATE_IDLE;
            break;
    }
}