#include "MHZ19.h"
MHZ19 myMHZ19;


void setup() {
  Serial.begin(9600);
  Serial.println("-- Initializing...");
  delay(1000);
}

void loop() {
  myMHZ19.run_MHZ19();
}

