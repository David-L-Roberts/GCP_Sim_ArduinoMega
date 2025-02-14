#pragma once
#include <Arduino.h>
#include "OutputStates.h"
#include "PinMappings.h"
#include "ComsAPI.h"

#define MAX_STATE_NUM NUM_STATES-1

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

