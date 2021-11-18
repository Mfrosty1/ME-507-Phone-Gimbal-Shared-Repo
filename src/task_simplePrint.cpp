/** @file task_simplePrint.cpp
 *    This file the task for the IMU
 * 
 *  @author Matthew Frost
 *  @author Ryan McLaughlin
 *  @date  2021-Nov-06 Original file
 */

#include "task_simplePrint.h"
#include <PrintStream.h>
#include <Arduino.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "taskshare.h"         // Header for inter-task shared data
#include "taskqueue.h"         // Header for inter-task data queues
#include "shares.h"

/** @brief   Task which produces creates pin objects for the IMU and gets necessary data.
 *  @details This will come later
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_simplePrint (void* p_params)
{
    Serial << "Simple Print starting" << endl;
    for (;;)
    {
        Serial << "." << endl;

        vTaskDelay(500); // Matthew moved it to here
    }
}