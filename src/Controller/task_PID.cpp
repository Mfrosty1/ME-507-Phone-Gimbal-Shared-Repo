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

    // Testing before reducing Tau
    // SetInputs             (  Kp,   Ki,    Kd,  tau,     min,    max, minInt, maxInt,     T)                   
    // pitchController.SetInputs(5.0f, 2.0f, 20.0f, 1.0f, -100.0f, 100.0f, -10.0f,  10.0f, 0.05f); // Matthew finished tuning with phone (5.0, 2.0, 20.0)
    // rollController.SetInputs (1.0f, 0.5f,  6.0f, 1.0f,  -75.0f,  75.0f, -10.0f,  10.0f, 0.05f); // Matthew finished tuning with phone (1.0, 0.5, 6.0)
    // yawController.SetInputs  (6.0f, 0.0f, 0.0f, 1.0f, -150.0f, 150.0f, -10.0f,  10.0f, 0.05f); // Matthew finished tuning with phone (1.0, 0.5, 6.0)

    // Testing after reducing Tau
    pitchController.SetInputs(7.0f, 0.0f, 4.0f, 0.1f, -250.0f, 250.0f, -10.0f,  10.0f, 0.05f); // Matthew finished tuning with phone (5.0, 2.0, 20.0)
    rollController.SetInputs (3.0f,  0.0f,  3.0f, 0.1f, -250.0f, 250.0f, -10.0f,  10.0f, 0.05f); // Matthew finished tuning with phone (1.0, 0.5, 6.0)
    yawController.SetInputs  (20.0f, 0.0f,  0.0f, 0.5f, -250.0f, 250.0f, -10.0f,  10.0f, 0.05f); // Matthew finished tuning with phone (1.0, 0.5, 6.0)

    // input setpoints for each motor here
    float pitchSetpoint =  -90.0f; // should be -90 for the BNO055
    float rollSetpoint  =   0.0f;  // should be 0 for the BNO055
    float yawSetpoint   =   0.0f;  // should be 0 for the BNO055

    for (;;)
    {
        // get updated controller outputs based on setpoint and values from IMU
        float controlPitchSpeed = pitchController.Update( pitchSetpoint, pitchAngle.get() );
        float controlRollSpeed  = rollController.Update( rollSetpoint, rollAngle.get() );
        float controlYawSpeed   = yawController.Update( yawSetpoint, yawAngle.get() );
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
