#include "PM.h"

/*
Indoor Environmental Quality Sensor - Particulate Matter Test code

Copyright (c) 2018 Benjamin Thorne

Copyright (c) 2016 Martin F. Falatic

UBC MECH 457, Team 26

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

int pm_output_pin = A0;
int pm_transistor_control = D0;
PM_7003 myPM;


void setup() {
    Serial.begin(9600);
    pinMode(pm_transistor_control,OUTPUT);
    Serial.println("-- Initializing...");
    delay(1000);
}

void loop() {
    digitalWrite(pm_transistor_control, HIGH);
    myPM.run_PM_sensor();
    delay(1000);
}

