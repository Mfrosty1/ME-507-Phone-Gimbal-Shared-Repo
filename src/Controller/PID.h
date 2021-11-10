/** @file PID.h
 *    This header file is for a class that implements a PID
 * 	  controller with some extra functionality (low-pass filter
 *    on differentiation, integrator clamping, etc.).
 * 
 *  @author Ryan McLaughlin
 *  @author Matthew Frost
 *  @author pms67 (original author of files adapted)
 *  @date   11/06/2021 Code accesed from pms67 GitHub repository
 *  @date   
 */

#include "Arduino.h"

class PID
{
	protected:
	
		// ------ User inputs -------
		// Controller Gains
		float Kp;
		float Ki;
		float Kd;

		// Derivative low-pass filter time constant
		float tau;

		// Output limits
		float limMin;
		float limMax;

		// Integrator limits
		float limMinInt;
		float limMaxInt;

		// Sample time (in seconds)
		float T;

		// ------- Controller Memory (not user inputted) -------
		float integrator;
		float prevError;			// Required for integrator
		float differentiator;
		float prevMeasurement;		// Required for differentiator

		// controller ouptut
		float out;

	public:
		void SetInputs(float Kp_in, 
					   float Ki_in, 
					   float Kd_in,
					   float tau_in,
					   float limMin_in,
					   float limMax_in,
					   float limMinInt_in,
					   float limMaxInt_in,
					   float T_in);
		
		void Init(void);

        float Update(float setpoint, 
					 float measurement);

};