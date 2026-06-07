#include "elevator.h"

#define ELEVATOR_PIN    20
#define DROP_DURATION   1500    /* ms relay stays ON */

void Elevator_Init(void) {
    IO0DIR |=  (1 << ELEVATOR_PIN);
    IO0CLR  =  (1 << ELEVATOR_PIN);  /* relay OFF by default */
}

void Elevator_Drop(void) {
    IO0SET = (1 << ELEVATOR_PIN);    /* energize relay   */
    Delay_ms(DROP_DURATION);         /* hold for drop    */
    IO0CLR = (1 << ELEVATOR_PIN);    /* release relay    */
    Delay_ms(500);                   /* settle time      */
}