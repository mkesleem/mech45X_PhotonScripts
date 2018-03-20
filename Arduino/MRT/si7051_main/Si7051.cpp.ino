#include <Wire.h>
#include "MRT.h"

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

float ClosedCube_Si7051::readT() {
    return readTemperature();
}

float ClosedCube_Si7051::readTemperature() {
    Wire.beginTransmission(_address);
    Wire.write(0xF3);
    Wire.endTransmission();
    
    delay(15);
    
    Wire.requestFrom(_address, (uint8_t)2);
    delay(25);
    byte msb = Wire.read();
    byte lsb = Wire.read();
    
    uint16_t val = msb << 8 | lsb;
    
    return (175.72*val) / 65536 - 46.85;
}

float ClosedCube_Si7051::run_mrt(void) {
    read_count = 1;
    
    while(read_count <= MAX_READ_COUNT) {
        T_buf[read_count - 1] = readTemperature();
        Serial.print("Reading #");
        Serial.print(read_count);
        Serial.print(": Tg is: ");
        Serial.println(T_buf[read_count - 1]);
        read_count ++;
        delay(250);
    }
    if(read_count > MAX_READ_COUNT) {
        T_ave = 0;
        for(int k = 0; k < MAX_READ_COUNT; k++) {
            T_ave = T_ave + T_buf[k];
        }
        T_ave = T_ave / MAX_READ_COUNT;
        Serial.print("Average Tg is: ");
        Serial.println(T_ave);
        return(T_ave);
    }
}

bool ClosedCube_Si7051::start_mrt(void) {
    begin(ADDR_MRT);
    delay(500);
    if(run_mrt() > DEFAULT_AVERAGE) {
        Serial.println("Failed to start MRT sensor!");
        return false;
    } else {
        Serial.println("Successfully started MRT sensor!");
        return true;
    }
}

float ClosedCube_Si7051::get_MRT_ave(void) {
    return T_ave;
}
