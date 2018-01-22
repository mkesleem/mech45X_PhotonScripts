bool first_run;
byte cmd[9] = {0XFF,0x01,0x88,0x07,0xD0,0x00,0x00,0x00,0xA0};

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("MH_Z19 calibrate span");
    first_run = true;
}

void loop()
{
    if (first_run == true) {
        Serial1.write(cmd, 9);
        first_run = false;
        delay(1000);
    }
    else if (first_run == false) {
        Serial.print("Sensor is being calibrated");
        delay(1000);
    }
}
