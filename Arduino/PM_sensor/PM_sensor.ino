#include "PM.h"
#include "Time.h"

int pm_transistor_control = A4;
int pm_tx_transistor_control = A5;
bool pm_has_been_read = false;
PM_7003 myPM;

void setup() {
    Serial.begin(9600);
    myPM.set_transistor(pm_transistor_control, pm_tx_transistor_control);
    Serial.println("-- Initializing --");
    delay(1000);
}

void loop() {
    pm_has_been_read = myPM.make_sensor_read();
    
    if(pm_has_been_read) {
        Serial.println("Done reading from PMS sensor.");
    }
    else if(!pm_has_been_read) {
        Serial.println("Not done reading from PMS sensor.");
    }
    
    delay(1000);
}

