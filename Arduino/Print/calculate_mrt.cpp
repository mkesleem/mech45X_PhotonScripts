/*
 * This is the .cpp file for calculating MRT and OT
 * This code was written entirely by Team 26
 * using formulas found in Literature. 
 */
#include "CALCULATE_MRT.h"

mrt_and_ot::mrt_and_ot(void)
{
}

float mrt_and_ot::calculate_convection_coefficient(float T_g, float T_a) {
    /*
     * Calculate convection coefficient using formula in Literature
     */
    h = abs(T_g - T_a) / diameter_to_power;
    h = pow(h, 0.25);
    return(1.4 * h);
}

void mrt_and_ot::calculate_mrt_and_ot(float T_g, float T_a) {
    /*
     * Calculate MRT and OT using formulas found in Literature
     */
    T_g = T_g + kelvin_conversion;
    T_a = T_a + kelvin_conversion;
    convection_coefficient = calculate_convection_coefficient(T_g, T_a);
    T_mrt = convection_coefficient / epsilon * (T_g - T_a);
    T_mrt = T_mrt + pow(T_g,4);
    T_mrt = pow(T_mrt,0.25);
    T_ot = 0.5 * (T_a + T_mrt);
}

// Getter functions for MRT and OT
float mrt_and_ot::get_mrt(void) {return(T_mrt);}
float mrt_and_ot::get_ot(void) {return(T_ot);}

