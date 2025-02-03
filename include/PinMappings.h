#pragma once
#include <Arduino.h>

// ==================================================
//                    Relay Vals
// ==================================================

// number of digital pins used for relays
#define NUM_OF_PINS 9

// Relay ID to digital pin
#define R01 53
#define R02 51
#define R03 49
#define R04 47
#define R05 45
#define R06 43
#define R07 41
#define R08 39
#define R09 37

// Map action codes to digital pin numbers
const uint8_t pinMappings[NUM_OF_PINS] = {
    R09,
    R08,
    R07,
    R06,
    R05,
    R04,
    R03,
    R02,
    R01,
};
