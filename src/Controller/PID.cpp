/** @file PID.cpp
 *    This file implements a PID controller to be used with 
 *    discrete data from a sensor. Additional features beyond
 *    a base PID controller include:
 *    - Low-pass filter on differentiation
 *    - Derivative-on-measurement
 *    - Integrator anti-windup
 *    - Controller ouptut clamping
 *  @author Ryan McLaughlin
 *  @author Matthew Frost
 *  @author pms67 (original author of files adapted)
 *  @date   11/06/2021 Code accesed from pms67 GitHub repository
 *  @date   
 */

#include "PID.h"
#include "Arduino.h"
#include "PrintStream.h"


/** @brief   Initialize controller gains and inputs.
 *  @details This constructor takes in user selections for gains
 *           and other contorller inputs such as output limits,
 *           integrator limits, and rate of sensor data collection.
 *  @param   Kp_in          Proportional gain.
 *  @param   Ki_in          Integral gain.
 *  @param   Kd_in          Derivative gain.
 *  @param   tau_in         Differentiator low-pass filter time constant.
 *  @param   limMin_in      Output limit (negative values).
 *  @param   limMax_in      Output limit (positive values).
 *  @param   limMinInt_in   Integral control output limit (negative values).
 *  @param   limMaxInt_in   Integral control output limit (positive values).
 *  @param   T_in           Sensor data collection rate [seconds].
 */
void PID::SetInputs(float Kp_in, 
                    float Ki_in, 
                    float Kd_in,
                    float tau_in,
                    float limMin_in,
                    float limMax_in,
                    float limMinInt_in,
                    float limMaxInt_in,
                    float T_in)
{
	// set controller values
	this->Kp = Kp_in;
    this->Ki = Ki_in;
    this->Kd = Kd_in;
    this->tau = tau_in;
    this->limMin = limMin_in;
    this->limMax = limMax_in;
    this->limMinInt = limMinInt_in;
    this->limMaxInt = limMaxInt_in;
    this->T = T_in;
    Serial.println("Controller values set.");
}


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

    // check for nan conditions
    if (isnan(proportional))
    {
        // Serial.println("prop: nan achieved");
        proportional = 0;
    }

	// Integral Signal
    integrator +=  0.5f*Ki*T*(error + prevError);

    // check for nan conditions
    if (isnan(integrator))
    {
        // Serial.println("int: nan achieved");
        integrator = 0;
    }

	// Anti-wind-up via integrator clamping 
	// set to limits if outside bounds
    if (integrator > limMaxInt) {
        integrator = limMaxInt;

    } else if (integrator < limMinInt) {
        integrator = limMinInt;

    }

	// Band Limited Differentiation
    differentiator = -(2.0f * Kd * (measurement - prevMeasurement)	 + (2.0f * tau - T) * differentiator)
                        / (2.0f * tau + T);
    
    // check for nan conditions
    if (isnan(differentiator))
    {
        // Serial.println("diff: nan achieved");
        differentiator = 0;
    }

	// Compute output and apply limits
    out = proportional + integrator + differentiator;

    // Serial << "PID.cpp, out = " << out << endl;

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
