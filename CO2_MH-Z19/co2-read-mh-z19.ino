byte mhz19_cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
char mhz19_response[9];
int i;
int mhz19_ppm;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("MH_Z19 test");
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

void loop()
{
    mhz19_ppm = read_MHZ19_CO2();
    Serial.print("CO2 PPM: ");
    Serial.println(mhz19_ppm);
    delay(1000);
}
