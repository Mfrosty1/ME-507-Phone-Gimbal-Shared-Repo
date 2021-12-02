/** @file task_motor.cpp
 *      This file contains code for a task to control the motors. This will
 *      instantiate each of the three motors and send the newest duty cycle
 *      from the controller to each of them separately.
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
#include "taskshare.h"         
#include "taskqueue.h"         
#include "shares.h"    
#include "task_motor.h"
#include "DRV8256.h"

/** @brief   Task which initalized the motors and then takes in speed from the controller.
 *  @details As designed on our circuit board, the pins for the enable, phase, !SLEEP, and 
 *           !FAULT pins are defined when the @c attachMotor function of the @c DRV8256 class.
 *           Once the motors are initialized, this task only gets the newest speed from the
 *           controller and tells the motor to move by that much.
 *  @param   p_params An unused pointer to nonexistent parameters
 */
void task_motor(void* p_params)
{
    // Make 3 objects of the DRV8256 class
    DRV8256 pitchMotor;
    DRV8256 rollMotor;
    DRV8256 yawMotor;

    // Pin Modes           IN1/EN, IN2/PH, !SLEEP, !FAULT
    rollMotor.attachMotor (   PB4,    PB5,    PA5,    PA4); // Roll motor pins
    pitchMotor.attachMotor(   PB8,    PB9,    PA5,    PA4); // Pitch motor pins
    yawMotor.attachMotor  (   PA9,    PA8,    PA6,    PA4); // Yaw motor pins 

    Serial.println("MotTask: motor objects defined");

    for (;;)
    {
        // Set the motors to their new speed
        rollMotor.moveMotor (rMotSpeed.get());
        pitchMotor.moveMotor(pMotSpeed.get());
        yawMotor.moveMotor  (yMotSpeed.get());

        // This task always runs once every 20 ms
        vTaskDelay (20);
    }
}
