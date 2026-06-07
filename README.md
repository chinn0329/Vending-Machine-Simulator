# Vending Machine Simulator — LPC2148
### Embedded C | Keil µVision | ARM7TDMI @ 60 MHz

---

## Project Overview

A coin-operated vending machine simulator implemented on the **LPC2148 microcontroller**
using all onboard peripherals of the development board. The system accepts item selection
via a matrix keypad, processes simulated coin input, drives motors to dispense items, and
provides audio-visual feedback throughout.

---

## Hardware Components

| Component       | Role in System                              | Interface         | LPC2148 Pins        |
|-----------------|---------------------------------------------|-------------------|---------------------|
| Matrix Keypad   | Item selection (A1–D4)                      | GPIO (row/col)    | P0.0 – P0.7         |
| 7-Segment       | Price display / balance counter             | GPIO              | P1.16 – P1.23       |
| LCD 16×2        | Item name, price, status messages           | 4-bit parallel    | P0.8 – P0.15        |
| Stepper Motor   | Rotates carousel to correct item slot       | GPIO + ULN2003    | P0.16 – P0.19       |
| DC Motor        | Opens/closes delivery tray door             | PWM + L298N       | P0.21 (PWM), P0.22  |
| Elevator        | Drops item from shelf down to tray          | GPIO + relay      | P0.20               |
| DAC             | Coin beep / dispense jingle (audio out)     | DAC (AOUT)        | P0.26               |
| LEDs & Switches | Stock indicator lights / Refund button      | GPIO + EINT       | P1.0–P1.11, P0.14   |

---


## System State Machine

![Vending Machine State Machine](1b326fae-a5be-480a-9139-467501e54596.png)



**States:**
- `IDLE` — Display welcome, wait for keypad input
- `SELECTED` — Validate item, show name + price on LCD, price on 7-seg
- `PAYMENT` — Accept coin presses, update balance on 7-seg, play coin beep via DAC
- `DISPENSING` — Stepper → Elevator → DC Motor → LDR check → Jingle
- `COLLECT` — Show "Please Collect", wait 3s, close tray, return to IDLE
- `REFUND` — Triggered by EINT1, reset balance, return to IDLE

---

## Project Structure

```
VendingMachine_LPC2148/
│
├── main.c                  # Entry point, SystemInit, main loop
│
├── state_machine/
│   ├── state_machine.c     # Core FSM logic
│   └── state_machine.h
│
├── drivers/
│   ├── keypad.c            # 4x4 matrix scan (row-out, col-in)
│   ├── keypad.h
│   ├── lcd.c               # HD44780, 4-bit mode
│   ├── lcd.h
│   ├── seg7.c              # 7-segment multiplexed display
│   ├── seg7.h
│   ├── stepper.c           # Half-step sequence, slot positioning
│   ├── stepper.h
│   ├── motor.c             # DC motor PWM, L298N direction control
│   ├── motor.h
│   ├── elevator.c          # Relay trigger for elevator mechanism
│   ├── elevator.h
│   ├── dac.c               # DAC output, sine table, beep/jingle
│   ├── dac.h
│   ├── ldr.c               # ADC read, threshold for tray detection
│   └── ldr.h
│
├── inventory/
│   ├── inventory.c         # Item table, stock count, price lookup
│   └── inventory.h
│
├── utils/
│   ├── delay.c             # Software delay (ms / us)
│   └── delay.h
│
└── startup/
    └── startup_LPC21xx.s   # Keil startup assembly file (from pack)
```

---

## Keil µVision Project Setup

### Step 1 — Create Project
1. Open Keil µVision → `Project` → `New µVision Project`
2. Save as `VendingMachine.uvproj` inside the project folder
3. Select device: **NXP (Philips)** → **LPC2148**

### Step 2 — Add Startup File
- In Project window, right-click `Source Group 1` → `Add existing files`
- Add `startup_LPC21xx.s` from `Keil\ARM\Startup\` or from the device pack

### Step 3 — Add All Source Files
Add all `.c` files from `drivers/`, `state_machine/`, `inventory/`, `utils/`, and `main.c`

### Step 4 — Configure Target
Go to `Project` → `Options for Target`:

| Tab          | Setting                                      |
|--------------|----------------------------------------------|
| Device       | LPC2148                                      |
| Target       | Xtal: **12.0 MHz** (onboard crystal)         |
| Output       | Check "Create HEX File"                      |
| C/C++        | Optimization: Level 0 (for debugging)        |
| Linker       | Use default scatter file for LPC2148         |
| Debug        | Use Keil ULINK / J-Link (whichever you have) |

### Step 5 — Include Paths
In `Options → C/C++ → Include Paths`, add:
```
.\drivers
.\state_machine
.\inventory
.\utils
```

### Step 6 — Build & Flash
- Build: `F7` or `Project → Build Target`
- Flash: `Flash → Download` (or use Flash Magic for UART ISP)

---

## Clock Configuration (PLL)

LPC2148 runs at **60 MHz** using the internal PLL from a 12 MHz crystal.

```c
// In SystemInit() / main.c
// FOSC = 12 MHz, CCLK = 60 MHz
// PLL multiplier M = 5, divider P = 2
#define FOSC    12000000UL
#define CCLK    60000000UL
#define PCLK    CCLK        // peripheral clock = CCLK

void PLL_Init(void) {
    PLLCFG  = (1<<5) | (4);   // P=2, M=5
    PLLCON  = 0x01;            // enable PLL
    PLLFEED = 0xAA; PLLFEED = 0x55;
    while(!(PLLSTAT & (1<<10))); // wait for lock
    PLLCON  = 0x03;            // connect PLL
    PLLFEED = 0xAA; PLLFEED = 0x55;
    VPBDIV  = 0x01;            // PCLK = CCLK
}
```

---

## Inventory Table (Example)

```c
// In inventory.h
typedef struct {
    char     name[16];
    uint16_t price;      // in rupees (or coins)
    uint8_t  stock;
    uint8_t  slot;       // stepper carousel position (0-11)
} Item;

// 12-slot carousel (3 rows × 4 columns: A1–C4)
Item inventory[12] = {
    {"Coca Cola",   20, 5, 0},
    {"Pepsi",       18, 5, 1},
    {"Sprite",      18, 3, 2},
    {"Water 500ml", 10, 8, 3},
    {"Lays Classic",15, 4, 4},
    {"KitKat",      20, 6, 5},
    {"Dairy Milk",  25, 4, 6},
    {"Kurkure",     15, 5, 7},
    {"Nimbooz",     20, 3, 8},
    {"Red Bull",    100,2, 9},
    {"5Star",       20, 6,10},
    {"Frooti",      15, 7,11},
};
```

---

## Pin Reference Quick Card

```
P0.0  - Keypad Row 0        P1.0  - Stock LED Slot 0
P0.1  - Keypad Row 1        P1.1  - Stock LED Slot 1
P0.2  - Keypad Row 2        P1.2  - Stock LED Slot 2
P0.3  - Keypad Row 3        P1.3  - Stock LED Slot 3
P0.4  - Keypad Col 0        P1.4  - Stock LED Slot 4
P0.5  - Keypad Col 1        P1.5  - Stock LED Slot 5
P0.6  - Keypad Col 2        P1.6  - Stock LED Slot 6
P0.7  - Keypad Col 3        P1.7  - Stock LED Slot 7
                             P1.8  - Stock LED Slot 8
P0.8  - LCD D4              P1.9  - Stock LED Slot 9
P0.9  - LCD D5              P1.10 - Stock LED Slot 10
P0.10 - LCD D6              P1.11 - Stock LED Slot 11
P0.11 - LCD D7
P0.12 - LCD RS              P0.14 - Refund Switch (EINT1)
P0.13 - LCD EN
                             P0.16 - Stepper IN1
P0.20 - Elevator Relay      P0.17 - Stepper IN2
P0.21 - DC Motor PWM        P0.18 - Stepper IN3
P0.22 - DC Motor DIR        P0.19 - Stepper IN4

P0.25 - LDR / ADC0.2        P0.26 - DAC AOUT (audio)

P1.16 - 7Seg Segment A      P1.20 - 7Seg Digit Select 0
P1.17 - 7Seg Segment B      P1.21 - 7Seg Digit Select 1
P1.18 - 7Seg Segment C
P1.19 - 7Seg Segment D
```

---

## Module Development Order

Work through each module in this order. Test each one independently before combining.

| Step | Module          | Test Goal                                      |
|------|-----------------|------------------------------------------------|
| 1    | `delay.c`       | LED blink at 1Hz confirms timing               |
| 2    | `keypad.c`      | Print pressed key to UART / toggle LED         |
| 3    | `lcd.c`         | Display "Hello World" on LCD                   |
| 4    | `seg7.c`        | Count 0–9 on 7-segment                         |
| 5    | `state_machine` | IDLE → SELECTED → PAYMENT (LCD + 7-seg only)   |
| 6    | `stepper.c`     | Rotate to each of 12 slots independently       |
| 7    | `elevator.c`    | Relay pulse — confirm mechanical drop          |
| 8    | `motor.c`       | Tray open/close with PWM                       |
| 9    | `ldr.c`         | Read ADC, print threshold value via UART       |
| 10   | `dac.c`         | Play beep and jingle audio                     |
| 11   | Integration     | Full state machine with all modules combined   |

---

## Common Keil Build Errors & Fixes

| Error                              | Fix                                                  |
|------------------------------------|------------------------------------------------------|
| `undefined symbol PINSEL0`         | Add `#include "LPC214x.h"` at top of file            |
| `cannot open source file`          | Check include paths in `Options → C/C++`             |
| Startup file missing               | Add `startup_LPC21xx.s` to project                   |
| HEX file not generated             | Enable in `Options → Output → Create HEX File`       |
| UART not printing                  | Check VPBDIV — PCLK must match baud rate calculation  |
| Stepper jitter / not rotating      | Increase step delay — try 5ms minimum between steps  |

---

## Resources

- LPC2148 User Manual: `UM10139` (NXP official datasheet)
- Keil µVision IDE: https://www.keil.com/download/product/
- Flash Magic (UART ISP flashing): https://www.flashmagictool.com
- LPC2148 Header: `LPC214x.h` (included in Keil ARM pack)

---

*Build module by module. Test early. Combine late.*