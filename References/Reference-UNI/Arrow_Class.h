/*!
    @file
        contains two classes.
            1. ArrowClass is a parent class that is used to draw a arrow on an
            OLED display. The angle of the arrow can be altered, and as well as
            its size. 
            The arrow is drawn as a solid shape

            2. MovingArrowClass inherits from the ArrowClass, and adds the functionality
            of movement across the OLED display. The speed of the moving arrow can be
            altered.
            The arrow is drawn as an outline.

*/

// inclusion gard
#ifndef ARROW_CLASS_H
#define ARROW_CLASS_H

#include <Arduino.h>
// allows you to communicate with I2C / TWI devices
#include <Wire.h>
// for OLED (AdaFruit SSD)
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define NUM_VERTICES 7      // number of Vertices of arrow
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels

#define MAX_SCALE 0.8       // 80%
#define MIN_SCALE 0.2       // 20%
#define UPPER_LIM 19.5      // magnitude limit
#define LOWER_LIM 3.0       // magnitude limit

#define MAX_SPEED 7         // speed limit
#define MIN_SPEED 1         // speed limit

#define X_LIM 100           // translation limit
#define Y_LIM 65            // translation limit

const float g = 9.81;       // gravitational acceleration
#define ACCEL_LIM 0.2*g     // acceleration limit

/**************************************************************************/
/*!
    @brief  class for a rotating arrow, with angle based upon acceleration
            in x and y axis.
*/
/**************************************************************************/
class ArrowClass {
protected:
    // original coordinates of arrow
    const int originalVertices[NUM_VERTICES][2] = 
    { // Rectangle piece {x, y}
        { 8,  12 },      // Top Right 
        { 8, -26 },      // Bottom Right
        {-8, -26 },      // Bottom Left
        {-8,  12 },      // Top Left
        // arrow head
        {-16, 10 },      // left triangle point
        {  0, 26 },      // middle triagle point
        { 16, 10 }       // right triangle point
    };
    // holds the transformed vertices that are used to draw arrow
    int newVertices[NUM_VERTICES][2];  // [vertex number][x,y]

    Adafruit_SSD1306 *ptrDisplay;   // pointer to Adafruit SSD1306 object
    float theta = 0;        // angle of arrow (anti-clockwise)
    float magnitude = 0;    // magnitude of acceleration
    uint16_t size;          // size of arrow
    float xOrigin = SCREEN_WIDTH/2;     // x-coord for centre of arrow
    float yOrigin = SCREEN_HEIGHT/2;    // y-coord for centre of arrow
   
    void set_magnitude(float &xAccel, float &yAccel);
    void set_direction(float &xAccel, float &yAccel);
    void rotate_arrow();
    void translate_arrow();
    void set_size();          
    
private:
    void draw_arrow();
    void draw_pause();

public:
    ArrowClass(Adafruit_SSD1306 &Display);
    void set_attributes(float &xAccel, float &yAccel);
    void draw();

};


/**************************************************************************/
/*!
    @brief  class for arrows that move across the screen in a specified direction
*/
/**************************************************************************/
class MovingArrowClass:public ArrowClass {
private:
    float speed = 1;        // speed of moving arrows

public:
    MovingArrowClass(Adafruit_SSD1306 &Display);
    void set_speed();
    void draw();
    void move();

};




#endif  // ARROW_CLASS_H