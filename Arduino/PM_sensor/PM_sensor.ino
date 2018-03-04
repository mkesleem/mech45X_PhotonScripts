#include "PM.h"

int fan = D3;
int i = 2500;
int led = D7;
int ramp = 1;
int freq = 10; //this frequency must be sufficiently low to allow the transistor to pulse the fan, 10 is reasonable
int pm_transistor_control = A4;
bool start_pm = false;
PM_7003 myPM;


void setup() {
    Serial.begin(9600);
    pinMode(pm_transistor_control,OUTPUT);
    Serial.println("-- Initializing --");
    pinMode(fan,OUTPUT);
    pinMode(led,OUTPUT);
    analogWriteResolution(fan,12); //this sets resolution to 12bit (0-4095)
    
    digitalWrite(pm_transistor_control, HIGH);
    delay(1000);
    start_pm = myPM.run_PM_sensor();
    digitalWrite(pm_transistor_control, LOW);
    
    if(start_pm){Serial.println("Successfully started PM sensor");}
    else if(!start_pm){Serial.println("Failed to start PM sensor");}
    
    delay(1000);
}

void loop() {
    analogWrite(fan,i,freq);
    
    if(start_pm) {
        digitalWrite(pm_transistor_control, HIGH);
        delay(1000);
        start_pm = myPM.run_PM_sensor();
        digitalWrite(pm_transistor_control, LOW);
        delay(2000);
    }
}

