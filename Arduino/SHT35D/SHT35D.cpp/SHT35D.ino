#include <Wire.h>
#include "SHT35D.h"

ClosedCube_SHT31D mySHT;

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("ClosedCube SHT3X-D Periodic Mode Example");
  Serial.println("supports SHT30-D, SHT31-D and SHT35-D");

  mySHT.begin(0x44); // I2C address: 0x44 or 0x45

  Serial.print("Serial #");
  Serial.println(mySHT.readSerialNumber());

  if (mySHT.periodicStart(SHT3XD_REPEATABILITY_HIGH, SHT3XD_FREQUENCY_10HZ) != SHT3XD_NO_ERROR)
    Serial.println("[ERROR] Cannot start periodic mode");
}

void loop()
{
  printResult("Periodic Mode", mySHT.periodicFetchData());
  delay(250);
}

void printResult(String text, SHT31D result) {
  if (result.error == SHT3XD_NO_ERROR) {
    Serial.print(text);
    Serial.print(": T=");
    Serial.print(result.t);
    Serial.print("C, RH=");
    Serial.print(result.rh);
    Serial.println("%");

  } else {
    Serial.print(text);
    Serial.print(": [ERROR] Code #");
    Serial.println(result.error);
  }
}
