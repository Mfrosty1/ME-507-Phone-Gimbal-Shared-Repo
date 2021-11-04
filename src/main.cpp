#include <Arduino.h>
#include "DRV8256.h"




DRV8256 pitchMotor;

void setup() {

    // Begin Serial port
    Serial.begin(115200);
    delay(5000);
    Serial.println("Initializing pins");

    // PB5 = D4, PB4 = D5, PA5 = D13, PA6 = D12
    pitchMotor.attachMotor(PB5, PB4, PA5, PA6);     

    pitchMotor.motorForward(20);
    delay(2000);
    pitchMotor.motorForward(255);
    
    // delay(2000);
    // pitchMotor.motorStop();
    // delay(2000);
    pitchMotor.motorReverse(100);
    delay(2000);
    pitchMotor.motorReverse(255);
    delay(2000);
    pitchMotor.motorStop();

    
}
 
void loop() 
{

}