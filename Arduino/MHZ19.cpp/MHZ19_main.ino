#include "MHZ19.h"
MHZ19 myMHZ19;
int mhz19_transistor_control = D0;

void setup() {
  Serial.begin(9600);
  pinMode(mhz19_transistor_control,OUTPUT);
  Serial.println("-- Initializing...");
  delay(1000);
}

void loop() {
  digitalWrite(mhz19_transistor_control, HIGH);
  myMHZ19.run_MHZ19();
  digitalWrite(mhz19_transistor_control, LOW);
}

