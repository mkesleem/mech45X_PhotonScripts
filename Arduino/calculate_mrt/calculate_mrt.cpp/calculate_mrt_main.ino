// This #include statement was automatically added by the Particle IDE.
#include "CALCULATE_MRT.h"
mrt_and_ot my_MRT_OT;

float T_g;
float T_a;

void setup() {
    T_g = 25.0;
    T_a = 26.0;
}

void loop() {
    my_MRT_OT.calculate_mrt_and_ot(T_g, T_a);
    Serial.print("MRT: ");
    Serial.println(my_MRT_OT.get_mrt());
    Serial.print("OT: ");
    Serial.println(my_MRT_OT.get_ot());
}
