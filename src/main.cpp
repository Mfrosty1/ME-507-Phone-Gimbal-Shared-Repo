#include <Arduino.h>

uint32_t in1Pin = PB4, in2Pin = PB10, sleepPin = PB3, faultPin = PA6; 


void setup() {

    // Begin Serial port
    Serial.begin(115200);
    delay(5000);
    Serial.println("Initializing pins");

    // Assign pin modes
    pinMode(in1Pin, OUTPUT);         // Set EN/IN1 pin to OUTPUT
    pinMode(in2Pin, OUTPUT);         // Set PH/IN1 pin to OUTPUT
    pinMode(sleepPin, OUTPUT);       // Set !SLEEP pin to OUTPUT
    // pinMode(faultPin, INPUT_PULLUP); // Set !FAULT pin to INPUT

    // Set up driver to run motors
    digitalWrite(sleepPin, HIGH);    // Set !SLEEP to HIGH
    digitalWrite(in1Pin, HIGH);      // Set EN to HIGH
    digitalWrite(in2Pin, LOW);       // Set PH to LOW

    Serial.println("Ready to start program");
    
}
 
void loop() {
   

    // See what the !FAULT pin is reading
    // Serial.print("Fault pin reads: ");
    // Serial.println(digitalRead(faultPin));

    // Run motors in one direction
    Serial.println("run motors");
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    delay(1000); // Pause

    // Turn off motor
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    delay(1000); // Pause

    // Run motors in opposite direction
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    delay(1000); // Pause
}