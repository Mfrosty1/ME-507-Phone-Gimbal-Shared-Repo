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

    //                     IN1/EN, IN2/PH, SLEEP, FAULT
    rollMotor.attachMotor (PB4,    PB5,    PA5,   PA4); // Roll motor pins with PCB
    pitchMotor.attachMotor(PB8,    PB9,    PA5,   PA4); // Pitch motor pins
    yawMotor.attachMotor  (PA9,    PA8,    PA6,   PA4); // Yaw motor pins 
    // rollMotor.attachMotor (PB5,  PB4, PA7,   PA4); // Roll motor pins with breakout board

    Serial.println("MotTask: motor objects defined");

    for (;;)
    {
        // Set the motors to their new speed
        pitchMotor.moveMotor(pMotSpeed.get());
        rollMotor.moveMotor(rMotSpeed.get());
        yawMotor.moveMotor(yMotSpeed.get());

        // This task always runs once every 30 ms
        vTaskDelay (50);
        // vTaskDelay (task_param->var);

    }
}
