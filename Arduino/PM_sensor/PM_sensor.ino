#include "PM.h"

int fan = D3;
int i = 2500;
int led = D7;
int ramp = 1;
int freq = 10; //this frequency must be sufficiently low to allow the transistor to pulse the fan, 10 is reasonable
int pm_transistor_control = A4;
PM_7003 myPM;


void setup() {
    Serial.begin(9600);
    pinMode(pm_transistor_control,OUTPUT);
    Serial.println("-- Initializing --");
    pinMode(fan,OUTPUT);
    pinMode(led,OUTPUT);
    analogWriteResolution(fan,12); //this sets resolution to 12bit (0-4095)
    delay(1000);
}

void loop() {
    analogWrite(fan,i,freq);
    digitalWrite(pm_transistor_control, HIGH);
    delay(5000);
    myPM.run_PM_sensor();
    digitalWrite(pm_transistor_control, LOW);
    delay(5000);
}

