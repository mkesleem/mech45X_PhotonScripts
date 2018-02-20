#include <Wire.h>
#include "Si7051.h"

ClosedCube_Si7051::ClosedCube_Si7051()
{
}

void ClosedCube_Si7051::begin(uint8_t address) {
  _address = address;
  Wire.begin();

  Wire.beginTransmission(_address);
  Wire.write(0xE6);
  Wire.write(0x0);
  Wire.endTransmission();
}

void ClosedCube_Si7051::readT() {
  Serial.print("T=");
  Serial.print(readTemperature());
  Serial.println("C");
}

float ClosedCube_Si7051::readTemperature() {
  Wire.beginTransmission(_address);
  Wire.write(0xF3);
  Wire.write(_address);
  Wire.write(_address);
  Wire.write(_address);
  Wire.endTransmission();

  delay(10);

  Wire.requestFrom(_address, (uint8_t)2);

  byte msb = Wire.read();
  byte lsb = Wire.read();

  uint16_t val = msb << 8 | lsb;

  return (175.72*val) / 65536 - 46.85;
}
