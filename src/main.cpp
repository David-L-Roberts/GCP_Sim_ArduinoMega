#include <Arduino.h>
#include "SerialPort.h"
#include "PinMappings.h"
#include "OutputStateMachine.h"

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

int switch_time = DEFAULT_WAIT_TIME;
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
            switch_time = serialPort.actionCode * SWITCH_T_MULT;
            if (switch_time < SWITCH_T_MIN) switch_time = SWITCH_T_MIN;
            switch_t_flag = false;
            Serial.println("Updating Switching time to: " + String(switch_time) + " ms");
        }
        else if (serialPort.actionCode == CHANGE_SWITCH_T) {
            switch_t_flag = true;
        }
        else if (serialPort.actionCode == HMI_HELLO) {
            Serial.print('<' + String(HMI_ACK) + '>');
        } 
        else if (serialPort.actionCode < NUM_OUTPUTS) {  // relay action code
            processRelayActionCode(serialPort, pinMappings);
        }
        else {
            outputSM.changeCylceMode(serialPort.actionCode);
        }

        // clear the action-code
        serialPort.actionCode = NO_CODE;
    }

    // increment state machine
    outputSM.nextState();

    // temp
    delay(switch_time);
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


