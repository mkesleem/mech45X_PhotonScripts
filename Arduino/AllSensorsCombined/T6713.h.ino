// CO2 Meter Amphenol T6713 i2c Example Interface

// By Marv Kausch, 12/2016 at CO2 Meter <co2meter.com>
// Talks via I2C to T6713 sensors and displays CO2 values
// We will be using the I2C hardware interface on the Arduino in
// combination with the built-in Wire library to interface.
// Arduino analog input 5 - I2C SCL
// Arduino analog input 4 - I2C SDA
/*
In this example we will do a basic read of the CO2 value and checksum verification.
For more advanced applications please see the I2C Comm guide.
*/

#define LIB_T6713_H

#include <Wire.h>
#define ADDR_6713  0x15 // default I2C slave address
#define MAX_READ_COUNT 5

class T6713 {
    public:
        T6713();
        virtual ~T6713();
        bool start_sensor(void);
        void run_co2_sensor(void);

        
    private:
        int data[4];
        const bool DEBUG = false;
        int CO2ppmValue;

        int co2_average;
        int co2_buf[MAX_READ_COUNT];
        int read_count;
        bool is_average_taken;

        void print_co2(void);
        void read_co2(void);
        void save_co2_values(void);
        void print_average_co2(void);
        
        void read_from_sensor(void);
        void optional_print(void);
};

