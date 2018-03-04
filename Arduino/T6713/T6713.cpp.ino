#include "T6713.h"

T6713::T6713() {
    CO2ppmValue = 0;
    Serial.println("Application Note: Particle Photon T6713_I2C");
}

T6713::~T6713() {
  // TODO Auto-generated destructor stub
}

void T6713::run_co2_sensor(void) {
    read_count = 1;
    is_average_taken = false;
    while(!is_average_taken){read_co2();}
}

void T6713::read_from_sensor(void) {
    Wire.beginTransmission(ADDR_6713); // start I2C
    Wire.write(0x04);
    Wire.write(0x13);
    Wire.write(0x8B);
    Wire.write(0x00);
    Wire.write(0x01);
    Wire.endTransmission(); // end transmission
    
    // read report of current gas measurement in ppm
    delay(250);
    Wire.requestFrom(ADDR_6713, 4); // request 4 bytes from slave device
    
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    CO2ppmValue = data[2] * 256 + data[3];
}

void T6713::read_co2(void) {
    CO2ppmValue = 0;
    read_from_sensor();
    print_co2();
    save_co2_values();
    print_average_co2();
    delay(1000);
}

void T6713::save_co2_values(void) {
    if(CO2ppmValue > 0 && read_count <= MAX_READ_COUNT) {
        co2_buf[read_count] = CO2ppmValue;
        read_count ++;
    }
}

void T6713::print_average_co2(void) {
    if(read_count > MAX_READ_COUNT) {
        co2_average = 0;
        for(int k = 0; k < MAX_READ_COUNT; k++) {co2_average += co2_buf[k];}
      co2_average = co2_average / MAX_READ_COUNT;
      Serial.print("CO2 PPM Average Reading: ");
      Serial.println(co2_average);
      read_count = 1;
      is_average_taken = true;
    }
}

void T6713::print_co2(void) {
  if(DEBUG == true) {optional_print();}
  
  if (CO2ppmValue > 0) {
    Serial.print("Reading #: ");
    Serial.print(read_count);
    Serial.print(": CO2 Value: ");
    Serial.println(CO2ppmValue);
  }
  else {Serial.println("T6713 Checksum failed / Communication failure");}
}

void T6713::optional_print(void) {
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

bool T6713::start_sensor(void) {
    for(int k = 0; k < 5; k++){
        read_from_sensor();
        if(CO2ppmValue > 0){return true;}
        delay(1000);
    }
    Serial.println("Failed to start CO2 sensor");
    return false;
}
