#include "Arduino.h"

void setup() {
  //initialize serial communication
  Serial.begin(9600);
  
}

void loop() {
  //Use the delay function to delay a period of time and print information
  delay(1000);
  Serial.println("delay");
  //Use the microseconds function to delay a period of time and print information
  delayMicroseconds(1000000);
  Serial.println("delayMicroseconds");
}
