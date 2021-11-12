/** @file mIMU_Task.cpp
 *    This file the task for the IMU
 * 
 *  @author Matthew Frost
 *  @author Ryan McLaughlin
 *  @date  2021-Nov-06 Original file
 */

#include "IMU/ICM_20948.h" // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
#include "task_IMU.h"
#include <PrintStream.h>
#include "Wire.h"
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
void task_IMU (void* p_params)
{
    (void)p_params; // Shuts up a compiler warning

    Serial.begin(115200); // Start the serial console

    uint8_t SDA = PC1; // Same thing as A4
    uint8_t SCL = PC0; // Same thing as A5

    // Define a WIREPORT object
    TwoWire ourWire(SDA, SCL);

    #define AD0_VAL 1  // The value of the last bit of the I2C address.                
                       // On the SparkFun 9DoF IMU breakout the default is 1, and when 
                       // the ADR jumper is closed the value becomes 0

    // Define an object of the class ICM_20948
    ICM_20948_I2C myICM; // Create an ICM_20948_I2C object

    // Complete the setup for the WIREPORT
    ourWire.begin();
    ourWire.setClock(400000);

    bool initialized = false;
    while (!initialized)
    {
        myICM.begin(ourWire, AD0_VAL); // Begin communication with the WIREPORT

    #ifndef QUAT_ANIMATION
        Serial.print(F("Initialization of the sensor returned: "));
        Serial.println(myICM.statusString());
    #endif
        if (myICM.status != ICM_20948_Stat_Ok)
        {
            #ifndef QUAT_ANIMATION
            Serial.println(F("Trying again..."));
            #endif
            delay(500);
        }
        else
        {
            initialized = true;
        }
    }

    #ifndef QUAT_ANIMATION
      Serial.println(F("Device connected!"));
    #endif

    bool success = true; // Use success to show if the DMP configuration was successful

    // Initialize the DMP. initializeDMP is a weak function. You can overwrite it if you want to e.g. to change the sample rate
    success &= (myICM.initializeDMP() == ICM_20948_Stat_Ok);
    // Enable the DMP Game Rotation Vector sensor
    success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR) == ICM_20948_Stat_Ok);
    success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat6, 0) == ICM_20948_Stat_Ok); // Set to the maximum
    // Enable the FIFO
    success &= (myICM.enableFIFO() == ICM_20948_Stat_Ok);
    // Enable the DMP
    success &= (myICM.enableDMP() == ICM_20948_Stat_Ok);
    // Reset DMP
    success &= (myICM.resetDMP() == ICM_20948_Stat_Ok);
    // Reset FIFO
    success &= (myICM.resetFIFO() == ICM_20948_Stat_Ok);
    // Check success
    if (success)
    {
      #ifndef QUAT_ANIMATION
          Serial.println(F("DMP enabled!"));
      #endif
    }
    else
    {
      Serial.println(F("Enable DMP failed!"));
      Serial.println(F("Please check that you have uncommented line 29 (#define ICM_20948_USE_DMP) in ICM_20948_C.h..."));
      while (1)
        ; // Do nothing more
    }

    for (;;)
    {
        icm_20948_DMP_data_t data;
        myICM.readDMPdataFromFIFO(&data);

        if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
        {

            if ((data.header & DMP_header_bitmap_Quat6) > 0) // We have asked for GRV data so we should receive Quat6
            {
                // Scale to +/- 1
                double q1 = ((double)data.Quat6.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
                double q2 = ((double)data.Quat6.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
                double q3 = ((double)data.Quat6.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30
                // Serial << "q1=" << q1 << " q2=" << q2 << " q3=" << q3 << endl;
                // uint32_t negativeCounter = 0;
                // double insidesqrt = 1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3));
                // Serial << "q0 sqrt( )" << insidesqrt;
                // if (insidesqrt < 0)
                // {
                //     negativeCounter++;
                //     Serial << "negCount = " << negativeCounter <<  endl;
                // }

                // Convert the quaternions to Euler angles (roll, pitch, yaw)
                // https://en.wikipedia.org/w/index.php?title=Conversion_between_quaternions_and_Euler_angles&section=8#Source_code_2

                double q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));        // sqrt of negative?
                // Serial << "q0=" << q0 << "q1=" << q1 << " q2=" << q2 << " q3=" << q3 << endl;
                double q2sqr = q2 * q2;

                // roll (x-axis rotation)
                double t0 = +2.0 * (q0 * q1 + q2 * q3);
                double t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
                // double roll = atan2(t0, t1) * 180.0 / PI; // original
                double pitch = atan2(t0, t1) * 180.0 / PI;

                // pitch (y-axis rotation)
                double t2 = +2.0 * (q0 * q2 - q3 * q1);
                t2 = t2 > 1.0 ? 1.0 : t2;
                t2 = t2 < -1.0 ? -1.0 : t2;
                // double pitch = asin(t2) * 180.0 / PI; // original 
                double roll = asin(t2) * 180.0 / PI;

                // yaw (z-axis rotation)
                double t3 = +2.0 * (q0 * q3 + q1 * q2);
                double t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
                // double yaw = atan2(t3, t4) * 180.0 / PI; // original
                double yaw = -1*atan2(t3, t4) * 180.0 / PI; 

                // Serial << "t0=" << t0 << " t1=" << t1 << " t3=" << t3 << " t4=" << t4 << endl;
                // Serial.println(roll);
                // Serial.println(pitch);
                // Serial.println(yaw);

                // Serial << "IMU: pA " << pitch << ", rA " << roll << ", yA " << yaw << endl;

                if (isnan(pitch))
                {
                    Serial << "Pitch is nan. Skipping" << endl;
                }
                else
                {
                    pitchAngle.put(pitch);
                }

                if (isnan(roll))
                {
                    Serial << "Roll is nan. Skipping" << endl;
                }
                else
                {
                    rollAngle.put(roll);
                }
                
                if (isnan(yaw))
                {
                    Serial << "Yaw is nan. Skipping" << endl;
                }
                else
                {
                    yawAngle.put(yaw);
                }
                

                vTaskDelay(10);
            }
        }
    }
}