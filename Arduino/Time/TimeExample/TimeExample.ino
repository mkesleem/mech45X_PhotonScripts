#include "Time.h"
#define CO2_START_UP 180

time_t start_time;
time_t current_time;
time_t duration;

void setup() {
    Serial.begin(9600);
    Serial.print(now());
    start_time = now();
}

void loop() {
    current_time = now();
    Serial.print("Current Time: ");
    Serial.println(current_time);
    duration = current_time - start_time;
    if(duration > CO2_START_UP) {
        Serial.print("Duration: ");
        Serial.println(duration);    
        Serial.println("Three minutes have passed");
    }
    else {
        Serial.print("Duration: ");
        Serial.println(duratin);
    }
    delay(1000);
}
