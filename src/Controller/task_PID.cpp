/** @file task_PID.cpp
 *      This file contains code for a task to run a controller object. This 
 *      creates three objects of the class @c PID such that each axes can be 
 *      controlled seperately.
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
#include "taskshare.h"        
#include "taskqueue.h"         


/** @brief   Task which sends and receives data from the PID controllers.
 *  @details In the setup of the task, the three objects of the @c PID class
 *           are initialized and the controller values are set. Also, the 
 *           setpoints of the IMU euler angles are set. Once it hits the @c for(;;)
 *           loop, the new duty cycles for the motors are retrieved and corrected 
 *           for by the pre-defined offset. These are then sent to shares to give
 *           to @c task_motor.cpp .
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
    
    // Set the inputs to the PID class for each axes 
    pitchController.SetInputs( 6.0f,  0.5f,  4.0f, 0.1f, -250.0f, 250.0f, -100.0f,  100.0f, 0.05f); 
    rollController.SetInputs ( 0.3f, 0.01f,  3.0f, 0.1f, -250.0f, 250.0f,  -10.0f,   10.0f, 0.05f); 
    yawController.SetInputs  (20.0f,  0.0f,  0.0f, 0.5f, -250.0f, 250.0f,  -10.0f,   10.0f, 0.05f); 

    // input setpoints for each motor here
    float pitchSetpoint = -90.0f; // should be -90 for the BNO055
    float rollSetpoint  =   0.0f; // should be   0 for the BNO055
    float yawSetpoint   =   0.0f; // should be   0 for the BNO055

    /* These are the minimum values such that the motors move with the inertia of
       the phone/gimbal assembly and the stiction in the system */
    float rollOffset  =  20;  // Minimum value to feel resistance at small angles
    float pitchOffset =  70;  // Minimum value to feel resistance at small angles 
    float yawOffset   =  30;  // Minimum value to feel resistance at small angles

    for (;;)
    {
        // Get updated controller outputs based on setpoint and values from IMU
        float controlPitchSpeed = pitchController.Update( pitchSetpoint, pitchAngle.get() );
        float controlRollSpeed  = rollController.Update( rollSetpoint, rollAngle.get() );
        float controlYawSpeed   = yawController.Update( yawSetpoint, yawAngle.get() );
        
        // Correct for the stiction in the motors.
        if (controlRollSpeed > 0)
        {
            controlRollSpeed += rollOffset;
        }
        else if (controlRollSpeed < 0)
        {
            controlRollSpeed -= rollOffset;
        }
        
        if (controlPitchSpeed > 0)
        {
            controlPitchSpeed += pitchOffset;
        }
        else if (controlPitchSpeed < 0)
        {
            controlPitchSpeed -= pitchOffset;
        }

        if (controlYawSpeed > 0)
        {
            controlYawSpeed += yawOffset;
        }
        else if (controlYawSpeed < 0)
        {
            controlYawSpeed -= yawOffset;
        }

        // put controller outputs into shares after correction
        pMotSpeed.put(controlPitchSpeed);
        rMotSpeed.put(controlRollSpeed);
        yMotSpeed.put(controlYawSpeed);

        // Print statements for debugging purposes
        // Serial << " r = " << controlRollSpeed << ", angle = " << rollAngle.get() << endl;
        // Serial << " p = " << controlPitchSpeed << ", angle = " << pitchAngle.get() << endl;
        // Serial << " y = " << controlYawSpeed << ", angle = " << yawAngle.get() << endl;

        // This task always runs once every 20 ms
        vTaskDelay (20);
    }
}
