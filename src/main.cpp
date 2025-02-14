#include <Arduino.h>
#include "SerialPort.h"
#include "PinMappings.h"
#include "OutputStateMachine.h"

#define DEBUG

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

int wait_time = DEFAULT_WAIT_TIME;


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
        if (serialPort.actionCode == HMI_HELLO) {
            Serial.print('<' + HMI_ACK + '>');
        } 
        else if (serialPort.actionCode < 100) {  // relay action code
            processRelayActionCode(serialPort, pinMappings);
        }
        else {
            switch (serialPort.actionCode)
            {
            case DECREASE_EZ:
                outputSM.changeCylceMode(DECREASE_EZ);
                #ifdef DEBUG 
                    Serial.println("Mode changed: DECREASE_EZ");
                #endif
                break;

            case INCREASE_EZ:
                outputSM.changeCylceMode(INCREASE_EZ);
                #ifdef DEBUG 
                    Serial.println("Mode changed: INCREASE_EZ");
                #endif
                break;

            case RESET_HIGH_EZ:
                outputSM.changeCylceMode(RESET_HIGH_EZ);
                #ifdef DEBUG 
                    Serial.println("Mode changed: RESET_HIGH_EZ");
                #endif
                break;

            case RESET_LOW_EZ:
                outputSM.changeCylceMode(RESET_LOW_EZ);
                #ifdef DEBUG 
                    Serial.println("Mode changed: RESET_LOW_EZ");
                #endif
                break;

            case MANUAL:
                outputSM.changeCylceMode(MANUAL);
                #ifdef DEBUG 
                    Serial.println("Mode changed: MANUAL");
                #endif
                break;
                
            case IDLE:
                outputSM.changeCylceMode(IDLE);
                #ifdef DEBUG 
                    Serial.println("Mode changed: IDLE");
                #endif
                break;
            
            default:
                #ifdef DEBUG 
                    Serial.println("[ERROR] invalid: serialPort.actionCode");
                    Serial.println(serialPort.actionCode);
                #endif
                break;
            }
        }

        // clear the action-code
        serialPort.actionCode = NO_CODE;
    }

    // increment state machine
    outputSM.nextState();

    // temp
    delay(wait_time);
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


