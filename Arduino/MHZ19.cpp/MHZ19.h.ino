#ifndef MHZ19_H
#define MHZ19_H
#define MHZ19_ZEROTH_BYTE 0xFF
#define MHZ19_FIRST_BYTE 0x86
#define MAX_FRAME_LEN 9
#define NUMBER_OF_VALUES 20
#define DISCARD_VALUES 10
#define STARTUP_TIME 10
#define MAX_FRAME_READ_COUNT 40
#include "WProgram.h"


class MHZ19 {
    public:
        MHZ19();
        virtual ~MHZ19();
        int get_co2_reading(void);
        bool run_sensor(void);
        bool start_sensor(void);
        int get_co2_ave(void);
    
    private:
        char frame_buffer[MAX_FRAME_LEN];
        const uint8_t mhz19_read_command[MAX_FRAME_LEN] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};;
        
        bool sync_state;
        bool does_sensor_work;
        bool is_average_taken;
        
        int frame_sync_count;
        int frame_read_count;
        int byte_sum;
        int current_byte;
        int drain;
        int co2_ppm;
        int co2_ppm_average;
        int reading_count;
        int mhz19_buffer[NUMBER_OF_VALUES];
        
        void frame_sync(void);
        void read_sensor(void);
        void serial_drain(void);
        void fill_frame_buffer(void);
        void add_to_ave_buf(void);
        void print_current_reading(void);
        void calculate_average_reading(void);
        void print_average_reading(void);
        void take_average(void);
        void start_countdown(int start_time);
};

#endif /* MHZ19_H_ */
