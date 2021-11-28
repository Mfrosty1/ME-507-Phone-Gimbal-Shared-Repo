/** @file mIMU_Task.cpp
 *    This file the task for the IMU
 * 
 *  @author Matthew Frost
 *  @author Ryan McLaughlin
 *  @date  2021-Nov-06 Original file
 */

#include "task_BNO055.h"
#include <PrintStream.h>
#include "Wire.h"
#include <Arduino.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "taskshare.h"         // Header for inter-task shared data
#include "taskqueue.h"         // Header for inter-task data queues
#include "shares.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/** @brief   Task which produces creates pin objects for the IMU and gets necessary data.
 *  @details This will come later
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_BNO055 (void* p_params)
{
    (void)p_params; // Shuts up a compiler warning

    Serial.begin(115200); // Start the serial console

    uint8_t SDA = PC1; // Same thing as A4
    uint8_t SCL = PC0; // Same thing as A5
    TwoWire ourWire(SDA, SCL);
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &ourWire); 

    /* Initialise the sensor */
    if (!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Oops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    else
    {
        Serial << "bno is connected properly" << endl;
    }
    delay(1000); // For initialization purposes
    uint8_t state = 0; // Set state machine to state 0
    double firstYaw = 0; // Initialize firstYaw

    for (;;)
    {
        
        sensors_event_t orientationData;
        bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

        sensors_event_t* event = &orientationData;

        if (state == 0)
        {
            firstYaw = event->orientation.x; 
            Serial << "firstYaw = " << firstYaw << endl; 
            state += 1;
        }

        if (state == 1)
        {
            if (event->type == SENSOR_TYPE_ORIENTATION) 
            {
                double yaw = event->orientation.x - firstYaw; // yaw
                if (yaw > 180)
                {
                    yaw -= 360;
                }
                double roll = -1*event->orientation.y; // roll
                double pitch = -1*event->orientation.z; // pitch
                yawAngle.put(yaw);
                rollAngle.put(roll);
                pitchAngle.put(pitch);
                // Serial << "yaw = " << yaw << ", roll = " << roll << ", pitch = " << pitch <<  endl;
            }
            else 
            {
                Serial.print("Unk:");
            }

            uint8_t system, gyro, accel, mag = 0;
            bno.getCalibration(&system, &gyro, &accel, &mag);
        }
        vTaskDelay(40); 
    }
}