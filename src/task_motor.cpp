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
#include "shares.h"
#include "task_motor.h"
#include "DRV8256.h"


/** @brief   Task which initalized the motors and then takes in speed from the controller.
 *  @param   p_params An unused pointer to nonexistent parameters
 */
void task_motor(void* p_params)
{
    // Make 3 objects of the DRV8256 class
    DRV8256 pitchMotor;
    DRV8256 rollMotor;
    DRV8256 yawMotor;

    //                     IN1, IN2, SLEEP, FAULT
    pitchMotor.attachMotor(PB5, PB4, PA5,   PA6); // Pitch motor pins
    rollMotor.attachMotor (PB5, PB4, PA5,   PA6); // Roll motor pins 
    yawMotor.attachMotor  (PB5, PB4, PA5,   PA6); // Yaw motor pins 

    for (;;)
    {

        int16_t newPitchSpeed = pMotSpeed.get(); // Get the next pitch speed (-255 to 255)
        int16_t newRollSpeed  = rMotSpeed.get(); // Get the next roll speed  (-255 to 255)
        int16_t newYawSpeed   = yMotSpeed.get(); // Get the next yaw speed   (-255 to 255)

        // Set new pitch speed
        if (newPitchSpeed > 0)
        {
            pitchMotor.motorForward(newPitchSpeed);
        }
        else if (newPitchSpeed < 0)
        {
            pitchMotor.motorReverse(newPitchSpeed);
        }
        else
        {
            pitchMotor.motorStop();
        }

        // Set new roll speed
        if (newRollSpeed > 0)
        {
            rollMotor.motorForward(newRollSpeed);
        }
        else if (newPitchSpeed < 0)
        {
            rollMotor.motorReverse(newRollSpeed);
        }
        else
        {
            rollMotor.motorStop();
        }

        // Set new yaw speed
        if (newYawSpeed > 0)
        {
            yawMotor.motorForward(newYawSpeed);
        }
        else if (newPitchSpeed < 0)
        {
            yawMotor.motorReverse(newYawSpeed);
        }
        else
        {
            yawMotor.motorStop();
        }

        // This task always runs once every 5 ms
        vTaskDelay (5);
    }
}
