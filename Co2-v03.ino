
#define T6713_I2C               // Switch to remove CO2 sensor Support, comment out to remove

#include <stdio.h>              // Library support for sprintf print formating
#include <RTClib.h>             // Library Support for Real Time Clock on SD Card
#include <SD.h>                 // Library Support for SD card on Datashield for data storage
#include <SPI.h>                // Library Support for SPI interface for Temperature Sensor
#include <HIH61XX.h>            // Library Support Header for HoneyWell Temperature Humidity Sensor
#include <Wire.h>               // Library Support Header allows you to communicate with I2C / TWI devices.

const int chipSelect = 10;      // Chip Select Pin Number for the SD Card access

// -- Junk for now

//const int pwmpin = 9;         // the number of the sensor pin
//long duration;
//int ppm;
//ppm CO2 corrected = ppm CO2 measured * ((Tmeasured*pref) / (pmeasured*Tref))

#ifdef T6713_I2C              // Install Co2 sensor Support if enabled
#include "T6713.h"            
#endif                        // T6713_I2C

// -------- Definitions ---------------------------------

#define DISPLAY_LINE_COUNT  10       // Display line number on display screen every N lines
#define NUM_LINES           500      // Number of lines of data per file to Acquire (2880 = 24 Hrs@30000)
#define TIME_PER_SCAN       10000    // one scan every N MilliSeconds
#define FOREVER1_TIME       40000    // a Long time so not too many lines are displayed
#define FOREVER2_TIME       40000    // a Long time so not too many lines are displayed


// -------- Global Variables ----------------------------------- 

File dataFile;                // File handle for SD card 
char filename[]="THC000.csv";   // Base File name to use for this datalog run use 8.3 format maximum

RTC_DS1307 RTC;               // Real Time Clock Data Area
HIH61XX hih(0x27);            // Temperature Sensor I2C Address
DateTime time_startup;        // The time the system started
DateTime latest;              // Variable for latest time from real time clock

boolean pass_one;             // flag to indicate data file was opened
boolean file_done;            // flag to indicate file data collection completed
boolean forever1;             // loop on fail flag
boolean forever2;             // loop on data file complete
int num_done;                 // Starting Line Number in data file , used for lines Completed
int num_lines;                // scan lines remaining in run  
int disp_lines;               // display lines remaining before status is displayed

// changed so only one scan used to store to file and display user data

float meas_Humidity;          // Latest measured Humidity
float meas_Temp;              // Latest measured Temperature
float meas_CO2;               // Latest C02 raw Reading      

 // Character strings to improve formating
 
 int len;                     // length returned by sprintf
 char curr_time[22];          // used to print the current time as a string
 int status;
 
// --------- Function prototypes ---------------------------------

char * Date2String (DateTime *dtptr, char *cp);
void GetScan(int lines);      // Function to collect a line of data

// ------------ Arduino Setup function ---------------------------
//
// Code execution begins here 
// Setup Function is executed once on program reset

void setup() {
  
  meas_Humidity=100.0;         // set default obviously wrong data 
  meas_Temp=99.0;
  meas_CO2= 0.0;
  
  num_lines=NUM_LINES;         // Number of lines to collect per file
  num_done = 1;                // Set the starting counter in the data file
  pass_one = false;            // flag for file open
  file_done = false;           // flag on data file completed at numlines
  
  Serial.begin(115200);        // Initialize Serial Port to communicate with the PC
  Wire.begin();                // Initialize IIC port on Arduino as the Master Controller
  RTC.begin();                 // Initialize Realtime clock
  
  if (!RTC.isrunning())  {
    Serial.println("RTC was not running, setting default time!!");    // If first time up , Initialize to a specific time and date
    RTC.adjust(DateTime(2017, 03, 15, 11, 45, 00));
  }

// Initialize the SD card 

  pinMode(10, OUTPUT);                                      // set the SS pin as an Output to Enable SD card Access added 3/20/2017
  Serial.println("Initializing the SD card");               // Tell user were Initializing the SD card

  if(!SD.begin(chipSelect))  {                              // Open SD card,  initialization
    Serial.println("SD Begin Initialization failed!!!");
    return;                                                 // Exit setup What does this do ???? Reset???
  }
  else {
    Serial.println("SD Initialization was OK");
  }
    
  time_startup = RTC.now();                                 // Get the start up initialization Time
  
  hih.start();                                              // should turn power on to the Sensor but no hardware is there to do this 
                                                            // but turning it on prevents an error from the HIH sensor class
                                                            
  } // End of setup function


  // --------- Main File Loop function ------------
  
  // used to create multiple files on SD card
  // Arduino possibly odd fact #899: The Arduinno loop function does not automatically loop,  as the name implies ,
  // you need to have a while loop within loop to do this.
  
  void loop() {

                                                            // check and increment datafile name here 
  if (pass_one==false) { 
    
                                                            // Report the current status as text messages
      
      dataFile = SD.open(filename, FILE_WRITE);               // Open a datafile on the SD card open for File Write
      if (dataFile) {                                         // if data file opened Ok on SD card 
        Serial.println("Open Data file OK");                  // message to user
        pass_one=true;                                        // insure pass one happens only once
        status=CO2GetStatus();                                // Get Sensor Status on first pass
        CO2Sensor_Report(status);
        Serial.print("Scans to Do = ");                       // Info for user
        Serial.println(num_lines);                            // Display the line count
        Serial.println("Starting Sampling");                  // only one started sampling message
      }
      else {
      Serial.println("Open Data file Failed");
      forever1 = true;                                                                // set flag to loop forever
      while(forever1) {                                                               // insure no exit until reset occurs
        Serial.println("Looping at forever1, not logging data ; you need to Reset");  // display not logging message
        delay(FOREVER1_TIME);                                                         // slow dow the loop
      }
     }
  }
  else {                                                    // Pass_one is true , Looping now 
    
    // --------- wait for time to start logging here later perhaps
    
    GetScan(0x01);                                          // Get a new Scan of values
    num_lines--;                                            // one less line to scan
    dataFile.print(num_done,DEC);                           // Write the line number to the data file
    dataFile.print(",");                                    // Write a Delimiter character
    Serial.print(num_done++,DEC);                           // Write to screen and Increment the line number count
    Serial.print(",");                                      // Write a delimiter to the screen
    
    // ------- Write Serial data to user -------------------------------------------
      
     // first Build the current time string from its components
     
    Date2String(&latest,curr_time);                         // function to format the latest time as a string
    Serial.print(curr_time);                                // display the current time string to the user

    // -- Humidity
    
    Serial.print(meas_Humidity,3);                        // old format was Serial.print(meas_Humidity,2);
    Serial.print(",");
    Serial.print(" RH ");
    Serial.print(",");

    // -- Temperature 
   
    Serial.print(meas_Temp,2);
    Serial.print(",");

    // -- CO2 
        
    Serial.print(" C ");
    Serial.print(",");
    Serial.print(meas_CO2,2);                       // was ppm value
    

    
  // ------- Write data to the SD file -----------------------------------------------
  
 // -- Data and Time of Reading

  dataFile.print(curr_time);                           // time string already formated just print it
  
 // -- Humidity
  
  dataFile.print(meas_Humidity,3);
  dataFile.print(",");
  dataFile.print(" RH ");
  dataFile.print(",");
 
  // -- Temperature 
    
  dataFile.print(meas_Temp,2);
  dataFile.print(",");
 
  // -- CO2 
    
  dataFile.print(" C ");
  dataFile.print(",");
  dataFile.println(meas_CO2,2);                 
  dataFile.flush();                             // insure the line is written from the buffer 
  Serial.println("  Scan stored");              // display status to user after write to file
  
  
  // --------------------- print the line number every DISPLAY_LINE_COUNT lines logged
    
    if ( disp_lines < (DISPLAY_LINE_COUNT-1) )  {
      disp_lines++;                                 // increment the count
    } 
    else {
      disp_lines = 0;                               // reset the display lines count
      Serial.print(" Scans Remaining = ");          // Info for user
      Serial.println(num_lines);                    // Display the line count
   }

   delay ((int) TIME_PER_SCAN);                    // delay after every scan line

  
  // -------done writing this scan of data to the file   ------------------------------------------------------------------
  //
  // 
    
   if (num_lines==0) {                      // see if all scans completed this file is done
      file_done=true;
      dataFile.close();                     // close the Data file
      Serial.println("File Sampling Complete");  
                                            
   }


   // also were done if Serial data was received

   if (Serial.available()!=0) {             // Wait for user input
   //   while (Serial.available()!=0) {       // later add... while data read until empty.
   //     junkdata=Serial.readString();
   //   }
       file_done=true;                      // indicate were done to stop looping
      dataFile.close();                     // close the Data file
   }
  
   if(file_done) {
      forever2 = true;                                                                // set flag to loop forever
      while(forever2) {                                                               // insure no exit until reset occurs
        // delay here for time to create new file
        // increment to next file
        Serial.println("Done, Looping at forever2;  Remove Power and then SD Card");  // display not logging message
        delay(FOREVER2_TIME);                                                                   // slow dow the loop
      }
   }
   
  }   // End of if pass one is true 
 }    // end of file loop function

// ---------------------- GetScan() -------------------------------------------------------------------------
//
// Description: Function gets data from Temperature Humidity Sensor, Real Time Clock and C02 Sensor
  
void GetScan(int lines) {

    latest = RTC.now();                     // Update the current Time of the reading
    hih.update();                           // Request an update of the humidity and temperature sensor values in the class
    meas_Humidity=hih.humidity();           // read the measured float Humidity value
    meas_Temp = hih.temperature();          // read the measured float temperature value
    meas_CO2 = (float)GetCO2PPM();          // measured raw integer CO2 , might need processing ... Call the function to get a raw C02 Sensor Value
    return;                                 // done scaning return to setup

} // end of function get scan

// -------------------- Date to String ----------------------------------------------------------------------
//
// Description: Function converts a DateTime class object to a character string that can be printed.
//         
// Entry: 
// Parameter 1 : caller provides a pointer to the DateTime object to use to get the data
// Parameter 2 : Caller provides a pointer to a character string to fill minimum of 21 Characters required
// Return Value
// Function returns with null terminated string in the character array
// format changed to better work with Excel YYYY-MM-DD HH:MM:SS

char * Date2String (DateTime *dtptr, char *cp) {

int len;

    len=sprintf(cp,"%4d-", latest.year());
    len+=sprintf(cp+len,"%02d-", latest.month()); 
    len+=sprintf(cp+len,"%02d ", latest.day()); 
    len+=sprintf(cp+len,"%02d:", latest.hour());  
    len+=sprintf(cp+len,"%02d:", latest.minute());     
    len+=sprintf(cp+len,"%02d,", latest.second()); 
    return(cp);
}








  

