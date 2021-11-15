/*
 * DRV8256 - Library for the DRV8256 dual motor driver carrier.
 * The DRV8256 can be found here: https://www.pololu.com/product/2130
 * The DRV8256 data sheet can be found here: https://www.pololu.com/file/download/DRV8833.pdf?file_id=0J534
 *
 * Library: DRV8256
 * File: DRV8256.cpp
 *
 * Defines the class for the library.
 * v1.0
 *
 * Created March 16, 2015, by Aleksandr J. Spackman.
 *
 * This code is not yet in the public domain.
 *
 */

#include "Arduino.h"
#include "DRV8256.h"
#include "PrintStream.h"

// constructor
DRV8256::DRV8256()
{
	// Does nothing.
	// Use attachMotor()
}

void DRV8256::attachMotor(uint8_t EN_pin, uint8_t PH_pin, uint8_t sleep_pin, uint8_t fault_pin)
{
	if (!this->motorAttached) // If motor A is NOT attached...
	{
		this->enablePin = EN_pin;
		this->phasePin = PH_pin;
		this->sleepPin = sleep_pin;
		this->faultPin = fault_pin;

		// Create a pin mode for the user button
		// this->userButton = PC_13; 
		// pinMode(userButton, INPUT);

		// Attach motor to the input pins.
		pinMode(enablePin, OUTPUT);      // Set EN/IN1 pin to OUTPUT
		pinMode(phasePin,  OUTPUT);      // Set PH/IN1 pin to OUTPUT
    	pinMode(sleepPin,  OUTPUT);      // Set !SLEEP pin to OUTPUT
		pinMode(faultPin, INPUT_PULLUP); // Set the fault pin to an INPUT with PULLUP

		// Show the motor is attached.
		this->motorAttached = true;

		// Initialize pins.
		digitalWrite(enablePin, HIGH); // Set EN to HIGH
		digitalWrite(phasePin, LOW);   // Set PH to LOW
		digitalWrite(sleepPin, HIGH);  // Set !SLEEP to HIGH

		// Serial.print("Starting Fault Pin Value: ");
		// Serial.println(digitalRead(faultPin));
	}
}

// Combined function to run motors in both direction
void DRV8256::moveMotor(int16_t speed)
{
	if (!digitalRead(faultPin))
	{
		Serial << "Fault occured" << endl;
	}

	if (!digitalRead(sleepPin))
	{
		Serial << "Sleep Low, Sleep Mode" << endl;
	}

	if (this->motorAttached) // If motor is attached...
	{
		if (speed > 0) // ... and speed is in forward direction
		{
			// ... then put it in forward.
			Serial << "F: " << speed << endl;
			analogWrite(enablePin, speed);
			digitalWrite(phasePin, HIGH);
		}
		else if (speed < 0) // ... and speed is in reverse direction
		{
			// ... then put it in reverse.
			speed *= -1;
			Serial << "R: " << speed << endl;
			analogWrite(enablePin, speed); 
			digitalWrite(phasePin, LOW);
		}
		else
		{
			Serial << "Speed is 0" << endl;
			analogWrite(enablePin, 0);
			digitalWrite(phasePin, LOW);
		}
	}
}

// function to run motor in reverse at a specified speed
void DRV8256::motorReverse(uint8_t speed)
{
	if (this->motorAttached) // If motor is attached...
	{
		// ...then put it in reverse.
		analogWrite(enablePin, speed);
		digitalWrite(phasePin, LOW);
	}
}

// function to run motor forward at a specified speed
void DRV8256::motorForward(uint8_t speed)
{
	if (this->motorAttached) // If motor is attached...
	{
		// ...then put it in forward.
		analogWrite(enablePin, speed);
		digitalWrite(phasePin, HIGH);
	}
}

// stop motor
void DRV8256::motorStop()
{
	if (this->motorAttached) // If motor is attached...
	{
		// ...then stop it.
		analogWrite(enablePin, 0);
		digitalWrite(phasePin, LOW);
	}
}

void DRV8256::clearFault()
{
	if (this->motorAttached) // If motor is attached...
	{
		Serial << "Button pressed" << endl;
		// Set SLEEP to low for 20-40ms
		digitalWrite(sleepPin, LOW);
		delay(25); // allow time to reset
		digitalWrite(sleepPin, HIGH); // Set back to high. Fault should be cleared now.
	}
}

