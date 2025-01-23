/*!
    @file
        Project C
        Displays Acceleration info from an IMU sensor on an OLED display.
        Draws an arrow in the direction the IMU is tilting, and draws moving
        arrows in that direction as well.

        A distance sensor adds collision detection to the program, where a wall
        will be displayed if the measured distance is small. The wall will get closer
        to the center of the display as the measured distance decreases, representing
        an appoaching obstacle.

        Before the program enters its main functionalty, it will require the user to enter
        a password that matches the one stored in EEPROM. Next, the user will be greeted with
        a menu, where they can either enter the main routine, or first store sensor 
        data in EEPROM.

*/

#include <Arduino.h>
// allows you to communicate with I2C / TWI devices
#include <Wire.h>
// for OLED (AdaFruit SSD)
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// for IMU
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
// custom arrow class
#include "Arrow_Class.h" 
// custom class for collision detection
#include "Collision_Class.h"
// custom functions for sensors
#include "Sensors.h"
// menu functions
#include "Menu.h"


/** used to enable or disable the menu **/
#define MENU true

/******* OBJECT DECLERATIONS *******/

// for SSD1306 declaration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin #

// Declaration for SSD1306 OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// create an 'mpu' object
Adafruit_MPU6050 mpu;
// create an arrow object
ArrowClass arrow = ArrowClass(display);
// create an object for a moving arrow
MovingArrowClass movingArrow = MovingArrowClass(display);
// create an object for a wall
CollisionClass wall = CollisionClass(display);

#define ssdAddress 0x3C   // I2C address of the SSD1306


/******* TIMER 1 FLAGS *******/
volatile uint16_t timerMove = 0;    // flag for moving arrow
volatile uint16_t timerDraw = 0;    // flag for drawing

/******* TIMER 3 FLAG *******/
volatile bool timerSensors = false;    // flag for sensor measurement timer

/******* BUTTON INTERRUPT *******/
#define Button_1 18  // declare button pin
volatile bool stop = false; // declare emergency stop flag


/*--------------------------SETUP & LOOP------------------------------*/

void setup() {
    Serial.begin(115200);

    Wire.begin();       // initialize as I2C master
    begin_UDS();        // set up distance sensor
    begin_OLED();       // try to connect to the OLED display
    begin_IMU(mpu);     // try to connect to IMU

    draw_logo();
    
    // Menu functions
    #if MENU
        store_password_EEPROM("EEE20003");  //store string in EEPROM for password
        ask_password(display);              //ask for password
        get_menu(mpu, display);             //display the menu
    #endif

    // timer 1 manages timing for arrows and display updates
    begin_timer1_interrupt(1000);  // Period = 1 ms
    // timer 3 managaes timing for sensor measurement updates
    begin_timer3_interrupt(200);  // Period = 5 ms

    // enable hardware interupt using button
    pinMode(Button_1, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Button_1), toggleStop, FALLING); 
}


void loop() {

    // updates sensor info
    if (timerSensors == true) { //only active when interrupt timer is complete
        // store tilt info
        float xAccel = 0;
        float yAccel = 0;

        // distance sensor measurement
        float distance = read_UDS();
        bool collision = false;   // has wall collided with arrow
        collision = wall.set_distance(distance);

        if (stop == false) {
            // read acceleration info from IMU of x-axis and y-axis
            get_tilt_info(mpu, xAccel, yAccel);
        }

        // update object parameters based on new acceleration
        arrow.set_attributes(xAccel, yAccel);
        wall.set_attributes(xAccel, yAccel);

        if (collision) {
            // remove the moving arrow if there is a collision
            float zero = 0;
            movingArrow.set_attributes(zero, zero);
        } else {
            movingArrow.set_attributes(xAccel, yAccel);
        }

        // reset timer flag
        timerSensors = false;
    }
    
    // drawing commands
    if (timerDraw >= 10) {  // update every 10 milliseconds
        display.clearDisplay();

        arrow.draw();
        movingArrow.draw();
        wall.draw();

        display.display();
        timerDraw = 0;
    }

    // move arrow
    if (timerMove >= 30) {  // update every 30 milliseconds
        movingArrow.set_speed();
        movingArrow.move();
        timerMove = 0;
    }
}


/*--------------------------FUNCTION DEFINITIONS------------------------------*/

/**************************************************************************/
/*!
    @brief  Attempt to connect to the SSD1306 OLED display.
            Will print a message to terminal if connection is a success 
            or a failure.
*/
/**************************************************************************/
void begin_OLED() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, ssdAddress)) {  // default address is 0x3C
        Serial.println(F("SSD1306 connection failed"));
        while(1);   // SSD1306 did not connect, loop forever
    }
    // device connected successfully
    Serial.println("SSD1306 Connected!");   
}


/**************************************************************************/
/*!
    @brief  set up timer interrupt using timer 1 at the given frequency. Uses
            a prescaler of 1024.
    @param  frequency
            frequency of timer interrupt. minimum value = 0.24 Hz
    @return void
*/
/**************************************************************************/
void begin_timer1_interrupt(float frequency) {
    // compare-match-register-value
    long compareMatch = (16000000)/(1024*frequency) - 1;

    noInterrupts(); // disable interrupts

    // use timer 1 for interrupts
    TCCR1A = 0; // set all bits in TCCR1A to 0
    TCCR1B = 0; // set all bits in TCCR1B to 0
    TCNT1  = 0; // initialize counter value to 0
    // set the compare match register
    OCR1A = compareMatch;  // prescaler = 1024
    // turn on CTC
    TCCR1B |= (1 << WGM12);
    // set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // enable timer 1 as compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    interrupts();   // reactivate interrupts
}


/**************************************************************************/
/*!
    @brief  Increment timer 1 flags
*/
/**************************************************************************/
ISR(TIMER1_COMPA_vect) {
    timerMove++;
    timerDraw++;
}


/**************************************************************************/
/*!
    @brief  set up timer interrupt using timer 3 at the given frequency. Uses
            a prescaler of 1024.
    @param  frequency
            frequency of timer interrupt. minimum value = 0.24 Hz
    @return void
*/
/**************************************************************************/
void begin_timer3_interrupt(float frequency) {
    // compare-match-register-value
    long compareMatch = (16000000)/(1024*frequency) - 1;

    noInterrupts(); // disable interrupts

    // use timer 3 for interrupts
    TCCR3A = 0; // set all bits in TCCR1A to 0
    TCCR3B = 0; // set all bits in TCCR1B to 0
    TCNT3  = 0; // initialize counter value to 0
    // set the compare match register
    OCR3A = compareMatch;  // prescaler = 1024
    // turn on CTC
    TCCR3B |= (1 << WGM32);
    // set prescaler to 1024
    TCCR3B |= (1 << CS32) | (1 << CS30);
    // enable timer 1 as compare interrupt
    TIMSK3 |= (1 << OCIE3A);

    interrupts();   // reactivate interrupts
}


/**************************************************************************/
/*!
    @brief  Increment timer 3 flag
*/
/**************************************************************************/
ISR(TIMER3_COMPA_vect) {
    timerSensors = true;
}


/**************************************************************************/
/*!
    @brief  Toggle emergency stop flag
    @return void
*/
/**************************************************************************/
void toggleStop() {
    stop = !stop;   //toggle stop flag state

    // display status of emergency stop
    if (stop == true) {
        Serial.println("Emergency Stop button pressed!");
    }
    else {
        Serial.println("Stop disabled.");
    }
}


/**************************************************************************/
/*!
    @brief  Shows Adafruit logo on screen during set up
*/
/**************************************************************************/
void draw_logo() {
    // display logo
    display.display();
    delay(1000);
    // Clear the buffer
    display.clearDisplay();
}
