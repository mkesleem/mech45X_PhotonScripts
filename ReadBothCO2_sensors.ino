#include <Wire.h>
#define ADDR_6713  0x15 // default I2C slave address
byte mhz19_cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; // Tx/Rx command
int data [4];
int CO2ppmValue;
int T6713_co2Value;
char mhz19_response[9];
int i;
int mhz19_co2Value;

void setup()
{
    Wire.begin ();
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("Try to read from both CO2 sensors simultaneously");
}

int read_MHZ19_CO2()
{
    Serial1.write(mhz19_cmd, 9);
    delay(1000);
    memset(mhz19_response, 0, 9);
    Serial1.readBytes(mhz19_response, 9);
    byte crc = 0;
    for (i = 1; i < 8; i++) crc+=mhz19_response[i];
    crc = 255 - crc;
    crc++;
    if ( !(mhz19_response[0] == 0xFF && mhz19_response[1] == 0x86 && mhz19_response[8] == crc) ) {
        Serial.println("CRC error: " + String(crc) + " / "+ String(mhz19_response[8]));
        mhz19_co2Value = -1;
    }
    else {
        unsigned int responseHigh = (unsigned int) mhz19_response[2];
        unsigned int responseLow = (unsigned int) mhz19_response[3];
        mhz19_co2Value = ( 256 * responseHigh ) + responseLow;
    }
    return mhz19_co2Value;
}

int read_T6713_CO2()
{
	Wire.beginTransmission(ADDR_6713); // start I2C
	Wire.write(0x04);
	Wire.write(0x13);
	Wire.write(0x8B);
	Wire.write(0x00);
	Wire.write(0x01);
	Wire.endTransmission(); // end transmission

	// read report of current gas measurement in ppm
	delay(1000);
	Wire.requestFrom(ADDR_6713, 4); // request 4 bytes from slave device

	data[0] = Wire.read();
	data[1] = Wire.read();
	data[2] = Wire.read();
	data[3] = Wire.read();
	CO2ppmValue = data[2] * 256 + data[3];
	return CO2ppmValue;
}

void loop()
{
    T6713_co2Value = read_T6713_CO2();
    mhz19_co2Value = read_MHZ19_CO2();

	if (T6713_co2Value > 0)
	{
		Serial.print("T6713 CO2 Value: ");
		Serial.println(T6713_co2Value);
	}
	else
	{
		Serial.println("T6713 Checksum failed / Communication failure");
	}

	if (mhz19_co2Value > 0)
	{
		Serial.print("MHZ19 CO2 Value: ");
		Serial.println(mhz19_co2Value);
	}
	else
	{
		Serial.println("MHZ19 Checksum failed / Communication failure");
	}
	Serial.println("<------------ Next ------------>");
	delay(1000);
}
