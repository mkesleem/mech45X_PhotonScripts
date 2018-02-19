#include "MHZ19.h"
MHZ19 myMHZ19;
int mhz19_transistor_control = D1;
bool mhz19_average_check = false;

void setup() {
  Serial.begin(9600);
  pinMode(mhz19_transistor_control,OUTPUT);
  Serial.println("-- Initializing...");
  delay(1000);
}

void loop() {
  digitalWrite(mhz19_transistor_control, HIGH);
  while(mhz19_average_check == false) {
      myMHZ19.run_MHZ19();
      mhz19_average_check = myMHZ19.transistor_on_off();
  }
  Serial.println("Transistor off");
  mhz19_average_check = false;
  digitalWrite(mhz19_transistor_control, LOW);
  delay(1000);
}

