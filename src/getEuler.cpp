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
        x = event->orientation.x;
        y = event->orientation.y;
        z = event->orientation.z;
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
