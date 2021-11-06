/** @file PID.cpp
 *    This file implements a PID controller to be used with 
 *    discrete data from a sensor. Additional features beyond
 *    a base PID controller include:
 *    - Low-pass filter on differentiation
 *    - Derivative-on-measurement
 *    - Integrator anti-windup
 *    - Controller ouptut clamping
 *  @author Ryan McLaughlin & Matthew Frost
 *  @author pms67 (original author of files adapted)
 *  @date   11/06/2021 Code accesed from pms67 GitHub repository
 *  @date   
 */

#include "PID.h"
#include "Arduino.h"

/** @brief   Initialize controller variables.
 *  @details This constructor initializes controller intermediate
 *           values such as i[n], e[n-1], d[n], and the  
 *           previous measurement. It also sets the controller output
 *           initially to zero.
 */
void PID::Init(void)
{
	// Clear controller variables
	integrator = 0.0f;
	prevError  = 0.0f;

	differentiator  = 0.0f;
	prevMeasurement = 0.0f;
	out = 0.0f;
}


/** @brief   Update the controller to get a new controller output.
 *  @details This method calculates a new controller output value
 *  @param   setpoint    The current controller setpoint (@c float data type).
 *  @param   measurement The current sensor measurment (@c float data type).
 *  @return  The controller ouptut value.
 */
float PID::Update(float setpoint, float measurement) 
{
	// Error Signal
    float error = setpoint - measurement;

	//Proportial Signal
    float proportional = Kp * error;

	// Integral Signal
    integrator +=  0.5f*Ki*T*(error + prevError);

	// Anti-wind-up via integrator clamping 
	// set to limits if outside bounds
    if (integrator > limMaxInt) {
        integrator = limMaxInt;

    } else if (integrator < limMinInt) {
        integrator = limMinInt;

    }

	// Band Limited Differentiation
    differentiator = -(2.0f * Kd * (measurement - prevMeasurement)	// Note: derivative on measurement, therefore minus sign in front of equation!
                        + (2.0f * tau - T) * differentiator)
                        / (2.0f * tau + T);

	// Compute output and apply limits
    out = proportional + integrator + differentiator;

    if (out > limMax) {
        out = limMax;

    } else if (out < limMin) {
        out = limMin;

    }

	// Store error and measurement for later use
    prevError       = error;
    prevMeasurement = measurement;

	// Return controller output
    return out;

}
