#include "T6713.h"
#include "SHT35D.h"
#include "Adafruit_CCS811.h"
#include <Wire.h>

// creae instances of objects
T6713 myCO2;
Adafruit_CCS811 myVOC;
ClosedCube_SHT31D mySHT;

// boolean expressions
bool start_co2 = false;
bool start_voc = false;
bool start_sht = false;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    Serial.println("Initializing");
    Serial.println("-----------------------");
    start_co2 = myCO2.start_sensor();
    Serial.println("-----------------------");
    start_voc = myVOC.start_voc();
    Serial.println("-----------------------");
    start_sht = mySHT.start_sht();
    Serial.println("-----------------------");
}

void loop()
{
    if(start_co2) {
        Serial.println("Reading from CO2 Sensor");
        Serial.println("-----------------------");
        myCO2.run_co2_sensor();
        Serial.println("-----------------------");
    }
    if(start_voc) {
        Serial.println("Reading from VOC Sensor");
        Serial.println("-----------------------");
        myVOC.run_voc();
        Serial.println("-----------------------");
    }
    if(start_sht) {
        Serial.println("Reading from SHT Sensor");
        Serial.println("-----------------------");
        mySHT.run_sht();
        Serial.println("-----------------------");
    }
}

