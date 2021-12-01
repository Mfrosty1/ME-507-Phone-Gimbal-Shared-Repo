/*
 * DRV8256 - Library for the DRV8256 dual motor driver carrier.
 * The DRV8256 can be found here: https://www.pololu.com/product/2130
 * The DRV8256 data sheet can be found here: https://www.pololu.com/file/download/DRV8833.pdf?file_id=0J534
 *
 * Library: DRV8256
 * File: DRV8256.h
 *
 * Describes the class for the library.
 * v1.0
 *
 * Created March 16, 2015, by Aleksandr J. Spackman.
 *
 * This code is not yet in the public domain.
 *
 */

#ifndef DRV8256_H
#define DRV8256_H

#include "Arduino.h"

/**
 * Prewritten DRV8256 driver class
 */
class DRV8256
{
public:
	// Constructor for the class:
	DRV8256();

	/**
 	* Functions to attach motors
 	*/
	void attachMotor(uint8_t EN_pin, uint8_t PH_pin, uint8_t sleep_pin, uint8_t fault_pin);

	// Motor control functions:
	/// Function to move the motors
	void moveMotor(int16_t speed);
	/// Function to move the motor in reverse
	void motorReverse(uint8_t speed);
	/// Function to move the motor in forward
	void motorForward(uint8_t speed);
	/// Function to stop the motor
	void motorStop();
	/// Function to clear a fault per the DRV8256 specs
	void clearFault();

private:
	// Fields for the class:
	uint8_t enablePin, phasePin, sleepPin, faultPin, userButton;
	bool motorAttached = false;

};

#endif // DRV8256_H
