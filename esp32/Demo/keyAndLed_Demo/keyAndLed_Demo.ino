#include "Arduino.h"

char ledPin = 2;
char keyPin = 0;

void ledInit(){
  //initialize the IO of led
  pinMode(ledPin,OUTPUT);
}

void keyInit(){
  //initialize the IO of led
  pinMode(keyPin,INPUT);
}

void setup() {
  //initialize the led
  ledInit();
  //initialize the key
  keyInit();
}

void loop() {
  //if key value is HIGH
  if(digitalRead(keyPin)){
      //turn on the led
      digitalWrite(ledPin,HIGH);
  }else{
      //turn off the led
      digitalWrite(ledPin,LOW);
  }
  //10 ms delay 
  delay(10);
}
