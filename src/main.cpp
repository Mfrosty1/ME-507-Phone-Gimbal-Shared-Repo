/** @file main.cpp
 *    Some description goes here
 *  @author Matthew Frost
 *  @author Ryan McLaughlin
 *  @date  2021-Nov-06 Original file
 */

// Includes for IMU stuff
// #include "IMU/ICM_20948.h" // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
#include "Wire.h"

// Includes for motor driver stuff
#include "DRV8256.h"

// General includes
#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif

// Includes for our tasks
// #include "task_IMU.h"            // Header for IMU task
#include "task_motor.h"          // Header for motor task
#include "Controller/task_PID.h" // Header for controller 
#include "task_simplePrint.h"
#include "task_BNO055.h"
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

// An example queue just in case we need it
// Queue<uint16_t> data_queue (100, "Data");

void setup() {

    // newClass myObj
    // void* pointer = &myObj;

    // Begin Serial port
    Serial.begin(115200);
    delay(5000);
    Serial.println("Initializing pins");

    // -------------- TASK DEFINITIONS -------------
    // xTaskCreate (task_IMU,
    //              "IMU Task",                      // Task name for printouts
    //              8192,                            // Stack size
    //              NULL,                            // Parameters for task fn.
    //              1,                               // Priority
    //              NULL);                           // Task handle

    xTaskCreate (task_BNO055,
                 "BNO Task",                      // Task name for printouts
                 8192,                            // Stack size
                 NULL,                            // Parameters for task fn.
                 1,                               // Priority
                 NULL);                           // Task handle

    // xTaskCreate (task_simplePrint,
    //              "Simple Print",                   // Task name for printouts
    //              1024,                            // Stack size
    //              NULL,                            // Parameters for task fn.
    //              1,                               // Priority
    //              NULL);                           // Task handle

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
 
void loop() 
{
    
}