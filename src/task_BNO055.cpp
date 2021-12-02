/** @file task_BNO055.cpp
 *    This file the task for the IMU. This makes an object of a 
 *    BNO055 driver and will @c put() the current Euler angles 
 *    for the controller task to interpret.
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
#include "taskshare.h"         
#include "taskqueue.h"         
#include "shares.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/** @brief   Task which produces creates pin objects for the IMU and sends necessary data.
 *  @details Upon start up, the pins for the SDA and SCL are defined using the @c Wire library.
 *           Once the BNO connection has been made successfully, the current Euler angles are 
 *           retreived and sent to the shares/controller.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_BNO055 (void* p_params)
{
    (void)p_params; // Shuts up a compiler warning

    Serial.begin(115200); // Start the serial console

    uint8_t SDA = PC1;         // Same thing as A4
    uint8_t SCL = PC0;         // Same thing as A5
    TwoWire ourWire(SDA, SCL); // Define a wireport for the SDA/SCL

    /* Make an object of the class @c Adafruit_BNO055, and initialize the constructor
       with the new wire. */
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

    // Set state machine to state 0
    uint8_t state = 0; 

    // Initialize firstYaw, which will be redefined later
    double firstYaw = 0; 

    for (;;)
    {
        /* Use the example code from the GitHub repository to get the
           current Euler angle data from the correct BNO055 address. */
        sensors_event_t orientationData;
        bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

        sensors_event_t* event = &orientationData;

        // Set the yaw angle offset such that it starts at 0 degrees.
        if (state == 0)
        {
            firstYaw = event->orientation.x; // Get the current yaw 
            Serial << "firstYaw = " << firstYaw << endl; 
            state += 1; // Increment the state machine
        }

        // The main state to send Euler angle data
        if (state == 1)
        {
            if (event->type == SENSOR_TYPE_ORIENTATION) 
            {
                double yaw = event->orientation.x - firstYaw; // current yaw angle
                if (yaw > 180)
                {
                    yaw -= 360;
                }
                double roll = -1*event->orientation.y; // currrent roll angle, corrected for desired direction
                double pitch = -1*event->orientation.z; // current pitch angle, corrected for desired direction
                
                // Put the current angles to shares
                yawAngle.put(yaw);
                rollAngle.put(roll);
                pitchAngle.put(pitch);

                // Printing data to be plotted later. If desired to collect data, uncomment the next line
                // Serial << millis() << ", " << pitch << endl;
            }
            else 
            {
                Serial.print("Unk:"); // If the sensor loses connection or doesn't have data to read
            }

            // A simple calibration/reset to keep the IMU running, as shown in the example code
            uint8_t system, gyro, accel, mag = 0;
            bno.getCalibration(&system, &gyro, &accel, &mag);
        }

        // This task always runs once every 10 ms
        vTaskDelay(10); 
    }
}