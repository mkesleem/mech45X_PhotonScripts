#include <Wire.h>
#include "SHT35D.h"

ClosedCube_SHT31D mySHT;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  mySHT.start_SHT();
}

void loop()
{
    mySHT.printResult("Periodic Mode", mySHT.periodicFetchData());
    delay(250);
}

