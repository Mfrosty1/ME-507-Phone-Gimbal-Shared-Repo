/** @file task_PID.cpp
 *  This file contains code for a task to run a controller object.
 * 
 *  @author Matthew Frost
 *  @author Ryan McLaughlin
 *  @date   11/09/2021 Original file creation
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h"
#include "task_PID.h"
#include "PID.h"
#include "taskshare.h"         // Header for inter-task shared data
#include "taskqueue.h"         // Header for inter-task data queues


/** @brief   Task which initalized the motors and then takes in speed from the controller.
 *  @param   p_params An unused pointer to nonexistent parameters
 */
void task_PID(void* p_params)
{
    // Make 3 objects of the PID class.
    PID pitchController;
    PID rollController;
    PID yawController;

    // Initialize the PID classes
    pitchController.Init();
    rollController.Init();
    yawController.Init(); 

    // SetInputs             (  Kp,   Ki,    Kd,  tau,     min,    max, minInt, maxInt,     T)                   
    pitchController.SetInputs(4.5f, 1.0f, 18.0f, 1.0f, -100.0f, 100.0f, -10.0f,  10.0f, 0.05f); // Matthew finished prelim tuning (4.5, 1.0, 18.0)
    rollController.SetInputs (1.5f, 0.5f,  4.0f, 1.0f,  -75.0f,  75.0f, -10.0f,  10.0f, 0.05f); // Matthew finished prelim tuning (1.5, 0.5, 4.0)
    yawController.SetInputs  (5.0f, 0.5f, 25.0f, 1.0f, -150.0f, 150.0f, -10.0f,  10.0f, 0.05f); // Matthew finished prelim tuning (5.0, 0.5, 25.0)

    // input setpoints for each motor here
    float pitchSetpoint =  -90.0f;          // should be -90 
    float rollSetpoint  =   0.0f;           // should be 0
    float yawSetpoint   =   yawAngle.get(); // whichever direction it's pointing in at start

    for (;;)
    {
        // get updated controller outputs based on setpoint and values from IMU
        float controlPitchSpeed = pitchController.Update( pitchSetpoint, pitchAngle.get() );
        float controlRollSpeed  = rollController.Update( rollSetpoint, rollAngle.get() );
        float controlYawSpeed   = yawController.Update( yawSetpoint, yawAngle.get() );
        // Serial << "Cont: p = " << controlPitchSpeed << " r = " << controlRollSpeed << " y = " << controlYawSpeed << endl;
        // Serial << " r = " << controlRollSpeed << ", angle = " << rollAngle.get() << endl;
        Serial << " p = " << controlPitchSpeed << ", angle = " << pitchAngle.get() << endl;
        // Serial << " y = " << controlYawSpeed << ", angle = " << yawAngle.get() << endl;

        // put controller outputs into shares
        pMotSpeed.put(controlPitchSpeed);
        rMotSpeed.put(controlRollSpeed);
        yMotSpeed.put(controlYawSpeed);

        // This task always runs once every 40 ms
        vTaskDelay (40);
    }
}
