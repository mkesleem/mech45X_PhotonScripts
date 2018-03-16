byte mhz19_cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
char mhz19_response[9];
int i;
int mhz19_ppm;
bool packetReceived;
bool DEBUG;
bool inFrame;
int detectOff;
int calcChecksum;

void setup()
{
    Serial.begin(9600);
    Serial.println("MH_Z19 test");
    DEBUG = false;
}

int MHZ19_CO2_read() {
    /*
    This function reads the PPM value from the MHZ19 sensor.

    Attributes
    ----------
    inFrame : bool
      Indicates whether the start of the frame of values from sensor has been
      found
    packetReceived : bool
      Indicates whether all of the sensor values have been received
      Controls a while loop that loops until the values have been read
    DEBUG : bool
      Inidcates whether or not to print debugging messages

    Implementation
    --------------
    Begin reading from Serial1
    Use a while loop to loop until values have been properly read

    */
    Serial.println("-- Reading MHZ19");
    Serial1.begin(9600);
    Serial1.write(mhz19_cmd, 9);
    packetReceived = false;
    inFrame = false;
    detectOff = 0;

    while (!packetReceived) {
        /*
        While loop runs until the sensor has been properly read
        Begins by draining available serial values
        */

        // Drain serial
        if (Serial1.available() > 32) {
            int drain = Serial1.available();
            if (DEBUG) {
                Serial.print("-- Draining buffer: ");
                Serial.println(Serial1.available(), DEC);
            }
            // drain the buffer
            for (int i = drain; i > 0; i--) {
                Serial1.read();
            }
        }
        // Read serial and find the start characters
        if (Serial1.available() > 0) {
            if (DEBUG) {
                Serial.print("-- Available: ");
                Serial.println(Serial1.available(), DEC);
            }
            incomingByte = Serial1.read();
            if (DEBUG) {
                Serial.print("-- READ: ");
                Serial.println(incomingByte, HEX);
            }
            if (!inFrame) {
                if (incomingByte == 0xFF && detectOff == 0) {
                  // set the first element of frame buf to be the incoming byte
                    frameBuf[detectOff] = incomingByte;
                    // set the frame header character to be this byte
                    thisFrame.frameHeader[0] = incomingByte;
                    calcChecksum = incomingByte; // Checksum init!
                    detectOff++;
                }
                // reading second start character, similar meanings to above
                else if (incomingByte == 0x86 && detectOff == 1) {
                    frameBuf[detectOff] = incomingByte;
                    thisFrame.frameHeader[1] = incomingByte;
                    calcChecksum += incomingByte;
                    // recognize that you are into the actual data now
                    inFrame = true;
                    detectOff++;
                }
                // haven't found the start charcter yet, continue looping through until you find it
                else {
                    Serial.print("-- Frame syncing... ");
                    Serial.print(incomingByte, HEX);
                    Serial.println();
                }
            }
            // code for the data-rich part of the serial message
            else {
                // set the framebuf array to equal current byte
                frameBuf[detectOff] = incomingByte;
                calcChecksum += incomingByte;
                switch (detectOff) {
                    case 2:
                        responseHigh = incomingByte;
                        break;
                    case 3:
                        responseLow = incomingByte;
                        break;
                    case 4:
                        mhz19_ppm = ( 256 * responseHigh ) + responseLow;
                        packetReceived = true;
                }
                detectOff++;
                mhz19_ppm = ( 256 * responseHigh ) + responseLow;
                }
            }
        }
    }
    Serial1.end();
    return (mhz19_ppm);
}

void loop()
{
    mhz19_ppm = MHZ19_CO2_read();
    Serial.print("CO2 PPM: ");
    Serial.println(mhz19_ppm);
    delay(1000);
}
