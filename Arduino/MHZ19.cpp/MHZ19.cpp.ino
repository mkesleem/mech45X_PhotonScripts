#include "MHZ19.h"
#include "Time.h"

//*********************************************************// 
// All of this code has been written by MECH 45X Team 26   // 
// It has been properly commented                          //
//*********************************************************// 

MHZ19::MHZ19() {
    first_time = true;
}

MHZ19::~MHZ19() {
}

void MHZ19::set_transistor(int pin) {
    co2_transistor_control = pin;
    pinMode(co2_transistor_control,OUTPUT);
}

void MHZ19::begin_timer(void) {
    co2_ppm_average_uncalibrated = -1;
    co2_ppm_average_calibrated = -1;
    digitalWrite(co2_transistor_control, HIGH);
    start_time = now();
    Serial.println("--------------");
    Serial.print("CO2 start time: ");
    Serial.println(start_time);
    Serial.println("--------------");
    first_time = false;
}

bool MHZ19::check_begin_reading(void) {
    current_time = now();
    duration = current_time - start_time;
    Serial.println("-----------------");
    Serial.print("CO2 Duration: ");
    Serial.println(duration);
    Serial.println("-----------------");
    
    if(duration >= CO2_START_UP_TIME) {
        Serial.println("Three minutes have elapsed since starting CO2 sensor!");
        return(true);
    } else{return(false);}
}

bool MHZ19::make_sensor_read(void) {
    if(first_time) {
        function_call_count = 0;
        begin_timer();
        return(false);
    }
    else if(function_call_count < MAX_FUNCTION_CALL_COUNT) {
        if(check_begin_reading()) {
            Serial.println("---------------------");
            Serial.print("Function Call Count: ");
            Serial.println(function_call_count);
            Serial.println("---------------------");
            run_sensor();
            function_call_count ++;
        } else {return(false);}
    }
    
    
    if(function_call_count >= MAX_FUNCTION_CALL_COUNT) {
        first_time = true;
        digitalWrite(co2_transistor_control, LOW);
        return(true);
    } else{return(false);}
}

void MHZ19::calibrate_sensor(void) {
    if(first_time) {
        function_call_count = 0;
        begin_timer();
    }

    if(check_begin_reading()) {
        Serial.println("---------------------");
        Serial.print("Function Call Count: ");
        Serial.println(function_call_count);
        Serial.println("---------------------");
        run_sensor();
        function_call_count ++;
    }
}

bool MHZ19::run_sensor(void) {
    /*
     * Run the MHZ19 sensor
     * Set ppm to zero
     * clear the frame_buffer
     * drain the serial buffer
     * read from the sensor
     * print reading
     * add the reading to the average value buffer
     * calculate average value
     */
    co2_ppm = -1;
    co2_ppm_average_uncalibrated = 0;
    co2_ppm_average_calibrated = 0;
    is_average_taken = false;
    does_sensor_work = true;
    reading_count = 1;

    serial_drain();
    
    while(is_average_taken == false && does_sensor_work == true) {
        memset(frame_buffer, 0, 9);
        read_sensor();
        print_current_reading();
        add_to_ave_buf();
        calculate_average_reading();
        print_average_reading();
        co2_ppm = -1;
    }
    if(is_average_taken == true) {return(true);}
    else {return(false);}
}

void MHZ19::print_current_reading(void) {
    /*
     * Prints current reading if reading is valid (i.e. co2_ppm > 0)
     * and if the maximum number of readings haven't been exceeded
     */
    if(co2_ppm > 0) {
      Serial.print("MHZ19 CO2 PPM Reading ");
      Serial.print(reading_count);
      Serial.print(": ");
      Serial.println(co2_ppm);
    }
    else {
      Serial.println("Error reading CO2 PPM from MHZ19");
    }
}

void MHZ19::add_to_ave_buf(void) {
    /*
     * IF a valid value of co2 is read and the number of reading is less than the max,
     * THEN add current value to buffer
     */
    if(co2_ppm > 0 && reading_count <= NUMBER_OF_VALUES) {
        mhz19_buffer[reading_count - 1] = co2_ppm;
        reading_count += 1;
    }
}

void MHZ19::calculate_average_reading(void) {
    /*
     * IF the number of readings exceeds the number of values to be read,
     * THEN calculate the average
     */
    if(reading_count > NUMBER_OF_VALUES) {
        for(int k = 0; k < NUMBER_OF_VALUES; k++) {co2_ppm_average_uncalibrated += mhz19_buffer[k];}
      
        co2_ppm_average_uncalibrated = co2_ppm_average_uncalibrated / ( NUMBER_OF_VALUES );
        is_average_taken = true;
    }
}

void MHZ19::apply_calibration_curve(void) {
    co2_ppm_average_calibrated = calib_a0 + co2_ppm_average_uncalibrated * calib_a1;
}

void MHZ19::print_average_reading(void) {
    /*
     * IF the average has been taken (co2_ppm_average > 0)
     * THEN print the average
     */
    if(co2_ppm_average_uncalibrated > 0) {
        Serial.println("-----------------------------");
        Serial.print("CO2 PPM Average Reading (Uncalibrated): ");
        Serial.println(co2_ppm_average_uncalibrated);
        apply_calibration_curve();
        Serial.print("CO2 PPM Average Reading (Calibrated): ");
        Serial.println(co2_ppm_average_calibrated);
        Serial.println("-----------------------------");
    }
}

void MHZ19::read_sensor(void) {
    /*  
     *  Start Serial1 connection
     *  Send command to read from sensor to the sensor
     *  Read from the sensor (fill_from_buffer();)
     *  Calculate PPM for CO2
     *  End Serial connection
    */  
    
    Serial1.begin(9600);
    Serial1.write(mhz19_read_command, 9);
    delay(1000);
    fill_frame_buffer();
    co2_ppm = 256*frame_buffer[2] + frame_buffer[3];
    Serial1.end();
}

void MHZ19::serial_drain(void) {
  /*
   * Drains serial buffer when sensor is turned on
   */
    while (Serial1.available() > 0) {
        drain = Serial1.available();
        Serial.print("-- Draining buffer: ");
    }
}

void MHZ19::frame_sync(void) {
    /*
     * Sync frames so that frames are added to the frame_buffer in the correct order
     * IF correct byte is read, THEN add to buffer and move on to next byte
     * ELSE read byte and discard
     * IF no bytes are available to read and the frames have not been synced, THEN send command to read from sensor again
     * 
     * frame_sync_count keeps track of how many frames are added to frame_buffer
     * frame_read_count keeps track of how many frames are read but not added to buffer (fails if too many frames read)
     */
    sync_state = false;
    frame_sync_count = 0;
    frame_read_count = 0;
    byte_sum = 0;
    
    while (!sync_state && Serial1.available() > 0 && frame_read_count < MAX_FRAME_READ_COUNT) {
        current_byte = Serial1.read();
    
        if (current_byte == MHZ19_ZEROTH_BYTE && frame_sync_count == 0) {
            frame_buffer[frame_sync_count] = current_byte;
            byte_sum = current_byte;
            frame_sync_count = 1;
        }
        else if (current_byte == MHZ19_FIRST_BYTE && frame_sync_count == 1) {
            frame_buffer[frame_sync_count] = current_byte;
            byte_sum += current_byte;
            sync_state = true;
            frame_sync_count = 2;
        }
        else {
            if(debug) {
                Serial.print("-- Frame syncing... ");
                Serial.println(current_byte, HEX);   
            }
            
            frame_read_count ++;
        }

        if (!sync_state && !(Serial1.available() > 0) && frame_read_count < MAX_FRAME_READ_COUNT) {
            Serial1.write(mhz19_read_command, 9);
            
            if(debug) {
                Serial.println("----------------------------------------");
                Serial.println("Read command has been sent to CO2 sensor");
                Serial.println("----------------------------------------");
            }
            
            delay(500);
        }        
    }
}

void MHZ19::fill_frame_buffer(void) {
  /*
   * Sync frames
   * Read byte into frame_buffer
   */
    frame_sync();
    
    while(sync_state && Serial1.available() > 0 && frame_sync_count < MAX_FRAME_LEN) {
        current_byte = Serial1.read();
        frame_buffer[frame_sync_count] = current_byte;
        byte_sum += current_byte;
        frame_sync_count++;
    }
}

// getter functions
int MHZ19::get_co2_ave_uncalibrated(void) {return co2_ppm_average_uncalibrated;}
int MHZ19::get_co2_ave_calibrated(void) {return co2_ppm_average_calibrated;}
int MHZ19::get_co2_reading(void) {return co2_ppm;}
void MHZ19::reset_co2_ave(void) {
    co2_ppm_average_uncalibrated = -1;
    co2_ppm_average_calibrated = -1;
}

