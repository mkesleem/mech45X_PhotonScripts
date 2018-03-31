#ifndef MHZ19_H
#define MHZ19_H
#define MHZ19_ZEROTH_BYTE 0xFF
#define MHZ19_FIRST_BYTE 0x86
#define MAX_FRAME_LEN 9
#define NUMBER_OF_VALUES 5
#define CO2_START_UP_TIME 10
#define MAX_FRAME_READ_COUNT 40
#define MAX_FUNCTION_CALL_COUNT 3
#include "WProgram.h"
#include "Time.h"


class MHZ19 {
    public:
        MHZ19();
        virtual ~MHZ19();
        int get_co2_reading(void);
        int get_co2_ave(void);
        void set_transistor(int pin);
        bool make_sensor_read(void);
        void calibrate_sensor(void);
        void reset_co2_ave(void);
    
    private:
        char frame_buffer[MAX_FRAME_LEN];
        const uint8_t mhz19_read_command[MAX_FRAME_LEN] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};;
        
        bool debug = false;
        
        bool sync_state;
        bool does_sensor_work;
        bool is_average_taken;
        bool first_time;
        int co2_transistor_control;
        
        int frame_sync_count;
        int frame_read_count;
        int byte_sum;
        int current_byte;
        int drain;
        int co2_ppm;
        int co2_ppm_average;
        int reading_count;
        int function_call_count;
        int mhz19_buffer[NUMBER_OF_VALUES];
        
        bool run_sensor(void);
        void frame_sync(void);
        void read_sensor(void);
        void serial_drain(void);
        void fill_frame_buffer(void);
        void add_to_ave_buf(void);
        void print_current_reading(void);
        void calculate_average_reading(void);
        void print_average_reading(void);
        void take_average(void);
        
        //Timer
        time_t start_time;
        time_t current_time;
        time_t duration;
        
        void begin_timer(void);
        bool check_begin_reading(void);
};

#endif /* MHZ19_H_ */
