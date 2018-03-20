#include "MHZ19.h"

MHZ19 myCO2;

int co2_transistor_control = A3;

bool start_co2 = false;

void setup() {
    Serial.begin(9600);
    pinMode(co2_transistor_control,OUTPUT);
    Serial.println("-- Initializing...");

    Serial.println("Trying to start CO2 sensor");
    delay(1000);
    digitalWrite(co2_transistor_control, HIGH);
    start_co2 = myCO2.start_sensor();
    Serial.println("-----------------------");
    digitalWrite(co2_transistor_control, LOW);

    delay(1000);
}

void loop() {
    if(start_co2 == true) {
        digitalWrite(co2_transistor_control, HIGH);
        start_co2 = myCO2.run_sensor();
        digitalWrite(co2_transistor_control, LOW);
        delay(1000);
    }
}

