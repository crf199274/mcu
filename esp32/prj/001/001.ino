#include "Arduino.h"
#include "Window.h"
#include "esp32-hal-cpu.h"

void setup() {
  //start window
  //Window.start();
  Serial.begin(9600);
  //setCpuFrequencyMhz(240);
  Window.begin();
  Window.LogShow();
}

void loop() {
  //
  Window.menu();
}
