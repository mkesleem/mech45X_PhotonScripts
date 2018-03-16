/*

Indoor Environmental Quality Sensor - Particulate Matter Test code

Copyright (c) 2018 Michael Sleeman

UBC MECH 457, Team 26

revision 1 (charles) - added led intication in loop and completed digitalwrite protocol
*/

byte mhz19_cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
char mhz19_response[9];
int i;
int j = 1;
int mhz19_ppm;
int choose_sensor = 1;
int sensor1 = A0;
int sensor2 = D3;
int led1 = D7;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("MH_Z19 test");
    //initialize transistor control pins
    pinMode(sensor1,OUTPUT);
    pinMode(sensor2,OUTPUT);
    //initialize D7 pin LED control
    pinMode(led1, OUTPUT);
}

int read_MHZ19_CO2()
{
    Serial1.write(mhz19_cmd, 9);
    memset(mhz19_response, 0, 9);
    Serial1.readBytes(mhz19_response, 9);
    byte crc = 0;
    for (i = 1; i < 8; i++) crc+=mhz19_response[i];
    crc = 255 - crc;
    crc++;
    if ( !(mhz19_response[0] == 0xFF && mhz19_response[1] == 0x86 && mhz19_response[8] == crc) ) {
        Serial.println("CRC error: " + String(crc) + " / "+ String(mhz19_response[8]));
        mhz19_ppm = -1;
   }
    else {
        unsigned int responseHigh = (unsigned int) mhz19_response[2];
        unsigned int responseLow = (unsigned int) mhz19_response[3];
        mhz19_ppm = ( 256 * responseHigh ) + responseLow;
    }
    return mhz19_ppm;
}

//enter loop, first initialize with warm up and then alternate reading both sensors
void loop()
{
    //turn on sensors to warm up for 3 minutes, leave led on to show warm up, flash when done
    if(j == 1){
    Serial.print("Sensor Warm Up Phase - 3min ");
    digitalWrite(sensor1, HIGH);
    digitalWrite(sensor2, HIGH);
    digitalWrite(led1, HIGH);
    delay(10000);
    Serial.print("Sensor Warm Up Phase - 2min ");
    delay(10000);
    Serial.print("Sensor Warm Up Phase - 1min ");
    delay(10000);
    digitalWrite(led1,LOW);
    delay(500);
    digitalWrite(led1, HIGH);
    delay(500);
    digitalWrite(led1,LOW);
    delay(500);
    digitalWrite(led1, HIGH);
    delay(500);
    digitalWrite(led1,LOW);
    digitalWrite(sensor1, LOW);
    digitalWrite(sensor2, LOW);
    delay(500);
    j++;
    }
    else{
    //read data from sensor 1, LED will be on while reading from sensor 1
    if( choose_sensor == 1 ) {
        //turn on sensor one using BJT and PWM output
        digitalWrite(sensor1, HIGH);
        digitalWrite(led1, HIGH);
        delay(1000);
        mhz19_ppm = read_MHZ19_CO2();
        Serial.print("Sensor 1 CO2 PPM: ");
        Serial.println(mhz19_ppm);
        choose_sensor = 2;
        delay(4300);
        digitalWrite(led1, LOW);
        digitalWrite(sensor1, LOW);
        delay(200);

    }
    //read data from sensor 2
    else {
        //turn on sensor 2 using BJT and PWM output
        digitalWrite(sensor2, HIGH);
        delay(1000);
        mhz19_ppm = read_MHZ19_CO2();
        Serial.print("Sensor 2 CO2 PPM: ");
        Serial.println(mhz19_ppm);
        choose_sensor = 1;
        delay(4300);
        digitalWrite(sensor2, LOW);
        delay(200);
    }
    }
}
