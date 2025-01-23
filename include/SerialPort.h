#pragma once
#include <Arduino.h>

#define MAX_INPUT 4    // max length of input chars read from serial
#define NO_CODE 255     // number used to signify when there is no current action code to execute


/**************************************************************************/
/*!
    @brief  Class for reading action code data packets from serial.
            Data packets to be enclosed with '<' and '>'.
            Data between < and > is to be an integer from 0-254, representing 
            an action code.
*/
/**************************************************************************/
class SerialPort {
private:
    char input_line[MAX_INPUT]; // Hold input chars read from serial
    uint8_t input_pos = 0;      // current index for input_line that should be written to

    bool processIncomingByte(const byte inByte);
    void processData(const char * data);

public:
    uint8_t actionCode = NO_CODE;   // 255 = invalid action code

    void readFromSerial();
};
