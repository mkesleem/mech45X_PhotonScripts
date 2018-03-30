#include <Wire.h>
#include "SHT35D.h"
bool start_sht = false;

ClosedCube_SHT31D mySHT;

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    Serial.println("Tring to start SHT");
    start_sht = mySHT.start_sht();
    Serial.println("-----------------------");
}

void loop() {
    if(start_sht) {
        Serial.println("Reading from SHT Sensor");
        Serial.println("-----------------------");
        mySHT.run_sht();
        Serial.println("-----------------------");
    }
    else if(!start_sht) {
        Serial.println("---------------------------");
        Serial.println("Not reading from SHT Sensor");
        Serial.println("---------------------------");
    }
    delay(500);
}

