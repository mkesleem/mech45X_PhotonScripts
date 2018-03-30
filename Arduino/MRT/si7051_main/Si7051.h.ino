#ifndef _CLOSEDCUBE_SI7051_h

#define _CLOSEDCUBE_SI7051_h
#define MAX_READ_COUNT 5
#define MAX_ERROR_COUNT 10
#define ADDR_MRT 0x40
#define DEFAULT_AVERAGE 128
#include <Arduino.h>

class ClosedCube_Si7051 {
public:
    ClosedCube_Si7051();
    
    float readT(); // short-cut for readTemperature
    bool run_mrt(void);
    bool start_mrt(void);
    float get_MRT_ave(void);

private:
    uint8_t _address;
    void begin(uint8_t address);
    float readTemperature();
    float T_buf[MAX_READ_COUNT];
    float T_ave;
    int read_count;
    int error_count;
};

#endif

