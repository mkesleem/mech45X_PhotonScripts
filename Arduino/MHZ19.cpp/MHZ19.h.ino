/*
 * MHZ19.h
 *
 *  Created on: Feb 7, 2018
 *      Author: michael
 */

#ifndef MHZ19_H
#define MHZ19_H
#define MHZ19_ZEROTH_BYTE 0xFF
#define MHZ19_FIRST_BYTE 0x86
#define MAX_FRAME_LEN 9
#include "WProgram.h"


class MHZ19 {
    public:
        MHZ19();
        virtual ~MHZ19();
        int getCO2(void);
        void run_MHZ19(void);
    
    private:
        char frame_buffer[MAX_FRAME_LEN];
        const uint8_t mhz19_read_command[MAX_FRAME_LEN] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};;
        bool sync_state;
        int drain;
        int frame_count;
        int byte_sum;
        int current_byte;
        int mhz19_ppm;
        
        void frame_sync(void);
        void read_data(void);
        void serial_drain(void);
        void fill_buffer(void);
};

#endif /* MHZ19_H_ */
