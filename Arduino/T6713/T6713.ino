#include "T6713.h"
T6713 myT6713;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop()
{
  myT6713.run_co2();
  delay(1000);
}
