#include <Arduino.h>

uint8_t in1Pin = PB4, in2Pin = PB5, sleepPin = PA_5; // enablePin = ?


void setup() {
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(sleepPin, OUTPUT);
    // pinMode(enablePin, OUTPUT);

    // digitalWrite(enablePin, HIGH);
    digitalWrite(sleepPin, HIGH);

    Serial.begin(115200);
    delay(5000);
    Serial.println("ready");
    digitalWrite(in2Pin, LOW);
    
}
 
void loop() {

    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    delay(2000);
    digitalWrite(in1Pin, 1);
    digitalWrite(in2Pin, 0);
    delay(2000);

}