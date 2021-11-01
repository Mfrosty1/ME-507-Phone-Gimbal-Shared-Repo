#include <Arduino.h>

uint8_t in1Pin = PB4, in2Pin = PB10, sleepPin = PB3, faultPin = PA6; 


void setup() {
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(sleepPin, OUTPUT);
    // pinMode(enablePin, OUTPUT);
    pinMode(faultPin, INPUT_PULLUP);

    // digitalWrite(enablePin, HIGH);
    digitalWrite(sleepPin, HIGH);

    // digitalWrite(in1Pin, HIGH);
    // digitalWrite(in2Pin, LOW);

    Serial.begin(115200);
    delay(5000);
    Serial.println("ready");

    
}
 
void loop() {
    Serial.print("Fault pin reads: ");
    Serial.println(digitalRead(faultPin));

    Serial.println("run motors");
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    delay(1000);
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    delay(1000);
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, HIGH);
    delay(1000);
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    delay(1000);
}