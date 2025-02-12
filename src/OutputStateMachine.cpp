#include "OutputStateMachine.h"

/**************************************************************************/
/*!
    @brief  Constructor
*/
/**************************************************************************/
OutputStateMachine::OutputStateMachine() {
    _currentStateNum = 0;
    _currentStateOutputsArr = outputStateArray[_currentStateNum];
}


/**************************************************************************/
/*!
    @brief  Transition outputs to next state, dependent on cycle mode.
    @return void
*/
/**************************************************************************/
void OutputStateMachine::nextState() {

    if (endStateReached) {      // TODO: REVIEW - send signal on completion
        return; // do nothing
    }

    switch (_cycleMode)
    {
    case DECREASE_EZ:
        _nextStateDecreaseEZ();
        _applyStateOutputs();
        break;
        
    case INCREASE_EZ:
        _nextStateIncreaseEZ();
        _applyStateOutputs();
        break;
        
    case MANUAL:
        // _applyStateOutputs();
        break;
    
    default:    // Other: IDLE, RESET_HIGH_EZ, RESET_LOW_EZ
        // pass
        break;
    }
}

/**************************************************************************/
/*!
    @brief  Transition outputs to the next state, giving a decrease in EZ.
    @return void
*/
/**************************************************************************/
void OutputStateMachine::_nextStateDecreaseEZ() {
    if (_currentStateNum == MAX_STATE_NUM) {
        endStateReached = true;
        return;
    } else if (_currentStateNum == (MAX_STATE_NUM-1)) {
        endStateReached = true;
    }

    _currentStateNum = _currentStateNum + 1;
    _currentStateOutputsArr = outputStateArray[_currentStateNum];
}

/**************************************************************************/
/*!
    @brief  Transition outputs to the next state, giving an increase in EZ.
    @return void
*/
/**************************************************************************/
void OutputStateMachine::_nextStateIncreaseEZ() {
    if (_currentStateNum == 0) {
        endStateReached = true;
        return;
    } else if (_currentStateNum == 1) {
        endStateReached = true;
    }

    _currentStateNum = _currentStateNum - 1;
    _currentStateOutputsArr = outputStateArray[_currentStateNum];
}

/**************************************************************************/
/*!
    @brief  Change the digital outputs to align with the state.
    @return void
*/
/**************************************************************************/
void OutputStateMachine::_applyStateOutputs() {
    int pinNumber;
    int val;

    for (int i; i < NUM_OUTPUTS; i++) {
        val = _currentStateOutputsArr[i];
        pinNumber = pinMappings[i];
        digitalWrite(pinNumber, val);
    }
}


/**************************************************************************/
/*!
    @brief  sets the '_cycleMode' attribute of the statemachine
    @param  newMode
            new cycle mode of the statemachine
    @return void
*/
/**************************************************************************/
void OutputStateMachine::changeCylceMode(CycleMode newMode) {
    _cycleMode = newMode;
    endStateReached = false;

    switch (newMode)
    {
    case DECREASE_EZ:
        // Temp
        break;
        
    case INCREASE_EZ:
        // Temp
        break;

    case RESET_HIGH_EZ:
        _currentStateNum = 0;
        _currentStateOutputsArr = outputStateArray[_currentStateNum];
        _applyStateOutputs();
        break;

    case RESET_LOW_EZ:
        _currentStateNum = MAX_STATE_NUM;
        _currentStateOutputsArr = outputStateArray[_currentStateNum];
        _applyStateOutputs();
        break;

    case IDLE:
        /* code */
        break;
    
    case MANUAL:
        _currentStateNum = 0;
        _currentStateOutputsArr = outputStateArray[_currentStateNum];
        break;

    default:
        break;
    }
}