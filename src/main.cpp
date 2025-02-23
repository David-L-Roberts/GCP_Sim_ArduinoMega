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
#define DEFAULT_WAIT_TIME 600       // in milliseconds

SerialPort serialPort = SerialPort();   // Custom Serial Port object
OutputStateMachine outputSM = OutputStateMachine();

uint16_t switchTime = DEFAULT_WAIT_TIME;
uint16_t switchTimeAdjusted= switchTime;
bool switch_t_flag = false;

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
            Serial.println("Updating Switching time to: " + String(switchTime) + " ms");
        }
        else if (serialPort.actionCode == CHANGE_SWITCH_T) {
            switch_t_flag = true;
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

    // ==================================================
    //                DYNAMIC SWITCHING
    // ==================================================

    // Temp
    int stateNum = outputSM.getCurrentStateNum();
    // int timeAdjust = floor(0.001068 * ((float)stateNum * (float)stateNum) * (float)switchTime * 0.5/100);
    // int timeAdjust = floor(0.001068 * pow(stateNum, 2) * (float)switchTime * 0.5/100);
    int timeAdjust = floor( (0.7*pow(3, 0.01475*(float)stateNum))/100 * (float)switchTime * 0.8);
    
    // Serial.print(stateNum);
    // Serial.print(" :: ");
    // Serial.println(timeAdjust);



    // increment state machine
    switchTimeAdjusted = switchTime - timeAdjust;
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


