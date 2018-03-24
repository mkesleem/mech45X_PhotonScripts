#include "CCS821.h"

Adafruit_CCS811 ccs;
bool voc_started;

void setup() {
    Serial.begin(9600);
    voc_started = ccs.start_voc();
}

void loop() {
    if(voc_started) {
        ccs.run_voc();
        Serial.println(ccs.get_eCO2_ave());
        Serial.println(ccs.get_TVOC_ave());
        Serial.println(ccs.get_temp_ave());
    }
    else{Serial.print("EPIC FAIL!");}
}
