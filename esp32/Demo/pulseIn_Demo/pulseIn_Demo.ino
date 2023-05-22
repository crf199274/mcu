#include "Arduino.h"

//define an variable to store the value of IO map
char TryIOPin = 0;

//define an variable to store the value of the duration
float value = 0;

//initialize an IO
void initIO(){
  //set pin mode
  pinMode(TryIOPin,INPUT);
}


void setup() {
  //executive initIO function
  initIO();
  //initialize serial communiction
  Serial.begin(9600);
}

void loop() {
  //claen value
  value = 0;
  //print tip
  Serial.println("starting");
  //executive pulseIn function
  value = pulseIn(TryIOPin,LOW,10000000)/float(1000000);
  //print out the duration
  if(value){
    Serial.println(value);
  }
}
