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

// boolean expressions
bool start_co2 = false;
bool start_voc = false;
bool start_sht = false;
bool start_pm = false;
bool start_mrt = false;
bool mhz19_average_check = false;

// average values
int co2_ave = -1;
float sht_rh_ave = -1;
float sht_t_ave = -1;
int voc_eCO2_ave = -1;
int voc_TVOC_ave = -1;
int voc_t_ave = -1;
float mrt_t_ave = -1;

// should we publish data
bool publish_data = true;

// pins numbers
int pm_transistor_control = A4;
int co2_transistor_control = A3;

void setup() {
    Serial.begin(9600);
    Wire.begin();
    pinMode(pm_transistor_control,OUTPUT);
    pinMode(co2_transistor_control,OUTPUT);
    Serial.println("Initializing");

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

void loop()
{
    digitalWrite(co2_transistor_control, HIGH);
    while(mhz19_average_check == false) {
        myCO2.run_MHZ19();
        mhz19_average_check = myCO2.transistor_on_off();
    }
    mhz19_average_check = false;
    digitalWrite(co2_transistor_control, LOW);
    
    
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
        if(start_mrt){
            mrt_t_ave = myMRT.get_MRT_ave();
            char data[1000];
            sprintf(data,"{ \"MRT Temperature\": \"%f\"}" , mrt_t_ave);
            Particle.publish("MRT Temperature", data, PRIVATE);
        }
        if(start_co2){
            co2_ave = myCO2.get_co2_ave();
            char data[1000];
            sprintf(data,"{ \"CO2 Concentration\": \"%i\"}" , co2_ave);
            Particle.publish("CO2 Concentration", data, PRIVATE);
        }
        
        if(start_voc){
            voc_eCO2_ave = myVOC.get_eCO2_ave();
            voc_TVOC_ave = myVOC.get_TVOC_ave();
            voc_t_ave = myVOC.get_temp_ave();
            
            char data[1000];
            sprintf(data,"{ \"VOC Equivalent CO2 Concentration\": \"%i\",\"TVOC\": \"%i\",\"VOC Temperature\": \"%i\" }" , voc_eCO2_ave,voc_TVOC_ave,voc_t_ave);
            Particle.publish("VOC Sensor", data, PRIVATE);
        }
        
        if(start_sht) {
            sht_rh_ave = mySHT.get_rh_ave();
            sht_t_ave = mySHT.get_t_ave();
            char data[1000];
            sprintf(data,"{ \"SHT Temperature\": \"%f\",\"SHT Humidity\": \"%f\" }" , sht_rh_ave,sht_t_ave);
            Particle.publish("SHT RH/T", data, PRIVATE);
        }
    }
}
