/** @file getEuler.cpp
 *    This file
 * 
 *  @author Matthew Frost
 *  @date  2020-Nov-18 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#include <getEuler.h>
#include <cmath>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
// #include <baseshare.h> 
// #include "taskshare.h"           // Header for inter-task shared data
// #include "taskqueue.h"           // Header for inter-task data queues
// #include "shares.h"              // Header for shares used in this project

/** @brief   
 *  @details 
 */
getEuler::getEuler(void)
{
    Serial << "getEuler class instantiated" << endl;
}

void getEuler::printEvent(sensors_event_t* event)
{
    x = -1000000;
    y = -1000000;
    z = -1000000; //dumb values, easy to spot problem
    if (event->type == SENSOR_TYPE_ORIENTATION) 
    {
        x = event->orientation.x; // yaw
        y = event->orientation.y; // roll
        z = event->orientation.z; // pitch
        x -= 180; // modify
        y -= 180; // modify
        z -= 180; // modify
        // yawAngle.put(x);
        // rollAngle.put(y);
        // pitchAngle.put(z);

    }
    else 
    {
        Serial.print("Unk:");
    }

    Serial.print("x = ");
    Serial.print(x);
    Serial.print(", y = ");
    Serial.print(y);
    Serial.print(", z = ");
    Serial.println(z);
}
