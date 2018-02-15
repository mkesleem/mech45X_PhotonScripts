#include "PM.h"

PM_7003::PM_7003() {
  current_byte = 0;
  packetdata.frame_length = MAX_FRAME_LENGTH;
  frame_length = MAX_FRAME_LENGTH;
}

PM_7003::~PM_7003() {
  // TODO Auto-generated destructor stub
}

void PM_7003::run_PM_sensor(void) {
    Serial1.begin(9600);
    drain_serial();
    delay(1000);
    // delay(DELAY_TIME);
    read_sensor();
    Serial1.end();
}

void PM_7003::drain_serial(void) {
  /*
   * Drains serial buffer if there are more than 32 entries
   * Reads entries to drain serial buffer
   */
  if (Serial1.available() > 32) {
    drain = Serial1.available();
    Serial.println("-- Draining buffer: ");
    Serial.println(Serial1.available(), DEC);
    for (int drain_index = drain; drain_index > 0; drain_index--) {Serial1.read();}
  }
}

void PM_7003::frame_sync(void) {
  /*
   * syncs frames for PM sensor
   * checks that frames are being read in correct order
   * exits when it confirms that frames are being read correctly
  */
    sync_state = false;
    frame_count = 0;
    byte_sum = 0;

    while (!sync_state && Serial1.available() > 0){
      current_byte = Serial1.read();
    
      if(current_byte == FIRST_BYTE && frame_count == 0) {
        frame_buffer[frame_count] = current_byte;
        packetdata.start_frame[0] = current_byte;
        byte_sum = current_byte;
        frame_count = 1;
      }
      else if(current_byte == SECOND_BYTE && frame_count == 1){
        frame_buffer[frame_count] = current_byte;
        packetdata.start_frame[1] = current_byte;
        byte_sum = byte_sum + current_byte;
        frame_count = 2;
        sync_state = true;
      }
      else{Serial.println("frame is syncing");}
  }
}

void PM_7003::read_sensor(void) {
  frame_sync();

  while(sync_state == true && Serial1.available() > 0) {
    current_byte = Serial1.read();
    frame_buffer[frame_count] = current_byte;
    byte_sum = byte_sum + current_byte;
    frame_count++;
    uint16_t current_data = frame_buffer[frame_count-1]+(frame_buffer[frame_count-2]<<8);
    data_switch(current_data);
    
    if (frame_count >= frame_length) {
      print_messages();
      break;
    }
  }    
}

void PM_7003::data_switch(uint16_t current_data) {
    switch (frame_count) {
    case 4:  
        packetdata.frame_length = current_data;
        frame_length = current_data + frame_count;
      break;
    case 6:
      packetdata.concPM1_0_factory = current_data;
      break;
    case 8:
      packetdata.concPM2_5_factory = current_data;
      break;
    case 10:
      packetdata.concPM10_0_factory = current_data;
      break;
    case 12:
      packetdata.concPM1_0_ambient = current_data;
      break;
    case 14:
      packetdata.concPM2_5_ambient = current_data;
      break;
    case 16:
      packetdata.concPM10_0_ambient = current_data;
      break;
    case 18:
      packetdata.countPM0_3um = current_data;
      break;
    case 20:
      packetdata.countPM0_5um = current_data;
      break;
    case 22:
      packetdata.countPM1_0um = current_data;
      break;
    case 24:
      packetdata.countPM2_5um = current_data;
      break;
    case 26:
      packetdata.countPM5_0um = current_data;
      break;
    case 28:
      packetdata.countPM10_0um = current_data;
      break;
    case 29:
      current_data = frame_buffer[frame_count-1];
      packetdata.version = current_data;
      break;
    case 30:
      current_data = frame_buffer[frame_count-1];
      packetdata.error = current_data;
      break;
    case 32:
      packetdata.checksum = current_data;
      byte_sum -= ((current_data>>8)+(current_data&0xFF));
      break;
    default:
      break;
    }
}

void PM_7003::print_messages(void){
    Serial.println("Print Messages");
    sprintf(print_buffer, ", %02x, %02x, %04x, ",
        packetdata.start_frame[0], packetdata.start_frame[1], packetdata.frame_length);
    sprintf(print_buffer, "%s%04d, %04d, %04d, ", print_buffer,
        packetdata.concPM1_0_factory, packetdata.concPM2_5_factory, packetdata.concPM10_0_factory);
    sprintf(print_buffer, "%s%04d, %04d, %04d, ", print_buffer,
        packetdata.concPM1_0_ambient, packetdata.concPM2_5_ambient, packetdata.concPM10_0_ambient);
    sprintf(print_buffer, "%s%04d, %04d, %04d, %04d, %04d, %04d, ", print_buffer,
        packetdata.countPM0_3um, packetdata.countPM0_5um, packetdata.countPM1_0um,
        packetdata.countPM2_5um, packetdata.countPM5_0um, packetdata.countPM10_0um);
    sprintf(print_buffer, "%s%02d, %02d, ", print_buffer,
        packetdata.version, packetdata.error);
    Serial.println(print_buffer);
}

