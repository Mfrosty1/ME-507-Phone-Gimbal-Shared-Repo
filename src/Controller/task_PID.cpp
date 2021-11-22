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

    // Init the PID classes
    pitchController.Init();
    rollController.Init();
    yawController.Init(); 

    // SetInputs(Kp, Ki, Kd, tau, min, max, minInt, maxInt, T)                   
    pitchController.SetInputs(1.0f, 0.0f, 0.0f,  1.0f,  -25.0f,  25.0f,  -100.0f,    100.0f,    0.05f);
    rollController.SetInputs (0.5f, 0.0f, 0.5f,  1.0f,  -25.0f,  25.0f,  -100.0f,    100.0f,    0.05f);
    yawController.SetInputs  (1.0f, 0.0f, 0.0f,  1.0f,  -25.0f,  25.0f,  -100.0f,    100.0f,    0.05f);

    // input setpoints for each motor here
    float pitchSetpoint =  -90.0f; // should be -90 
    float rollSetpoint  =   0.0f; // should be 0
    float yawSetpoint   =   0.0f; // should be 0 or whichever direction it's pointing in

    for (;;)
    {
        // Serial << "PID here" << endl; // for testing purposes
        // get updated controller outputs based on setpoint and values from IMU
        float controlPitchSpeed = pitchController.Update( pitchSetpoint, pitchAngle.get() );
        float controlRollSpeed  = rollController.Update( rollSetpoint, rollAngle.get() );
        float controlYawSpeed   = yawController.Update( yawSetpoint, yawAngle.get() );
        // Serial.println("controller updates have been made");
        // Serial.println(controlYawSpeed);
        //Serial << "Cont: p = " << controlPitchSpeed << " r = " << controlRollSpeed << " y = " << controlYawSpeed << endl;
        Serial << " r = " << controlRollSpeed << ", angle = " << rollAngle.get() << endl;

        // put controller outputs into shares
        pMotSpeed.put(controlPitchSpeed);
        rMotSpeed.put(controlRollSpeed);
        yMotSpeed.put(controlYawSpeed);
        // Serial.println("controller has placed values into shares");

        // This task always runs once every 5 ms
        vTaskDelay (50);
    }
}
