#include "Collision_Class.h"

/**************************************************************************/
/*!
    @brief  Constructor
    @param  Display
            address of OLED object
*/
/**************************************************************************/
CollisionClass::CollisionClass(Adafruit_SSD1306 &Display) {
    ptrDisplay = &Display;
}


/**************************************************************************/
/*!
    @brief  sets the attributes of the wall.
            also rotates and translates wall coordinates, ready for drawing.
    @param  xAccel
            acceleration in the x-axis
    @param  yAccel
            acceleration in the y-axis
    @return void
*/
/**************************************************************************/
void CollisionClass::set_attributes(float &xAccel, float &yAccel) {
    // set the angle of the wall
    set_direction(xAccel, yAccel);
    // rotate wall coordinates to the new angle
    rotate_wall();
    // translate the wall to be centred about display
    translate_wall();
}


/**************************************************************************/
/*!
    @brief  sets the attribute 'theta' based on axis acceleration
    @param  xAccel
            acceleration in the x-axis
    @param  yAccel
            acceleration in the y-axis
    @return void
*/
/**************************************************************************/
void CollisionClass::set_direction(float &xAccel, float &yAccel) {
    // angle of triangle formed by x and y acceleration vectors
    theta = atan2(-xAccel, yAccel);
    // don't show the wall when the acceleration is zero
    if (sqrt(sq(xAccel) + sq(yAccel)) < ACCEL_LIM) {
        show = false;
    } else {
        show = true;
    }
}


/**************************************************************************/
/*!
    @brief  rotates wall coordinates to a position that reflects the angle 'theta'.
            also adjusts the distance from origin
    @return void
*/
/**************************************************************************/
void CollisionClass::rotate_wall() {
    // cycle through each vertex, and rotate its coordinates
    for (int i = 0; i < NUM_VERTICES; i++) {
        float xx = originalVertices[i][0];  // get x-coordinate of vertex
        float yy = originalVertices[i][1];  // get y-coordinate of vertex

        yy = yy + distance;     // account for distance from origin

        int new_xx = round(xx*cos(theta) - yy*sin(theta)); // rotate x-coordinate
        int new_yy = round(xx*sin(theta) + yy*cos(theta)); // rotate y-coordinate

        newVertices[i][0] = new_xx;         // reassign x-coordinate
        newVertices[i][1] = new_yy;         // reassign y-coordinate
    }
}


/**************************************************************************/
/*!
    @brief  translates the vertices of the wall to be centred arround the 
            wall's unique origin
    @return Void
*/
/**************************************************************************/
void CollisionClass::translate_wall() {
    // cycle through each vertex, and translate it to the origin
    for (int i = 0; i < NUM_VERTICES; i++) {
        int xx = newVertices[i][0];     // get x-coordinate of vertex
        int yy = newVertices[i][1];     // get y-coordinate of vertex

        int new_xx = xOrigin + xx;      // translate x-coordinate
        int new_yy = yOrigin - yy;      // translate y-coordinate

        newVertices[i][0] = new_xx;     // reassign x-coordinate
        newVertices[i][1] = new_yy;     // reassign y-coordinate
    }
}



/**************************************************************************/
/*!
    @brief  draws a wall oriented based on translated coordinates
    @return Void
*/
/**************************************************************************/
void CollisionClass::draw() {
    if (show) {
        (*ptrDisplay).drawLine( newVertices[0][0], newVertices[0][1], newVertices[3][0], newVertices[3][1], SSD1306_WHITE);
        (*ptrDisplay).drawLine( newVertices[1][0], newVertices[1][1], newVertices[2][0], newVertices[2][1], SSD1306_WHITE);
    }
}


/**************************************************************************/
/*!
    @brief  sets the distance attribute based on a passed in value. 
            Converts the given distance in cm to pixels.
    @param  dist
            a float value that represents the distance from the wall in cm
    @return Void
*/
/**************************************************************************/
bool CollisionClass::set_distance(float dist) {
    bool minDistance = false;   // is the wall at its minimum distance

    if (dist < SENS_MIN) {
        distance = DIST_MIN;
        minDistance = true;

    } else if (dist > SENS_MAX) {
        distance = DIST_MAX;

    } else {
        const float m = (DIST_MAX-DIST_MIN)/(SENS_MAX-SENS_MIN);
        const float c = DIST_MAX - m*SENS_MAX;
        distance = m*dist + c;  // y = mx + c
    }

    return minDistance;
}
