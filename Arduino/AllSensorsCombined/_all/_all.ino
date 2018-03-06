// This #include statement was automatically added by the Particle IDE.
#include "calculate_mrt.h"

#include "PM.h"
#include "T6713.h"
#include "SHT35D.h"
#include "Adafruit_CCS811.h"
#include "Si7051.h"
#include <Wire.h>

// creae instances of objects
T6713 myCO2;
Adafruit_CCS811 myVOC;
ClosedCube_SHT31D mySHT;
PM_7003 myPM;
ClosedCube_Si7051 myMRT;

// boolean expressions
bool start_co2 = false;
bool start_voc = false;
bool start_sht = false;
bool start_pm = false;
bool start_mrt = false;

// pins numbers
int pm_transistor_control = A4;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    pinMode(pm_transistor_control,OUTPUT);
    Serial.println("Initializing");
    Serial.println("-----------------------");
    start_co2 = myCO2.start_sensor();
    Serial.println("-----------------------");
    start_voc = myVOC.start_voc();
    Serial.println("-----------------------");
    start_sht = mySHT.start_sht();
    Serial.println("-----------------------");
    
    Serial.println("Trying to start PM sensor");
    digitalWrite(pm_transistor_control, HIGH);
    delay(1000);
    start_pm = myPM.run_PM_sensor();
    digitalWrite(pm_transistor_control, LOW);
    
    if(start_pm){Serial.println("Successfully started PM sensor");}
    else if(!start_pm){Serial.println("Failed to start PM sensor");}
    Serial.println("-----------------------");
    
    start_mrt = myMRT.start_mrt();
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
    else if(!start_co2) {
        Serial.println("Not reading from CO2 Sensor");
        Serial.println("---------------------------");
        delay(500);
    }
    
    if(start_voc) {
        Serial.println("Reading from VOC Sensor");
        Serial.println("-----------------------");
        myVOC.run_voc();
        Serial.println("-----------------------");
    }
    else if(!start_voc) {
        Serial.println("Not reading from VOC Sensor");
        Serial.println("---------------------------");
        delay(500);
    }
    
    if(start_sht) {
        Serial.println("Reading from SHT Sensor");
        Serial.println("-----------------------");
        mySHT.run_sht();
        Serial.println("-----------------------");
    }
    else if(!start_sht) {
        Serial.println("Not reading from SHT Sensor");
        Serial.println("---------------------------");
        delay(500);
    }
    
    if(start_pm) {
        Serial.println("Reading from PMS Sensor");
        Serial.println("-----------------------");
        digitalWrite(pm_transistor_control, HIGH);
        delay(500);
        start_pm = myPM.run_PM_sensor();
        digitalWrite(pm_transistor_control, LOW);
        delay(500);
    }
    else if(!start_pm) {
        Serial.println("Not reading from PMS Sensor");
        Serial.println("---------------------------");
        delay(500);
    }
    
    if(start_mrt) {
        myMRT.run_mrt();
        delay(500);
    }
    else if(!start_mrt) {
        Serial.println("Not reading from MRT Sensor");
        Serial.println("---------------------------");
        delay(500);
    }
}

