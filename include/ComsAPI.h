#pragma once

// received switching time is multiplied by this number (e.g. recv <250>; switching time becomes 250*4 = 1000 ms)
#define SWITCH_T_MULT 4
// minimum allowed swtching time (in ms)
#define SWITCH_T_MIN 100

// ===================================

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
#define HMI_ACK         253     // action code for acknowledging HMI hello
#define HMI_HELLO       254     // action code for startup hello from HMI
#define NO_CODE         255     // number used to signify when there is no current action code to execute