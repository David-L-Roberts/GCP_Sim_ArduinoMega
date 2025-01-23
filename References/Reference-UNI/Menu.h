/*!
    @file
        functions to generate and interact with a menu. Also provides password functions.
        Password functions prompt for input on start-up. Default password is:
            --> EEE20003
        menu has 3 options:
            1. store IMU reading into EEPROM
            2. store Distance sensor into EEPROM
            3. enter main program (loop() function)
*/

// inclusion gard
#ifndef MENU_H      
#define MENU_H

#include <Arduino.h>
// for OLED (AdaFruit SSD)
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// for IMU
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "EEPROM.h"
#include "Sensors.h"


void ask_password(Adafruit_SSD1306 &display);
String get_password(Adafruit_SSD1306 &display);
void store_password_EEPROM(String text);
void store_floatData_EEPROM(float data, int addr);
String read_from_EEPROM(int address, int data_length);
void store_data_EEPROM(String text);
void get_menu(Adafruit_MPU6050 &mpu, Adafruit_SSD1306 &display);
void oled_write(Adafruit_SSD1306 &display, String text);
void option1(Adafruit_MPU6050 &mpu, Adafruit_SSD1306 &display);
void option2(Adafruit_SSD1306 &display);
void invalid_option(Adafruit_SSD1306 &display);
void serialFlush();

/******* EEPROM ADRESSES *******/
int password_addr = 50; //address to store password
#define X_ACCEL_ADDR 0 //address for x acceleration
#define Y_ACCEL_ADDR 4 //address for y acceleration
#define DIST_ADDR    8 //address for distance data


/*--------------------------PASSWORD FUNCTIONS------------------------------*/

/**************************************************************************/
/*!
    @brief  retrieves user input from Serial, compares to password stored in EEPROM, holds if false
    @param  display
            address of OLED display object
    @return Void
*/
/**************************************************************************/
void ask_password(Adafruit_SSD1306 &display)
{
    String pass_correct = read_from_EEPROM(password_addr, 9); // read the correct password from EEPROM, length 9
    String pass_in = get_password(display);                   // get user input

    while (pass_in != pass_correct)
    { // retake password until correct is entered
        oled_write(display, "Password Failed");
        delay(1000); // give user time to read
        pass_in = get_password(display);
    }
}

/**************************************************************************/
/*!
    @brief  get password from serial and hold until correct
    @param  display
            address of OLED display object
    @return string. holds password retrieved from serial
*/
/**************************************************************************/
String get_password(Adafruit_SSD1306 &display)
{
    // only allow continuation if correct password entered
    String input = ""; // initialise input string

    oled_write(display, "Enter Password");
    serialFlush();
    delay(50); // allow the Serial time to flush

    while (Serial.available() == 0)
    {
        // do nothing
    }
    while (Serial.available() > 0)
    {
        input += Serial.readString(); // put all characters written into input string
    }

    int null = input.length() - 1; // index address of null character, -1 since 0 indexed
    input.remove(null);            // remove the null character from string

    return input;
}

/**************************************************************************/
/*!
    @brief  store the password string into EEPROM memory
    @param  text
            String describing the correct password
    @return Void
*/
/**************************************************************************/
void store_password_EEPROM(String text)
{
    // store length of text
    int length = text.length();

    // write the string to memory
    for (int i = 0; i < length; i++)
    {
        EEPROM.update(password_addr, text[i]);
        password_addr += 1;
    }
    password_addr = 50; // return password address to original value
}

/*--------------------------EEPROM FUNCTIONS------------------------------*/

/**************************************************************************/
/*!
    @brief  store a float into EEPROM memory
    @param  data
            float to be stored, addr, integer for address in EEPROM
    @param  addr
            EEPROM address to be written to
    @return Void
*/
/**************************************************************************/
void store_floatData_EEPROM(float data, int addr)
{
    // write the data to memory
    EEPROM.update(addr, data);
}

/**************************************************************************/
/*!
    @brief  reads data from EEPROM and stores it in a String
    @param  address
            initial position integer of desired data
    @param  data_length
            the size of data being retrieved
    @return String holding data from EEPROM
*/
/**************************************************************************/
String read_from_EEPROM(int address, int data_length)
{
    String output = ""; // initialise output string
    for (int i = address; i < address + data_length; i++)
    {
        output += (char)EEPROM.read(i);
    }
    int null = output.length() - 1; // String address of null character
    output.remove(null);            // remove the null character
    return output;
}

/*--------------------------MENU FUNCTIONS------------------------------*/

/**************************************************************************/
/*!
    @brief  presents a menu on OLED screen to user and takes a value for
            respective options
    @param  mpu
            address of MPU object
    @param  display
            address of OLED display object        
    @return Void
*/
/**************************************************************************/
void get_menu(Adafruit_MPU6050 &mpu, Adafruit_SSD1306 &display)
{
    while (true)
    {
        // make three functions for three options
        oled_write(display, "Enter 1, 2 or 3 for  the following actions");
        delay(2000); // read time

        // write options on oled
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("1: Write position    into EEPROM"); // large space for new line
        display.println();                                   // extra line for readability
        display.println("2: Write sensor data into EEPROM");
        display.println();
        display.println("3: Start driving");
        display.display();

        int option = 0;
        serialFlush(); // flush the serial
        delay(50);     // give Serial time to flush
        while (Serial.available() == 0)
        {
            // do nothing
        }
        while (Serial.available() > 0)
        {
            option = Serial.read() - '0'; // take input, convert from ASCII
        }

        Serial.print("Option: ");
        Serial.println(option);

        switch (option)
        {
        case 1:
            option1(mpu, display); // calls option 1
            break;
        case 2:
            option2(display); // calls option 2
            break;
        case 3:
            // continues to interrupt driven-program
            return;
            break;
        default:
            invalid_option(display);
            break;
        }
    }
}

/**************************************************************************/
/*!
    @brief  Writes a message onto the OLED
    @param  display
            address of OLED display object   
    @param  text
            String for desired message
    @return Void
*/
/**************************************************************************/
void oled_write(Adafruit_SSD1306 &display, String text)
{
    display.clearDisplay(); // remove anything on display

    display.setTextSize(1);      // text size
    display.setTextColor(WHITE); // use white colour
    display.setCursor(0, 28);    // texts starts from left, halhway down
    display.println(text);       // print the text

    display.display(); // display new display
}

/**************************************************************************/
/*!
    @brief  stores mpu data into EEPROM and then prints from EEPROM to serial
            to confirm success
    @param  mpu
            address of MPU object
    @param  display
            address of OLED display object      
    @return Void
*/
/**************************************************************************/
void option1(Adafruit_MPU6050 &mpu, Adafruit_SSD1306 &display)
{
    serialFlush(); // remove 1 from Serial
    // store tilt info
    float xAccel = 0;
    float yAccel = 0;

    get_tilt_info(mpu, xAccel, yAccel);
    // store acceleration in EEPROM
    EEPROM.put(X_ACCEL_ADDR, xAccel); 
    EEPROM.put(Y_ACCEL_ADDR, yAccel);
    // OLED message
    oled_write(display, "IMU data Stored");
    
    float data = 0;     // hold EEPROM data
    EEPROM.get(X_ACCEL_ADDR, data);

    // print x accel on Serial from EEPROM
    Serial.print("x acceleration from EEPROM: ");
    Serial.print(data); 
    Serial.print(" m/s\n");

    EEPROM.get(Y_ACCEL_ADDR, data);

    // print y accel on Serial from EEPROM
    Serial.print("y acceleration from EEPROM: ");
    Serial.print(data); 
    Serial.print(" m/s\n");

    delay(2000);    // read time

    return;
}

/**************************************************************************/
/*!
    @brief  Writes ultrasonic distance sensor data into EEPROM, then prints
            from EEPROM to serial to confirm success
    @param  display
            address of OLED display object    
    @return Void
*/
/**************************************************************************/
void option2(Adafruit_SSD1306 &display) {
    serialFlush(); //remove 2 from Serial

    // store uds info
    float dist_data = read_UDS(); //get data from distance sensor
    EEPROM.put(DIST_ADDR, dist_data); //put in EEPROM
    // OLED message
    oled_write(display, "Distance Data Stored");

    float data = 0;
    EEPROM.get(DIST_ADDR, data);

    // print distance on Serial from EEPROM
    Serial.print("Distance from EEPROM: ");
    Serial.print(data); 
    Serial.print(" cm\n");

    delay(2000); //read time

    return;
}

/**************************************************************************/
/*!
    @brief  Displays invalid option message on OLED when anything other than
            1, 2 or 3 is entered
    @param  display
            address of OLED display object    
    @return Void
*/
/**************************************************************************/
void invalid_option(Adafruit_SSD1306 &display)
{
    serialFlush(); // remove the invalid option from Serial
    oled_write(display, "Invalid Option");
    delay(2000); // read time

    return;
}

/*--------------------------MISC------------------------------*/

/**************************************************************************/
/*!
    @brief  flush the serial input buffer
    @return Void
*/
/**************************************************************************/
void serialFlush()
{
    delay(50); // wait for extra bytes to arrive
    while (Serial.available() > 0)
    {
        char t = Serial.read();
    }
}


#endif // MENU_H