#include <Wire.h>
#include "Si7051.h"

ClosedCube_Si7051 myMRT;

void setup()
{
  Serial.begin(9600);
  Serial.println("ClosedCube Si7051 Arduino Test");

  myMRT.begin(0x40); // default I2C address is 0x40
}

void loop()
{
  myMRT.readT();
  delay(1000);
}
