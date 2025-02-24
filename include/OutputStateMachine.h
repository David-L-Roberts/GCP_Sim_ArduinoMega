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
    uint16_t _currentStateNum;       // number of the current state (indexes outputStateArray)
    bool _currentStateOutputsArr[NUM_RELAYS];    // array of outputs for the current state

    CycleMode _cycleMode = MANUAL;

    void _applyStateOutputs();
    void _nextStateDecreaseEZ();
    void _nextStateIncreaseEZ();
    void _updateCurrentStateOutputsArray();

public:
    bool endStateReached = false;
    uint16_t switchTime = 1000;

    OutputStateMachine();
    void nextState();
    void changeCylceMode(uint8_t newMode);
    CycleMode getCycleMode();

    uint16_t getCurrentStateNum();
    void setCurrentStateNum(uint16_t newStateNum);

};

