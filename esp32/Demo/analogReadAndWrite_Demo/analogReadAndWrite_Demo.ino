#include "Arduino.h"

//define a variable to store the value of IO map
char ADC1_channel0 = 36;
//define a variable to store the value of IO map
char ADC2_channel2 = 2;
//define a variable to recevie analog value
int value1 = 0;
//define a variable to recevie analog value
int value2 = 0;
//define a variable to store the value of luminescence intensity
int value3 = 0;

int ADC_try(){
  //read the ananlog value of IO and return
  return analogRead(ADC1_channel0);
}

void ledPWM(int value){
  //write an analog value to an IO
  analogWrite(ADC2_channel2,value);
}

void setup() {
  //initialize the serial communication
  Serial.begin(9600);
}

void loop() {
  //Led luminescence intensity from strong to weak
  for(value3=255;value3>=0;value3-=5){
    //Execute the ledPWM function
    ledPWM(value3);
    //20 ms delay
    delay(20);
  }
  //Led luminescence intensity from weak to strong
  for(value3=0;value3<=255;value3+=5){
    //Execute the ledPWM function
    ledPWM(value3);
    //20 ms delay
    delay(20);
  }
  //execute the ADC_try function
  value1 = ADC_try();
  //Print the analog value to the serial port
  Serial.println(value1);
  //1 s delay
  delay(1000);
}
