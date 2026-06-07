#include "stepper.h"

/* Half-step sequence for ULN2003 */
static const uint8_t step_seq[8] = {
    0b0001,
    0b0011,
    0b0010,
    0b0110,
    0b0100,
    0b1100,
    0b1000,
    0b1001
};

/*
 * Adjust STEPS_PER_SLOT based on your motor spec:
 * typical 28BYJ-48: 4096 steps/revolution
 * 12 slots → 4096/12 = 341 steps per slot
 */
#define STEPS_PER_REV   4096
#define STEPS_PER_SLOT  (STEPS_PER_REV / TOTAL_SLOTS)
#define STEP_DELAY_MS   2

static uint8_t  step_idx     = 0;
static uint8_t  current_slot = 0;

static void Stepper_WriteStep(uint8_t val) {
    /* Write 4-bit val to P0.16-P0.19 */
    IO0PIN = (IO0PIN & ~(0xF << 16)) | ((uint32_t)(val & 0xF) << 16);
    Delay_ms(STEP_DELAY_MS);
}

void Stepper_Init(void) {
    /* P0.16-P0.19 as output */
    IO0DIR |= (0xF << 16);
    IO0CLR  = (0xF << 16);
    step_idx     = 0;
    current_slot = 0;
}

void Stepper_Step(uint8_t dir) {
    if(dir == STEPPER_CW) {
        step_idx = (step_idx + 1) % 8;
    } else {
        step_idx = (step_idx + 7) % 8;
    }
    Stepper_WriteStep(step_seq[step_idx]);
}

void Stepper_MoveSteps(uint16_t steps, uint8_t dir) {
    uint16_t i;
    for(i = 0; i < steps; i++) {
        Stepper_Step(dir);
    }
    /* Deenergize coils after move to prevent heating */
    IO0CLR = (0xF << 16);
}

void Stepper_MoveToSlot(uint8_t slot) {
    int8_t  diff;
    uint8_t dir;
    uint16_t steps;

    if(slot >= TOTAL_SLOTS) return;

    diff = (int8_t)(slot - current_slot);

    /* Take shortest path */
    if(diff > 6)  diff -= 12;
    if(diff < -6) diff += 12;

    dir   = (diff > 0) ? STEPPER_CW : STEPPER_CCW;
    steps = (uint16_t)((diff < 0 ? -diff : diff) * STEPS_PER_SLOT);

    Stepper_MoveSteps(steps, dir);
    current_slot = slot;
}

void Stepper_Home(void) {
    Stepper_MoveToSlot(0);
}