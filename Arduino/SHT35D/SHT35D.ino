#include <Wire.h>
#include "SHT35D.h"
bool sht_started;

ClosedCube_SHT31D mySHT;

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    sht_started = mySHT.start_sht();
}

void loop()
{
    mySHT.run_sht();
}

