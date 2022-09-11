#include "Arduino.h"

//define a variable to store the value of IO map
char externalPin = 19;
char outPin = 5;
//define a variable to store the value of time
char timeCnt = 0;
//define a variable to store the value of external interrupt flag
volatile char flag = 0;

void setup() {
  //initialize serial communiction
  Serial.begin(9600);
  //set pin mode
  pinMode(externalPin,INPUT);
  pinMode(outPin,OUTPUT);
  //set external interrupt
  attachInterrupt(externalPin,externalInterruptFun,CHANGE);
}

void loop() {
  //print the information at the intervals
  timeCnt++;
  Serial.println(timeCnt,DEC);
  delay(1000);
  //Preventing Data Overflow
  if(timeCnt == 5){
    timeCnt = 0;
  }
  digitalWrite(outPin,HIGH);
  delay(10);
  digitalWrite(outPin,LOW);

  //Print information if the fall edge is triggered
  if(flag){
    flag = 0;
    Serial.println("trigger fall edge");
  }
}

//external interrupt function
void externalInterruptFun(){
  flag = 1;
}
