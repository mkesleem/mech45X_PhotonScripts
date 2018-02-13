// This #include statement was automatically added by the Particle IDE.
#include "T6713.h"
T6713 myT6713;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop()
{
  myT6713.print_co2();
  delay(2000);
}
