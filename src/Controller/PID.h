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

/**
 * Class to calculate the next motor speed based on a PID loop
 */
class PID
{
	protected:
	
		/// Proportional gain
		float Kp;
		/// Integrator gain
		float Ki;
		/// Derivative gain
		float Kd;

		/// Derivative low-pass filter time constant
		float tau;

		/// Output max limit
		float limMin;
		/// Output min limit
		float limMax;

		/// Integrator max limit
		float limMinInt;
		/// Integrator min limit
		float limMaxInt;

		/// Sample time (in seconds)
		float T;

		// ------- Controller Memory (not user inputted) -------
		/// Integrator term
		float integrator;
		/// The previous error, needed for the integrator
		float prevError;		
		/// Differentiator term
		float differentiator;
		/// The previous measurement, needed for the differentiator
		float prevMeasurement;

		///S controller ouptut
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