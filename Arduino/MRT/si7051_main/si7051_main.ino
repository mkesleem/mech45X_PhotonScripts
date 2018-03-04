// This #include statement was automatically added by the Particle IDE.
#include "ClosedCube_Si7051.h"

/**************************************************************************************
This is example for ClosedCube Si7051 ±0.1°C Digital Temperature Sensor breakout board

Initial Date: 15-May-2016

Hardware connections for Arduino Uno:
  VDD to 3.3V DC
  SCL to A5
  SDA to A4
  GND to common ground

Written by AA for ClosedCube

MIT License
**************************************************************************************/

#include <Wire.h>
#include "ClosedCube_Si7051.h"

ClosedCube_Si7051 myMRT;
bool start_mrt = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("ClosedCube Si7051 Arduino Test");

  start_mrt = myMRT.start_mrt();
}

void loop()
{
  if(start_mrt) {
      myMRT.run_mrt();
      delay(1000);
  }
  else if(!start_mrt) {
      Serial.println("MRT not working");
      delay(1000);
    }
}

