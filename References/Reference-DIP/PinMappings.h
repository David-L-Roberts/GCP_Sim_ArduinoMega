#pragma once
#include <Arduino.h>

// ==================================================
//                    Solenoid Vals
// ==================================================

// number of digital pins used for solenoids
#define NUM_OF_PINS 25

// function buttons
#define F1 22
#define F2 23
#define F3 24
#define F4 25
#define F5 26

// num pad
// row 1
#define B_LEFT  28
#define B_UP    29
#define B_RIGHT 30
// row 2
#define B_TICK  31
#define B_DOWN  32
#define B_CROSS 33
// row 3
#define B_1 34
#define B_2 36
#define B_3 37
// row 4
#define B_4 38
#define B_5 39
#define B_6 40
// row 5
#define B_7 41
#define B_8 42
#define B_9 45
// row 6
#define B_STAR 47
#define B_0 49
#define B_HASH 50

// Utility Buttons
#define B_MENU 48
#define B_ID   46

// Map action codes to digital pin numbers
const uint8_t pinMappings[NUM_OF_PINS] = {
    B_0,
    B_1,
    B_2,
    B_3,
    B_4,
    B_5,
    B_6,
    B_7,
    B_8,
    B_9,
    B_STAR,
    B_HASH,
    B_LEFT,
    B_UP,
    B_RIGHT,
    B_DOWN,
    B_TICK,
    B_CROSS,
    F1,
    F2,
    F3,
    F4,
    F5,
    B_MENU,
    B_ID,
};

// ==================================================
//                    Servo Motor Vals
// ==================================================
#define SERVO_PIN   3   // pwm pin to control servo motor
#define SERVO_ON    0 // Servo angle for Power On
#define SERVO_OFF   180   // Servo angle for Power Off

const uint8_t servoAngles[2] = {
    SERVO_ON,
    SERVO_OFF,
};