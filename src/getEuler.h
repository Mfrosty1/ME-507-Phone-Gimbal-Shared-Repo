/** @file getEuler.h
 *    This file
 * 
 *  @author Matthew Frost
 *  @date  2021-Nov-18 Original file
 */
// #include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
// #include <utility/imumaths.h>

/** @brief   Class which is given measurements and computes some statistical parameters.
 *  @details This class will add data to a database and can compute averages or
 *           standard deviations when called. It can also return the number of measurements
 *           in the database or can clear all numbers.
 */
class getEuler
{
protected:
    double x;
    double y; 
    double z;                      

public:  
    getEuler(void);                            // Constructor to initialize variables.
    void printEvent(sensors_event_t* event);   
};