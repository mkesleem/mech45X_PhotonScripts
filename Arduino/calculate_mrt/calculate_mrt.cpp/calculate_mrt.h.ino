/*
 * This is the .h file for calculating MRT and OT
 * This code was written entirely by Team 26
 * using formulas found in Literature.
 */
#ifndef CALCULATE_MRT_H
#define CALCULATE_MRT_H

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class mrt_and_ot {
    public:
        mrt_and_ot(void);
        
        void calculate_mrt_and_ot(float T_g, float T_a);
        float get_mrt(void);
        float get_ot(void);
        
    private:
        float calculate_convection_coefficient(float T_g, float T_a);
        float h;
        float T_mrt;
        float T_ot;
        float T_a;
        float T_g;
        float convection_coefficient;
        
        const float epsilon = 0.94;
        const float diameter = 0.04;
        const float diameter_to_power = pow(diameter, 0.4);
        const float kelvin_conversion = 273.15;
};
#endif
