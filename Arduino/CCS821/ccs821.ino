#include "Adafruit_CCS811.h"

Adafruit_CCS811 ccs;
bool voc_started;

void setup() {
    Serial.begin(9600);
    voc_started = ccs.start_voc();
}

void loop() {
    if(voc_started) {ccs.run_voc();}
    else{Serial.print("EPIC FAIL!");}
}
