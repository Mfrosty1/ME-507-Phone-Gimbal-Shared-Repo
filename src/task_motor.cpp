/** @file task_motor.cpp
 *  This file contains code for a task to control the motors.
 * 
 *  @author Matthew Frost
 *  @author Ryan McLaughlin
 *  @date   2021-Nov-09 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "taskshare.h"         // Header for inter-task shared data
#include "taskqueue.h"         // Header for inter-task data queues
#include "shares.h"
#include "task_motor.h"
#include "DRV8256.h"



/** @brief   Task which initalized the motors and then takes in speed from the controller.
 *  @param   p_params An unused pointer to nonexistent parameters
 */
void task_motor(void* p_params)
{
    // newClass* task_params = (newClass*)p_params;
    // Make 3 objects of the DRV8256 class
    DRV8256 pitchMotor;
    DRV8256 rollMotor;
    DRV8256 yawMotor;

    //                     IN1, IN2, SLEEP, FAULT
    // pitchMotor.attachMotor(PB9, PB8, PA5,   PA4); // Pitch motor pins
    // rollMotor.attachMotor (PB5, PB4, PA5,   PA4); // Roll motor pins 
    // yawMotor.attachMotor  (PA8, PA9, PA6,   PA4); // Yaw motor pins 
    // pitchMotor.attachMotor(PB5, PB4, PA5,   PA4); // Pitch motor pins
    // rollMotor.attachMotor (PB5, PB4, PA5,   PA4); // Roll motor pins 
    rollMotor.attachMotor (PB5, PB4, PA7,   PA4); // Roll motor pins 

    // yawMotor.attachMotor  (PB5, PB4, PA5,   PA4); // Yaw motor pins 

    // Switching the nSLEEP and nFAULT pins to see if fault is being triggered
    // rollMotor.attachMotor (PB5, PB4, PA4,   PA5); // Roll motor pins 
    Serial.println("MotTask: motor objects defined");

    // Set up USER button for clearing faults
    // uint8_t userButton = PC13;
    // pinMode(userButton, INPUT);

    for (;;)
    {
        // Set the motors to their new speed
        // pitchMotor.moveMotor(pMotSpeed.get());
        rollMotor.moveMotor(rMotSpeed.get());
        // yawMotor.moveMotor(yMotSpeed.get());

        // if (!digitalRead(userButton))
        // {
        //     rollMotor.clearFault(); // if button is pressed, clear fault.
        // }

        // This task always runs once every 30 ms
        vTaskDelay (10);
        // vTaskDelay (task_param->var);

    }
}
