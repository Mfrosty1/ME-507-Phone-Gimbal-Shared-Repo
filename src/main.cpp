/** @file main.cpp
 *    Some description goes here
 *  @author Matthew Frost
 *  @author Ryan McLaughlin
 *  @date  2021-Nov-06 Original file
 */

// Includes for IMU stuff
#include "Wire.h"

// Includes for motor driver stuff
#include "DRV8256.h"

// General includes
#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif

// Includes for tasks
#include "task_motor.h"          // Header for motor task
#include "Controller/task_PID.h" // Header for controller 
#include "task_BNO055.h"         // BNO055 IMU Task
#include "taskshare.h"           // Header for inter-task shared data
#include "taskqueue.h"           // Header for inter-task data queues
#include "shares.h"              // Header for shares used in this project

// Declare the shared variables and queues
/// A share to hold the current pitch angle.
Share<float> pitchAngle("Pitch Angle");
/// A share to hold the current roll angle.
Share<float> rollAngle("Roll Angle");
/// A share to hold the current yaw angle.
Share<float> yawAngle("Yaw Angle");
/// A share to hold the desired pitch motor speed.
Share<int16_t> pMotSpeed("Pitch Motor Speed");
/// A share to hold the desired roll motor speed.
Share<int16_t> rMotSpeed("Roll Motor Speed");
/// A share to hold the desired yaw motor speed.
Share<int16_t> yMotSpeed("Yaw Motor Speed");

/**
 * The standard Arduino setup function used for setup and configuration tasks.
 */
void setup() {

    // Begin Serial port
    Serial.begin(115200);
    delay(5000);
    Serial.println("Initializing pins");

    // -------------- TASK DEFINITIONS -------------
    xTaskCreate (task_BNO055,
                 "BNO Task",                      // Task name for printouts
                 8192,                            // Stack size
                 NULL,                            // Parameters for task fn.
                 1,                               // Priority
                 NULL);                           // Task handle

    xTaskCreate (task_motor,
                 "Motor Task",                    // Task name for printouts
                 4096,                            // Stack size
                 NULL,                            // Parameters for task fn.
                 1,                               // Priority
                 NULL);                           // Task handle

    xTaskCreate (task_PID,
                 "Controller Task",               // Task name for printouts
                 4096,                            // Stack size
                 NULL,                            // Parameters for task fn.
                 1,                               // Priority
                 NULL);                           // Task handle             

    // If using an STM32, we need to call the scheduler startup function now;
    // if using an ESP32, it has already been called for us
    #if (defined STM32L4xx || defined STM32F4xx)
        vTaskStartScheduler ();
    #endif             
}

/**
 * The standard Arduino loop function used for repeating tasks. Nothing is inputted for this project
 */ 
void loop() 
{
    
}