/*
 * MHZ19.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: michael
 */

#include "MHZ19.h"

MHZ19::MHZ19() {
  mhz19_ppm = 0;
}

MHZ19::~MHZ19() {
  // TODO Auto-generated destructor stub
}

int MHZ19::getCO2(void) {
  return mhz19_ppm;
}

void MHZ19::run_MHZ19(void) {
    mhz19_ppm = 0;
    memset(frame_buffer, 0, 9);
    read_sensor();
    
    if(mhz19_ppm > 0) {
      Serial.print("MHZ19 CO2 PPM: ");
      Serial.println(mhz19_ppm);
    }
    else {
      Serial.println("Error reading CO2 PPM from MHZ19");
    }
    delay(1000);
}

void MHZ19::read_sensor(void) {
    Serial1.begin(9600);
    Serial1.write(mhz19_read_command, 9);
    serial_drain();
    delay(1000);
    fill_buffer();
    mhz19_ppm = 256*frame_buffer[2] + frame_buffer[3];
    Serial1.end();
}

void MHZ19::serial_drain(void) {
  if (Serial1.available() > 32) {
    drain = Serial1.available();
    Serial.print("-- Draining buffer: ");
    Serial.println(Serial1.available(), DEC);
    for (int i = drain; i > 0; i--) {
        Serial1.read();
    }
  }
}

void MHZ19::frame_sync(void) {
    sync_state = false;
    frame_count = 0;
    byte_sum = 0;
    
    while (!sync_state && Serial1.available() > 0) {
        current_byte = Serial1.read();
    
        if (current_byte == MHZ19_ZEROTH_BYTE && frame_count == 0) {
            frame_buffer[frame_count] = current_byte;
            byte_sum = current_byte;
            frame_count = 1;
        }
        else if (current_byte == MHZ19_FIRST_BYTE && frame_count == 1) {
            frame_buffer[frame_count] = current_byte;
            byte_sum += current_byte;
            sync_state = true;
            frame_count = 2;
        }
        else {
            Serial.print("-- Frame syncing... ");
            Serial.println(current_byte, HEX);
            Serial.println(Serial1.available());
        }
    }
}

void MHZ19::fill_buffer(void) {
  frame_sync();
  
  while(sync_state && Serial1.available() > 0 && frame_count < MAX_FRAME_LEN) {
    current_byte = Serial1.read();
    frame_buffer[frame_count] = current_byte;
    byte_sum += current_byte;
    frame_count++;
  }
}
