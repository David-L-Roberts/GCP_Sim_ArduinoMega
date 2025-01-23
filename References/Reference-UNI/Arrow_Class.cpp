#include "Arrow_Class.h"

/**************************************************************************/
/*!
    @brief  Constructor
    @param  Display
            address of OLED object
*/
/**************************************************************************/
ArrowClass::ArrowClass(Adafruit_SSD1306 &Display) {
    ptrDisplay = &Display;
}


/**************************************************************************/
/*!
    @brief  sets the attributes 'theta', 'magnitude', and 'size' of the arrow
    @param  xAccel
            acceleration in the x-axis
    @param  yAccel
            acceleration in the y-axis
    @return void
*/
/**************************************************************************/
void ArrowClass::set_attributes(float &xAccel, float &yAccel) {
    // set the magnitude of the arrow
    set_magnitude(xAccel, yAccel);
    // set the angle of the arrow
    set_direction(xAccel, yAccel);
    // rotate arrow coordinates to the new angle
    rotate_arrow();
    // change the size of the arrow
    set_size();
    // translate the arrow
    translate_arrow();
}


/**************************************************************************/
/*!
    @brief  sets the 'magnitude' attribute of the arrow
    @param  xAccel
            acceleration in the x-axis
    @param  yAccel
            acceleration in the y-axis
    @return void
*/
/**************************************************************************/
void ArrowClass::set_magnitude(float &xAccel, float &yAccel) {
    // magnitude of acceleration
    magnitude = sqrt(sq(xAccel) + sq(yAccel));
}


/**************************************************************************/
/*!
    @brief  sets the direction of the arrow by changing the attribute 'theta'.
    @param  xAccel
            acceleration in the x-axis
    @param  yAccel
            acceleration in the y-axis
    @return void
*/
/**************************************************************************/
void ArrowClass::set_direction(float &xAccel, float &yAccel) {
    // angle of triangle formed by x and y acceleration vectors
    theta = atan2(-xAccel, yAccel);

    if (magnitude < ACCEL_LIM) {
        theta = 0;
    }
}


/**************************************************************************/
/*!
    @brief  rotates arrow coordinates to a position that reflects the angle 'theta'
    @return void
*/
/**************************************************************************/
void ArrowClass::rotate_arrow() {
    // cycle through each vertex, and rotate its coordinates
    for (int i = 0; i < NUM_VERTICES; i++) {
        float xx = originalVertices[i][0];  // get x-coordinate of vertex
        float yy = originalVertices[i][1];  // get y-coordinate of vertex

        int new_xx = round(xx*cos(theta) - yy*sin(theta)); // rotate x-coordinate
        int new_yy = round(xx*sin(theta) + yy*cos(theta)); // rotate y-coordinate

        newVertices[i][0] = new_xx;         // reassign x-coordinate
        newVertices[i][1] = new_yy;         // reassign y-coordinate
    }
}


/**************************************************************************/
/*!
    @brief  scales the vertices of the arrow to change its size according to
            the magnitude attribute.
    @return void
*/
/**************************************************************************/
void ArrowClass::set_size() {
    // calculate the scaling factor based on magnitude
    float scaleFactor = 0;
    if (magnitude < LOWER_LIM) {
        scaleFactor = MIN_SCALE;
    } else if (magnitude > UPPER_LIM) {
        scaleFactor = MAX_SCALE;
    } else {
        const float m = (MAX_SCALE-MIN_SCALE)/(UPPER_LIM-LOWER_LIM);
        const float c = MAX_SCALE - m*UPPER_LIM;
        scaleFactor = m*magnitude + c;  // y = mx + c
    }
    
    // cycle through each vertex, and scale the coordinates
    for (int i = 0; i < NUM_VERTICES; i++) {
        int xx = newVertices[i][0];     // get x-coordinate of vertex
        int yy = newVertices[i][1];     // get y-coordinate of vertex

        int new_xx = scaleFactor * xx;  // scale x-coordinate
        int new_yy = scaleFactor * yy;  // scale y-coordinate

        newVertices[i][0] = new_xx;     // reassign x-coordinate
        newVertices[i][1] = new_yy;     // reassign y-coordinate
    }
}


/**************************************************************************/
/*!
    @brief  translates the vertices of the arrow to be centred arround the 
            arrow's unique origin
    @return Void
*/
/**************************************************************************/
void ArrowClass::translate_arrow() {
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
    @brief  draws an arrow pointing in the direction specified by the attribute 'theta'
    @return Void
*/
/**************************************************************************/
void ArrowClass::draw_arrow() {
    // draw 3-triangles to form an arrow. traingles are based on vertices stored in 'line' array.
    (*ptrDisplay).fillTriangle(newVertices[0][0], newVertices[0][1], newVertices[1][0], 
            newVertices[1][1], newVertices[3][0], newVertices[3][1], WHITE);    // bottom left rectangle

    (*ptrDisplay).fillTriangle(newVertices[1][0], newVertices[1][1], newVertices[2][0], 
            newVertices[2][1], newVertices[3][0], newVertices[3][1], WHITE);    // top right rectangle

    (*ptrDisplay).fillTriangle(newVertices[4][0], newVertices[4][1], newVertices[5][0], 
            newVertices[5][1], newVertices[6][0], newVertices[6][1], WHITE);    // arrow head   
}


/**************************************************************************/
/*!
    @brief  draws a circle as a pause symbol
    @return Void
*/
/**************************************************************************/
void ArrowClass::draw_pause() {
    (*ptrDisplay).drawCircle(xOrigin, yOrigin, 12, WHITE);
}


/**************************************************************************/
/*!
    @brief  draws either an arrow or pause symbol depending on magnitude of
            acceleration.
    @return Void
*/
/**************************************************************************/
void ArrowClass::draw() {
    if (magnitude < ACCEL_LIM) {
        draw_pause();
    } else {
        draw_arrow();
    }
}


// --------------------------MOVING ARROW------------------------------


/**************************************************************************/
/*!
    @brief  Constructor
    @param  Display
            address of OLED object
    @param  xAccel
            acceleration in the x-axis
    @param  yAccel
            acceleration in the y-axis
*/
/**************************************************************************/
MovingArrowClass::MovingArrowClass(Adafruit_SSD1306 &Display)
    :ArrowClass(Display) 
{
}


/**************************************************************************/
/*!
    @brief  sets the movement speed of the arrow based on magnitude attribute
    @return Void
*/
/**************************************************************************/
void MovingArrowClass::set_speed() {
    if (magnitude < LOWER_LIM) {
        speed = 0;
    } else if (magnitude > UPPER_LIM) {
        speed = MAX_SPEED;
    } else {
        const float m = (MAX_SPEED - MIN_SPEED)/(UPPER_LIM - LOWER_LIM);
        const float c = MAX_SPEED - m*UPPER_LIM;
        speed = m*magnitude + c;  // y = mx + c
    }
    
}


/**************************************************************************/
/*!
    @brief  draws an arrow pointing in the direction specified by the 
            attribute 'theta'. arrow is a silhouette.
    @return Void
*/
/**************************************************************************/
void MovingArrowClass::draw() {
    // don't draw arrow if magnitude is below threshold
    if (magnitude < LOWER_LIM) {
        return;
    }

    bool done = false;
    int i = 0;
    while (!done) {
        // draw lines between the vertices
        (*ptrDisplay).drawLine( newVertices[i][0], newVertices[i][1],
            newVertices[i+1][0], newVertices[i+1][1], SSD1306_WHITE);

        i++;

        // special case, for connecting last vertex back to the starting vertex
        if (i == NUM_VERTICES-1) {
            // connect last vertex to first
            (*ptrDisplay).drawLine( newVertices[i][0], newVertices[i][1],
                newVertices[0][0], newVertices[0][1], SSD1306_WHITE);
            // exit loop
            done = true;
        }
    }
}


/**************************************************************************/
/*!
    @brief  moves arrow across display
    @return Void
*/
/**************************************************************************/
void MovingArrowClass::move() {
    float xChange = 0;
    float yChange = 0;

    xChange = -speed * sin(theta);
    yChange = -speed * cos(theta);

    xOrigin = xOrigin + xChange;
    yOrigin = yOrigin + yChange;
    
    // reset arrow if it is too far off screen
    if ((abs(xOrigin - SCREEN_WIDTH/2) >= X_LIM) or (abs(SCREEN_HEIGHT/2 - yOrigin) >= Y_LIM)) {
        xOrigin = SCREEN_WIDTH/2;
        yOrigin = SCREEN_HEIGHT/2;

    }
}
