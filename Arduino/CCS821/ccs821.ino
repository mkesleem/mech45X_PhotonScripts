#include "CCS821.h"

bool start_voc = false;

Adafruit_CCS811 myVOC;

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("--------------------------");
    Serial.println("Trying to start VOC sensor");
    Serial.println("--------------------------");
    start_voc = myVOC.start_voc();
}

void loop() {
if(start_voc) {
        Serial.println("Reading from VOC Sensor");
        Serial.println("-----------------------");
        start_voc = myVOC.run_voc();
        Serial.println("-----------------------");
    }
    else if(!start_voc) {
        start_voc = myVOC.start_voc();
        Serial.println("Reading from VOC Sensor");
        Serial.println("-----------------------");
        start_voc = myVOC.run_voc();
        Serial.println("-----------------------");
    }
    delay(500);
}
