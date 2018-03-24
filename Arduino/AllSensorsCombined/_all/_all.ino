#include "CALCULATE_MRT.h"
#include "MHZ19.h"
#include "CCS821.h"
#include "SHT35D.h"
#include "MRT.h"
#include "PM.h"
#include <Wire.h>

// creae instances of objects
PM_7003 myPM;
ClosedCube_Si7051 myMRT;
ClosedCube_SHT31D mySHT;
Adafruit_CCS811 myVOC;
MHZ19 myCO2;
mrt_and_ot my_MRT_OT;

/*
 * Boolean expression
 * Indicate whether sensor has
 * been started succesfully
 */
bool start_co2 = false;
bool start_voc = false;
bool start_sht = false;
bool start_pm = false;
bool start_mrt = false;

// average reading values
int co2_ave = -1;
float sht_rh_ave = -1;
float sht_t_ave = -1;
float voc_eCO2_ave = -1;
float voc_TVOC_ave = -1;
float pm_ave = -1;
float T_g = -1;
float T_a = -1;
float T_mrt = -1;
float T_ot = -1;

bool publish_data = true; // should we publish data?

// pin numbers for pm and co2 sensors
int pm_transistor_control = A4;
int co2_transistor_control = A3;

void setup() {
    /*
     * start Serial and wire connections
     * try to start each sensor
     * assign true false to each of the relevant booleans
     */
    Serial.begin(9600);
    Wire.begin();
    pinMode(pm_transistor_control,OUTPUT);
    pinMode(co2_transistor_control,OUTPUT);
    Serial.println("Initializing");
    
    Serial.println("Trying to start CO2 sensor");
    delay(1000);
    digitalWrite(co2_transistor_control, HIGH);
    start_co2 = myCO2.start_sensor();
    Serial.println("-----------------------");
    digitalWrite(co2_transistor_control, LOW);
    
    start_mrt = myMRT.start_mrt();
    Serial.println("-----------------------");
    start_sht = mySHT.start_sht();
    Serial.println("-----------------------");
    
    start_voc = myVOC.start_voc();
    Serial.println("-----------------------");

    Serial.println("Trying to start PM sensor");
    digitalWrite(pm_transistor_control, HIGH);
    delay(1000);
    start_pm = myPM.run_PM_sensor();
    digitalWrite(pm_transistor_control, LOW);
    
    if(start_pm){Serial.println("Successfully started PM sensor");}
    else if(!start_pm){Serial.println("Failed to start PM sensor");}
    Serial.println("-----------------------");
}

void loop() {
    /*
     * Run each sensor if it has been started
     * If the sensor has not been started, print error message
     * After all values have been read, prepare to publish data
     */
    if(start_co2) {
        digitalWrite(co2_transistor_control, HIGH);
        delay(1800);
        start_co2 = myCO2.run_sensor();
        digitalWrite(co2_transistor_control, LOW);
        delay(1000);
    }
    
    if(start_pm) {
        Serial.println("Reading from PMS Sensor");
        Serial.println("-----------------------");
        digitalWrite(pm_transistor_control, HIGH);
        delay(10000);
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
    
    if(publish_data) {
        char data[1000];
        if(start_mrt && start_sht){
            T_g = myMRT.get_MRT_ave();
            T_a = mySHT.get_t_ave();
            sht_rh_ave = mySHT.get_rh_ave();
            my_MRT_OT.calculate_mrt_and_ot(T_g, T_a);
            T_mrt = my_MRT_OT.get_mrt();
            T_ot = my_MRT_OT.get_ot();
        }
        else if(start_mrt && !start_sht) {
            T_g = myMRT.get_MRT_ave();
            T_a = -1;
            sht_rh_ave = -1;
            T_mrt = -1;
            T_ot = -1;
        }
        else if(!start_mrt && start_sht) {
            T_g = -1;
            T_a = mySHT.get_t_ave();
            sht_rh_ave = mySHT.get_rh_ave();
            T_mrt = -1;
            T_ot = -1;
        }
        else {
            T_g = -1;
            T_a = -1;
            sht_rh_ave = -1;
            T_mrt = -1;
            T_ot = -1;
        }
        
        if(start_pm){pm_ave = myPM.getpm();}
        else {pm_ave = -1;}
        
        if(start_co2){co2_ave = myCO2.get_co2_ave();}
        else{co2_ave = -1;}
        
        if(start_voc){
            voc_eCO2_ave = myVOC.get_eCO2_ave();
            voc_TVOC_ave = myVOC.get_TVOC_ave();
        } else {
            voc_eCO2_ave = -1;
            voc_TVOC_ave = -1;
        }
        
        
        //sprintf(data,"{ \"Mean Radiant Temperature\": \"%f\", \"Operating Temperature\": \"%f\", \"Globe Temperature\": \"%f\", \"CO2 Concentration\": \"%i\", \"VOC Equivalent CO2 Concentration\": \"%i\",\"TVOC\": \"%i\",\"PM 2.5 (Counts/m^3)\": \"%f\", \"Air Temperature\": \"%f\",\"Relative Humidity of Air\": \"%f\"}" , T_mrt, T_ot, T_g, co2_ave, voc_eCO2_ave, voc_TVOC_ave, pm_ave, T_a, sht_rh_ave);
        sprintf(data,"{ \"Mean Radiant Temperature\": \"%f\", \"Operating Temperature\": \"%f\", \"Globe Temperature\": \"%f\", \"CO2 Concentration\": \"%i\", \"TVOC\": \"%f\",\"PM 2.5 (Counts/m^3)\": \"%f\", \"Air Temperature\": \"%f\",\"Relative Humidity of Air\": \"%f\"}" , T_mrt, T_ot, T_g, co2_ave, voc_TVOC_ave, pm_ave, T_a, sht_rh_ave);
        Serial.println(data);
        Particle.publish("IEQ Data", data, PRIVATE);
           
    }
}

