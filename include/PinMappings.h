#pragma once
#include <Arduino.h>

// ==================================================
//                    Relay Vals
// ==================================================

// number of digital pins used for relays
#define NUM_OF_PINS 9

// Relay ID to digital pin
#define R01 31
#define R02 33
#define R03 35
#define R04 37
#define R05 39
#define R06 41
#define R07 43
#define R08 45
#define R09 47

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
