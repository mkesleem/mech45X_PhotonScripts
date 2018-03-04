#include "T6713.h"
T6713 myT6713;
bool start_co2 = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  start_co2 = myT6713.start_sensor();
}

void loop()
{
  if(start_co2){myT6713.run_co2_sensor();}
}
