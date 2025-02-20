#include "OutputStateMachine.h"

#define DEBUG

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

    // TODO: add disable list here
}


/**************************************************************************/
/*!
    @brief  sets the '_cycleMode' attribute of the statemachine
    @param  newMode
            new cycle mode of the statemachine
    @return void
*/
/**************************************************************************/
void OutputStateMachine::changeCylceMode(uint8_t newMode) {
    endStateReached = false;

    switch (newMode)
    {
    case DECREASE_EZ:
        _cycleMode = DECREASE_EZ;
        #ifdef DEBUG 
            Serial.println("Mode changed: DECREASE_EZ"); 
        #endif
        break;
        
    case INCREASE_EZ:
        _cycleMode = INCREASE_EZ;
        #ifdef DEBUG 
            Serial.println("Mode changed: INCREASE_EZ");
        #endif
        break;

    case RESET_HIGH_EZ:
        _cycleMode = RESET_HIGH_EZ;
        #ifdef DEBUG 
            Serial.println("Mode changed: RESET_HIGH_EZ");
        #endif
        _currentStateNum = 0;
        _currentStateOutputsArr = outputStateArray[_currentStateNum];
        _applyStateOutputs();
        break;

    case RESET_LOW_EZ:
        _cycleMode = RESET_LOW_EZ;
        #ifdef DEBUG 
            Serial.println("Mode changed: RESET_LOW_EZ");
        #endif
        _currentStateNum = MAX_STATE_NUM;
        _currentStateOutputsArr = outputStateArray[_currentStateNum];
        _applyStateOutputs();
        break;

    case IDLE:
        _cycleMode = IDLE;
        #ifdef DEBUG 
            Serial.println("Mode changed: IDLE");
        #endif
        break;
    
    case MANUAL:
        _cycleMode = MANUAL;
        #ifdef DEBUG 
            Serial.println("Mode changed: MANUAL");
        #endif
        _currentStateNum = 0;
        _currentStateOutputsArr = outputStateArray[_currentStateNum];
        break;

    default:
        Serial.print("[ERROR] invalid serialPort.actionCode: ");
        Serial.println(newMode);
        break;
    }
}


int OutputStateMachine::getCurrentStateNum() {
    return _currentStateNum;
}