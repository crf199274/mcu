#include "Arduino.h"
#include "pitches.h"

//define a variable to store the value of IO map
char beebPin = 5;

//define an array to store the frequencies of some note
//define an array to store some note frequencies
int notoFrequencies[] = {NOTE_C4,NOTE_G3,NOTE_G3,NOTE_A3,NOTE_G3,0,NOTE_B3,NOTE_C4};

//define an array to store some note durations
int noteDurations[] = {4,8,8,4,4,4,4,4};

//define a variable to store the interval between two notes
int pauseBetweenNotes  = 0;

//define a variable to store the value of note duration
int noteDuration = 0;

void IOInit(){
  //initialize an IO
  pinMode(beebPin,OUTPUT);
}

void beebTry(){
  //Use a for loop to play a simple tune
  for(int i = 0;i < 8;i++){
    //to Simulate a note
    //The duration of a note is equal to 1000 / noteDurations[index]
    noteDuration = 1000 / noteDurations[i];
    tone(beebPin,notoFrequencies[i],noteDuration);
    //in order to distinguish between different notes ,you need to set a certain delay between the two tones
    //tip :increase the duration by 30 percent
    pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    //stop making noises
    noTone(beebPin);
  }
}

void setup() {
  //executive beebTry function
  beebTry();
  //executive IOInit function
  IOInit();
}

void loop() {
  //Write electrical signals to I/O
  digitalWrite(beebPin,HIGH);
}
