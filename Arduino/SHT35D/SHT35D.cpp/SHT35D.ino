#include <Wire.h>
#include "SHT35D.h"

ClosedCube_SHT31D mySHT;
bool SHT_done_reading;


void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("ClosedCube SHT3X-D Periodic Mode Example");
  Serial.println("supports SHT30-D, SHT31-D and SHT35-D");

  mySHT.begin(0x44); // I2C address: 0x44 or 0x45
  mySHT.initilze_values();

  Serial.print("Serial #");
  Serial.println(mySHT.readSerialNumber());

  if (mySHT.periodicStart(SHT3XD_REPEATABILITY_HIGH, SHT3XD_FREQUENCY_10HZ) != SHT3XD_NO_ERROR)
    Serial.println("[ERROR] Cannot start periodic mode");
  
  SHT_done_reading = false;
}

void loop()
{
  
  while(!SHT_done_reading) {
      mySHT.printResult("Periodic Mode", mySHT.periodicFetchData());
      SHT_done_reading = mySHT.check_done_reading();
      delay(250);    
  }
  if(SHT_done_reading) {
      Serial.println("SHT DONE READING!");
      SHT_done_reading = false;
  }
}

