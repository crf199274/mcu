#include "Arduino.h"
#include "Window.h"
#include "CST816_driver.h"

void setup() {
  //start window
  //Window.start();
  Serial.begin(9600);
  Window.begin();
  Window.LCD_Fill(0,0,240,320,0xffff);
}

void loop() {
  //
  Window.CST816_Get_XY();
  Serial.println(CST816_driver.CST816_Get_ChipID());
  Serial.print(CST816_driver.X_Pos);
  Serial.print("\t");
  Serial.println(CST816_driver.Y_Pos);
  delay(1000);
}
