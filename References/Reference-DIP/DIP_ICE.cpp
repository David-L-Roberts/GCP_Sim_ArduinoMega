#include <Arduino.h>
#include <Servo.h>
#include <SerialPort.h>
#include <PinMappings.h>

// ==================================================
//                 Function Prototypes
// ==================================================

void setupPinModes(const uint8_t *pinMappings);
void processSolenoidActionCode(SerialPort &serialPort, const uint8_t *pinMappings);
void toggleDigitalPin(const uint8_t &pin, const uint16_t &time);
void processPowerActionCode(SerialPort &serialPort, const uint8_t *servoAngles);


// ==================================================
//                      SETUP
// ==================================================

#define BAUD_RATE 9600
#define WAIT_TIME 400
SerialPort serialPort = SerialPort();   // Custom Serial Port object
Servo servo;    // servo motor object


// ==================================================
//                      Main Loop
// ==================================================
void setup() {
    // setup pins
    setupPinModes(pinMappings);
    servo.attach(SERVO_PIN);
    // initialize servo angle
    servo.write(SERVO_ON);
    // begin serial
    Serial.begin(BAUD_RATE);
}

void loop() {
    // read data from the serial
    serialPort.readFromSerial();

    // if an action code was recieved, process it
    if (serialPort.actionCode != NO_CODE) {
        if (serialPort.actionCode < 100) {  // Solenoid action code
            processSolenoidActionCode(serialPort, pinMappings);
        } 
        else {  // Power action code
            processPowerActionCode(serialPort, servoAngles);
        }
    }
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
void setupPinModes(const uint8_t *pinMappings) {
    for (int i; i < NUM_OF_PINS; i++){
        pinMode(pinMappings[i], OUTPUT);
    }
}


/**************************************************************************/
/*!
    @brief  Activate the solenoid corresponding to the currently
            received action code.
    @param  SerialPort
            Object for reading data from serial Comm port and processing the data.
    @param  pinMappings
            Array mapping action codes to digital pins used for solenoids.
    @return void
*/
/**************************************************************************/
void processSolenoidActionCode(SerialPort &serialPort, const uint8_t *pinMappings) {
    // activate the digital pin that corrsponds to the action-code recieved.
    toggleDigitalPin(pinMappings[serialPort.actionCode], WAIT_TIME);
    // clear the action-code
    serialPort.actionCode = NO_CODE;
}


/**************************************************************************/
/*!
    @brief  Set the given digital pin HIGH for the specified amount of time.
            Then set it back to LOW.
    @param  pin
            number of the digital pin to activate.  
    @param  time
            time to activate the pin for (in milliseconds).
    @return void
*/
/**************************************************************************/
void toggleDigitalPin(const uint8_t &pin, const uint16_t &time) {
    digitalWrite(pin, HIGH);
    delay(time);
    digitalWrite(pin, LOW);
}


/**************************************************************************/
/*!
    @brief  Set the servo motor to the angle corresponding to the received
            action code.
    @param  SerialPort
            Object for reading data from serial Comm port and processing the data.
    @param  ServoAngles
            array mapping action codes to servo angles
    @return void
*/
/**************************************************************************/
void processPowerActionCode(SerialPort &serialPort, const uint8_t *servoAngles) {
    // set the servo to the angle corrsponding to the recieved action code received.
    // servo action codes are in the 100-series. Require subtraction of '100' to use.
    servo.write(servoAngles[serialPort.actionCode - 100]);

    // reset the action-code
    serialPort.actionCode = NO_CODE;
}