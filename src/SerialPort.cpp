#include "SerialPort.h"

// #define DEBUG

/**************************************************************************/
/*!
    @brief  If serial data is available, read byte stream until a full action
            has been recieved.
            Once an action code has been read, save the code to the [actionCode]
            attribute and exit the function. 

    @return void
*/
/**************************************************************************/
void SerialPort::readFromSerial() {
    bool code_received = false;

    while ((Serial.available() > 0) && (!code_received)) {
        code_received = processIncomingByte(Serial.read());
    }
}


/**************************************************************************/
/*!
    @brief  Process a single byte of data read from the serial.
            If the byte is the last one in the data packet,
            proceed to process the whole packet.
    @param  inByte
            Single Byte of data read from serial.
    @return void
*/
/**************************************************************************/
bool SerialPort::processIncomingByte(const byte inByte) {

    switch (inByte) {
        case '>':   // end of data packet
            input_line[input_pos] = 0;  // terminating null byte
            processData(input_line);   // terminator reached! process input_line here ...
            input_pos = 0;  // reset buffer for next time
            return true;

        case '<':   // start of data packet
            input_pos=0;
            break;

        default:
            // keep adding if not full. allow for terminating null byte
            if (input_pos < (MAX_INPUT - 1))
                input_line[input_pos++] = inByte;
            break;
    }

    return false;
}


/**************************************************************************/
/*!
    @brief  Processes the char-array read from the comm serial port.
            Saves the action code received to object's attribute.
    @param  data
            Char-array read from the serial port.
    @return void
*/
/**************************************************************************/
void SerialPort::processData(const char * data) {
    // Store action code read from serial as attribute
    actionCode = atoi(data);

    #ifdef DEBUG
        // DEBUG: log action code received to Serial 
        Serial.print("[ACTION CODE RECEIVED] :: ");
        Serial.println(actionCode);
    #endif
}