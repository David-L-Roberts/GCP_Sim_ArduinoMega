#pragma once
#include <Arduino.h>

// received switching time is multiplied by this number (e.g. recv <250>; switching time becomes 250*4 = 1000 ms)
#define SWITCH_T_MULT 4
// minimum allowed swtching time (in ms)
#define SWITCH_T_MIN 50
// number of relays that control inductors
#define NUM_RELAYS 9

/*
    Holds list of threshold times related to each relay. 
    When the swtiching time per state falls below a relay's threshold,
    that relay will stop switching in/out.
    Relays will remain in the ON state after triggering for the first time.
*/
const uint16_t relayLockoutTimes[NUM_RELAYS] = {
    1,      // Relay 9
    1,      // Relay 8
    3,      // Relay 7
    6,      // Relay 6
    12,     // Relay 5
    25,     // Relay 4
    50,     // Relay 3
    100,    // Relay 2
    200,    // Relay 1
};