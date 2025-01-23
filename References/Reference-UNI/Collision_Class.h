/*!
    @file   
        contains a class that represents distance detection by displaying a wall
        on the OLED screen. 
        The wall is centred around the middle of the display, where its radial distance can be altered.
        Direction of the wall can be set, the angle of the wall is adjusted to always face the centre
        of the display.
*/

// inclusion gard
#ifndef COLLISION_CLASS_H
#define COLLISION_CLASS_H


#include <Arduino.h>
// allows you to communicate with I2C / TWI devices
#include <Wire.h>
// for OLED (AdaFruit SSD)
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define NUM_VERTICES 4      // number of Vertices of arrow
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels

#define ACCEL_LIM 0.2*9.81  // acceleration limit

#define DIST_MIN 15.0       // minimum distance for wall (pixels)
#define DIST_MAX 90.0       // maximum distance for wall (pixels)
#define SENS_MIN 45.0       // sensor reading for min display distance (cm)
#define SENS_MAX 350.0      // sensor reading for max display distance (cm)


/**************************************************************************/
/*!
    @brief  class that stores info for drawing a wall on an OLED display
            to represent collision detection.
*/
/**************************************************************************/
class CollisionClass {
private:
    // original coordinates of wall
    const int originalVertices[NUM_VERTICES][2] = 
    { // {x, y}
        { 72,  3 },      // Top Right 
        { 72, -3 },      // Bottom Right
        {-72, -3 },      // Bottom Left
        {-72,  3 },      // Top Left

    };
    // holds the transformed vertices that are used to draw arrow
    int newVertices[NUM_VERTICES][2];  // [vertex number][x,y]

    Adafruit_SSD1306 *ptrDisplay;       // pointer to Adafruit SSD1306 object
    float theta = 0;                    // angle of wall
    float xOrigin = SCREEN_WIDTH/2;     // x-coord for centre of wall
    float yOrigin = SCREEN_HEIGHT/2;    // y-coord for centre of wall
    uint16_t distance = 25;             // distance to wall
    bool show = true;                   // should the wall be shown

    void set_direction(float &xAccel, float &yAccel);
    void rotate_wall();
    void translate_wall();

public:
    CollisionClass(Adafruit_SSD1306 &Display);
    void set_attributes(float &xAccel, float &yAccel);
    bool set_distance(float dist);
    void draw();


};


#endif  // COLLISION_CLASS_H