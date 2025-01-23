#include <OutputStateMachine.h>

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

    if (endStateReached) {      // TODO: REVIEW
        return;
    }

    switch (_cycleMode)
    {
    case DECREASE_EZ:
        _nextStateDecreaseEZ();
        break;
        
    case INCREASE_EZ:
        /* code */
        break;
    
    default:    // MANUAL
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
    _currentStateNum = _currentStateNum + 1;
    _currentStateOutputsArr = outputStateArray[_currentStateNum];

    if (_currentStateNum == MAX_STATE_NUM) {
        endStateReached = true;
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

        break;
        
    case INCREASE_EZ:
        /* code */
        break;
    
    default:    // MANUAL
        break;
    }
}