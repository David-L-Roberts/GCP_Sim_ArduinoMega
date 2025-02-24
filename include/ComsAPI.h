#pragma once

enum CycleMode {
    DECREASE_EZ     = 100,
    INCREASE_EZ     = 101,
    RESET_HIGH_EZ   = 102,
    RESET_LOW_EZ    = 103,
    MANUAL          = 110,
    IDLE            = 111
};

// action code for changing switching time. Lets system know to interpret the next message as a time value.
#define CHANGE_SWITCH_T 200
#define SET_STATE       240
#define HMI_ACK         253     // action code for acknowledging HMI hello
#define HMI_HELLO       254     // action code for startup hello from HMI
#define NO_CODE         65535   // number used to signify when there is no current action code to execute