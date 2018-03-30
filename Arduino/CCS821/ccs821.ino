#include "CCS821.h"

int voc_transistor_control = D6;
bool start_voc = false;

Adafruit_CCS811 myVOC;

void setup() {
    Serial.begin(9600);
    pinMode(voc_transistor_control,OUTPUT);
    delay(1000);
    digitalWrite(voc_transistor_control, HIGH);
    start_voc = myVOC.start_voc();
    Serial.println("-----------------------");
    digitalWrite(voc_transistor_control, LOW);
}

void loop() {
if(start_voc) {
        digitalWrite(voc_transistor_control, HIGH);
        Serial.println("Reading from VOC Sensor");
        Serial.println("-----------------------");
        start_voc = myVOC.run_voc();
        Serial.println("-----------------------");
        digitalWrite(voc_transistor_control, LOW);
    }
    else if(!start_voc) {
        Serial.println("Not reading from VOC Sensor");
        Serial.println("---------------------------");
        delay(500);
    }
}
