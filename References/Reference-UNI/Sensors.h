/*!
    @file   functions to interact with external sensors.
            sensors include IMU and UDS (Ultrasonic Distance Sensor)
*/

// inclusion gard
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
// for IMU
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


void begin_IMU(Adafruit_MPU6050 &mpu);
void get_tilt_info(Adafruit_MPU6050 &mpu, float &tilt_x, float &tilt_y);
void begin_UDS();
float read_UDS();


#define mpuAddress 0x68   // I2C address of the MPU-6050
// Distance sensor pins
#define PIN_TRIG 3
#define PIN_ECHO 2

// --------------------------IMU------------------------------

/**************************************************************************/
/*!
    @brief  Attempt to connect to the IMU.
            Will print a message to terminal if connection is a success 
            or a failure.
    @param  mpu
            address of MPU sensor object
    @return Void
*/
/**************************************************************************/
void begin_IMU(Adafruit_MPU6050 &mpu) {
    if (!mpu.begin(mpuAddress)) {     // default address is 0x68
        Serial.println("MPU6050 connection failed");
        while (1);  // IMU didn't connect, loop forever
    }
    // device connected successfully
    Serial.println("MPU6050 Connected!");   
}


/**************************************************************************/
/*!
    @brief  Assigns the tilt info from IMU of x-axis and y-axis to 
            the two parameters
    @param  mpu
            adress of MPU sensor object
    @param  tilt_x 
            Reference to float type variable. Holds x-axis info
    @param  tilt_y
            Refernce to float type variable. Holds y-axis info
    @return Void
*/
/**************************************************************************/
void get_tilt_info(Adafruit_MPU6050 &mpu, float &tilt_x, float &tilt_y) {
    // declare accel, gyro, and temp as 'sensors_event_t' objects
    sensors_event_t accel, gyro, temp;
    // use method from the mpu object to set attributes of accel, gyro, and temp
    mpu.getEvent(&accel, &gyro, &temp);

    // assign the tilt information from the accelerometer of the IMU
    tilt_x = accel.acceleration.x;
    tilt_y = accel.acceleration.y;
}


/*--------------------------DISTANCE SENSOR------------------------------*/

/**************************************************************************/
/*!
    @brief  Connect the ultrasonic distance sensor
    @return Void
*/
/**************************************************************************/
void begin_UDS() {
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO,  INPUT);
}


/**************************************************************************/
/*!
    @brief  retrieve information from the ultrasonic distance sensor
    @return float describing the distance
*/
/**************************************************************************/
float read_UDS() {
    //start new measurement
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    //read result
    float duration = pulseIn(PIN_ECHO, HIGH);
    float distance = duration / 58; //distance in cm

    return distance;
}


#endif  // SENSORS_H