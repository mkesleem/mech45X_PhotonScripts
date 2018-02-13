#include "T6713.h"

T6713::T6713() {
    CO2ppmValue = 0;
    Serial.println("Application Note: Particle Photon T6713_I2C");
}

T6713::~T6713() {
  // TODO Auto-generated destructor stub
}

void T6713::read_C02() {
  Wire.beginTransmission(ADDR_6713); // start I2C
  Wire.write(0x04);
  Wire.write(0x13);
  Wire.write(0x8B);
  Wire.write(0x00);
  Wire.write(0x01);
  Wire.endTransmission(); // end transmission

  // read report of current gas measurement in ppm
  delay(read_delay);
  Wire.requestFrom(ADDR_6713, 4); // request 4 bytes from slave device

  data[0] = Wire.read();
  data[1] = Wire.read();
  data[2] = Wire.read();
  data[3] = Wire.read();
  CO2ppmValue = data[2] * 256 + data[3];
  return;
}

void T6713::optional_print() {
  Serial.print("Func code: ");
  Serial.print(data[0],HEX);
  Serial.print(" byte count: ");
  Serial.println(data[1],HEX);
  Serial.print("MSB: 0x");
  Serial.print(data[2],HEX);
  Serial.print("  ");
  Serial.print("LSB: 0x");
  Serial.print(data[3],HEX);
}

void T6713::print_co2(void) {
  if(DEBUG == true) {optional_print();}
  
  if (CO2ppmValue > 0) {
    Serial.print("T6713 CO2 Value: ");
    Serial.println(CO2ppmValue);
  }
  else {Serial.println("T6713 Checksum failed / Communication failure");}
  
}

