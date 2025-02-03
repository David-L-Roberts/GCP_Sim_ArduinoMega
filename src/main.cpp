#include <Arduino.h>
#include "SerialPort.h"
#include "PinMappings.h"
#include "OutputStateMachine.h"

// ==================================================
//                 Function Prototypes
// ==================================================

void setupOutputPinModes(const uint8_t *pinMappings);
void processRelayActionCode(SerialPort &serialPort, const uint8_t *pinMappings);
void toggleDigitalPin(const uint8_t &pin, const uint16_t &time);


// ==================================================
//                      SETUP
// ==================================================

#define BAUD_RATE 9600
#define WAIT_TIME 500       // in milliseconds
SerialPort serialPort = SerialPort();   // Custom Serial Port object
OutputStateMachine outputSM = OutputStateMachine();


// ==================================================
//                      Main Loop
// ==================================================

void setup() {
    // setup pins
    setupOutputPinModes(pinMappings);

    // begin serial
    Serial.begin(BAUD_RATE);
}

void loop() {
    // read data from the serial
    serialPort.readFromSerial();

    // if an action code was recieved, process it
    if (serialPort.actionCode != NO_CODE) {
        if (serialPort.actionCode < 100) {  // relay action code
            processRelayActionCode(serialPort, pinMappings);
        }
        else {  // start, stop, reset action codes
            // temp
            switch (serialPort.actionCode)
            {
            case DECREASE_EZ:
                outputSM.changeCylceMode(DECREASE_EZ);
                Serial.println("Mode changed: DECREASE_EZ");
                break;
            case INCREASE_EZ:
                outputSM.changeCylceMode(INCREASE_EZ);
                Serial.println("Mode changed: INCREASE_EZ");
                break;
            case RESET_HIGH_EX:
                outputSM.changeCylceMode(RESET_HIGH_EX);
                Serial.println("Mode changed: RESET_HIGH_EX");
                break;
            case RESET_LOW_EX:
                outputSM.changeCylceMode(RESET_LOW_EX);
                Serial.println("Mode changed: RESET_LOW_EX");
                break;
            case MANUAL:
                outputSM.changeCylceMode(MANUAL);
                break;
                Serial.println("Mode changed: MANUAL");
            case IDLE:
                outputSM.changeCylceMode(IDLE);
                break;
                Serial.println("Mode changed: IDLE");
            
            default:
                Serial.println("[ERROR] invalid: serialPort.actionCode");
                Serial.println(serialPort.actionCode);
                break;
            }
        }

        // clear the action-code
        serialPort.actionCode = NO_CODE;
    }

    // increment state machine
    outputSM.nextState();

    // temp
    delay(WAIT_TIME);
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
    toggleDigitalPin(pinMappings[serialPort.actionCode], WAIT_TIME);
}


/**************************************************************************/
/*!
    @brief  Toggle the specified digital pin.
    @param  pin
            number of the digital pin to activate.  
    @return void
*/
/**************************************************************************/
void toggleDigitalPin(const uint8_t &pin, const uint16_t &time) {
    int state = digitalRead(pin);
    digitalWrite(pin, ~state);
}
