#pragma once
#include <Arduino.h>
#include "OutputStates.h"
#include "PinMappings.h"


/*  FEATURES
    ---

    Allow call next state.
    Two modes:
        Train approach (decrease EZ)
        Train leave (increase EZ) 
    upon reaching end state, send signal of completion.
    calling next state on end state should return an error.

    manual and automatic mode.
        automatic mode:
            changing to a state will automatically set the digital outputs to the state.
                increase and decrease EZ are sub-categories of this state
        manual mode:
            digital outputs will not be set by the state.
            ? revert to state 0

    allow reading of current state number.
    allow reading of current state output list.

    expect following commands from PC:
        Start
        Stop
        Reset

            
*/

#define MAX_STATE_NUM NUM_STATES-1

enum CycleMode {
    DECREASE_EZ,
    INCREASE_EZ,
    MANUAL,
    IDLE,
    RESET_LOW_EX,
    RESET_HIGH_EX,
};

// enum State {
//     IDLE,
//     RUNNING,
//     ERROR
// };



/**************************************************************************/
/*!
    @brief  Class for controlling digital outputs that interface to relays.
            Uses a state machine to cycle through list of valid output
            states.
*/
/**************************************************************************/
class OutputStateMachine {
private:
    int _currentStateNum;       // number of the current state (indexes outputStateArray)
    const bool* _currentStateOutputsArr;    // array of outputs for the current state

    CycleMode _cycleMode = MANUAL;

    void _applyStateOutputs();
    void _nextStateDecreaseEZ();
    void _nextStateIncreaseEZ();

public:
    bool endStateReached = false;

    OutputStateMachine();
    void nextState();
    void changeCylceMode(CycleMode newMode);
};

