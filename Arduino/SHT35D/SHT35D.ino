#include <Wire.h>
#include "SHT35D.h"
bool sht_started;

ClosedCube_SHT31D mySHT;

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    sht_started = mySHT.start_SHT();
    Serial.print("Has SHT been started?");
    Serial.println(sht_started);
}

void loop()
{
    mySHT.printResult("Periodic Mode", mySHT.periodicFetchData());
    delay(250);
}

