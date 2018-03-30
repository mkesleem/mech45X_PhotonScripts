#include "Time.h"
#include "MHZ19.h"

MHZ19 myCO2;

int co2_transistor_control = A3;

bool start_co2 = false;

void setup() {
    Serial.begin(9600);
    myCO2.set_transistor(co2_transistor_control);
    Serial.println("-- Initializing...");
    delay(1000);
}

void loop() {
    start_co2 = myCO2.make_sensor_read();
    delay(500);
}

