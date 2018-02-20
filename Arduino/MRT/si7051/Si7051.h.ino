#ifndef _CLOSEDCUBE_SI7051_h

#define _CLOSEDCUBE_SI7051_h
#include <Arduino.h>

class ClosedCube_Si7051 {
public:
  ClosedCube_Si7051();
  void begin(uint8_t address);
  void readT(); // short-cut for readTemperature
private:
  uint8_t _address;
  float readTemperature();
};
#endif

