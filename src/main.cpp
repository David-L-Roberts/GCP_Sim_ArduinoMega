#include <Arduino.h>
#include <math.h>
#include "SerialPort.h"
#include "PinMappings.h"
#include "OutputStateMachine.h"
#include "Configs.h"

// ==================================================
//                 Function Prototypes
// ==================================================

void setupOutputPinModes(const uint8_t *pinMappings);
void processRelayActionCode(SerialPort &serialPort, const uint8_t *pinMappings);
void toggleDigitalPin(const uint8_t &pin);


// ==================================================
//                      SETUP
// ==================================================

#define BAUD_RATE 9600
#define DEFAULT_WAIT_TIME 200       // in milliseconds

SerialPort serialPort = SerialPort();   // Custom Serial Port object
OutputStateMachine outputSM = OutputStateMachine();

uint16_t switchTime = DEFAULT_WAIT_TIME;
uint16_t switchTimeAdjusted= switchTime;
uint16_t newStateNum;
bool switch_t_flag = false;
bool setStateFlag = false;

// ==================================================
//                      Main Loop
// ==================================================

void setup() {
    // setup pins
    setupOutputPinModes(pinMappings);

    // begin serial
    Serial.begin(BAUD_RATE);
    Serial.println("=== System Start ===");
}

void loop() {
    // read data from the serial
    serialPort.readFromSerial();

    // if an action code was recieved, process it
    if (serialPort.actionCode != NO_CODE) {

        if (switch_t_flag == true) {
            switchTime = serialPort.actionCode * SWITCH_T_MULT;
            if (switchTime < SWITCH_T_MIN) switchTime = SWITCH_T_MIN;
            switch_t_flag = false;
            Serial.println("|DEBUG| Updating base switching period to: " + String(switchTime) + " ms");
        }
        else if (setStateFlag == true) {
            newStateNum = serialPort.actionCode;
            if (newStateNum >= NUM_STATES) {
                Serial.println("[ERROR] Invalid state number received (" + String(newStateNum) 
                                + "). Changing to: " + String(NUM_STATES-1));
                newStateNum = NUM_STATES - 1;
            }
            outputSM.setCurrentStateNum(newStateNum);
            Serial.println("System state set to: #" + String(outputSM.getCurrentStateNum()));
            setStateFlag = false;
        }
        else if (serialPort.actionCode == CHANGE_SWITCH_T) {
            switch_t_flag = true;
        }
        else if (serialPort.actionCode == SET_STATE) {
            setStateFlag = true;
            if (outputSM.getCycleMode() != MANUAL) {
                outputSM.changeCylceMode(MANUAL);
            }
        }
        else if (serialPort.actionCode == HMI_HELLO) {
            Serial.print('<' + String(HMI_ACK) + '>');
        } 
        else if (serialPort.actionCode < NUM_RELAYS) {  // relay action code
            processRelayActionCode(serialPort, pinMappings);
        }
        else {
            outputSM.changeCylceMode(serialPort.actionCode);
        }

        // clear the action-code
        serialPort.actionCode = NO_CODE;
    }

    int stateNum = outputSM.getCurrentStateNum();
    // ==================================================
    //                DYNAMIC SWITCHING
    // ==================================================
    // adjust switching time to linearize EZ curve
    int timeAdjust = floor((7*pow(10, -6)*pow(stateNum, 2) - 0.0055*stateNum + 1.02) * (switchTime*SWITCH_BASE_MULT));

    // DEBUG
    // Serial.print(stateNum);
    // Serial.print(" :: ");
    // Serial.println(timeAdjust);

    // TODO: can probably move this logic to a better location (into outputSM)
    // Log arrival to every 10th state number
    if ((outputSM.getCycleMode() == DECREASE_EZ) || (outputSM.getCycleMode() == INCREASE_EZ)) {
        if (((stateNum % 10) == 0) && (stateNum != 0) && (stateNum != MAX_STATE_NUM)) {
            Serial.print("[DEBUG] :: state reached = ");
            Serial.println(stateNum);
        }
    }


    // increment state machine
    switchTimeAdjusted = switchTime + timeAdjust;
    outputSM.switchTime = switchTimeAdjusted;
    outputSM.nextState();
    delay(switchTimeAdjusted);
}


// ==================================================
//                Function Definitions
// ==================================================

/**************************************************************************/
/*!
    @brief  Set all required digital pins to OUTPUT mode
    @param  pinMappings
            Array containing digital pins to be used as output.
    @return void
*/
/**************************************************************************/
void setupOutputPinModes(const uint8_t *pinMappings) {
    for (int i; i < NUM_OF_PINS; i++) {
        pinMode(pinMappings[i], OUTPUT);
    }
}


/**************************************************************************/
/*!
    @brief  Toggle the relay corresponding to the currently
            received action code.
    @param  SerialPort
            Object for reading data from serial Comm port and processing the data.
    @param  pinMappings
            Array mapping action codes to digital pins used for relays.
    @return void
*/
/**************************************************************************/
void processRelayActionCode(SerialPort &serialPort, const uint8_t *pinMappings) {
    // toggle the digital pin that corrsponds to the action-code recieved.
    toggleDigitalPin(pinMappings[serialPort.actionCode]);
}


/**************************************************************************/
/*!
    @brief  Toggle the specified digital pin.
    @param  pin
            number of the digital pin to activate.  
    @return void
*/
/**************************************************************************/
void toggleDigitalPin(const uint8_t &pin) {
    int state = digitalRead(pin);
    digitalWrite(pin, !state);
}


