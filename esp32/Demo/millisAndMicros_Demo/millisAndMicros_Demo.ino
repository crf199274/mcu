#include "Arduino.h"

//define a variable to store the value of time
unsigned long time1;
unsigned long time2;

void setup() {
  //initialize serial communication
  Serial.begin(9600);
}

void loop() {
  //get the system running time and print information of time
  time1 = millis();
  time2 = micros();
  Serial.print(time1,DEC);
  Serial.println(" ms");
  Serial.print(time2,DEC);
  Serial.println(" us");
  //Prevent frequent printing
  delay(1000); 
}
