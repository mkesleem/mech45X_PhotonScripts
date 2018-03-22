#include <Wire.h>
#include "MRT.h"

//*********************************************************// 
// Part 1: Si7051 online library                           // 
// The following code was not written by MECH 45X Team 26  // 
// This section of the code was retrieved from:            // 
// https://github.com/closedcube/ClosedCube_Si7051_Arduino //
// This code will not be explained by team 26              // 
// This section of the code is too advanced for team 26 to // 
// understand.                                             // 
// Part 2 explains the code written by Team 26             // 
//*********************************************************// 

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

//*********************************************************//
// Part 2: Si7051 MECH 45X Team 26 library                 //
// The following code was written by MECH 45X Team 26      //
// It is properly commented                                //
//*********************************************************//


bool ClosedCube_Si7051::start_mrt(void) {
    /*
     * Start MRT sensor
     * 
     * The code will read a value of 128 or greater
     * if the sensor is broken or disconnected
     * 
     * The start sequence returns false (sensor does not work)
     * if a value of 128 is read
     * 
     * If the value is less than 128, it returns true
     * (sensor works)
     * 
     * The code retrieved from the online library should be improved
     * to fix this.
     */
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

float ClosedCube_Si7051::run_mrt(void) {
    /*
     * Takes MRT measurements until read_count is exceeded
     * once read_count is exceeded, the average is taken
     */
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

// Getter function for MRT average value
float ClosedCube_Si7051::get_MRT_ave(void) {return T_ave;}
