byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
char response[9];
int i;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("MH_Z19 test");
}

void loop()
{
    Serial1.write(cmd, 9);
    memset(response, 0, 9);
    Serial1.readBytes(response, 9);
    byte crc = 0;
    for (i = 1; i < 8; i++) crc+=response[i];
    crc = 255 - crc;
    crc++;
    if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
        Serial.println("CRC error: " + String(crc) + " / "+ String(response[8]));
    } else {
        unsigned int responseHigh = (unsigned int) response[2];
        unsigned int responseLow = (unsigned int) response[3];
        unsigned int ppm = ( 256 * responseHigh ) + responseLow;
        Serial.print("CO2 PPM: ");
        Serial.println(ppm);
        delay(1000);
    }
    delay(1000);
}
