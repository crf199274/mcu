#include "Arduino.h"
#include "string.h"

//use serial communication
/*1.Serial.begin(speed)
 *2.Serial.begin(speed, config)
 *3.Serial.print(val) ---The difference between print and prinTLn is that prinTLn's output is newlines
 *4.Serial.println(val)
 *5.Serial.read() ---serial receive
 */

//speed is Baud rate
//Baud rate to support these :300、600、1200、2400、4800、9600、14400、19200、28800、38400、57600、115200

//config
/*SERIAL_5N1/SERIAL_6N1/SERIAL_6N1/SERIAL_8N1(The default value)/SERIAL_5N2/SERIAL_6N2/SERIAL_6N2/SERIAL_8N2/SERIAL_5E1/SERIAL_5E1/SERIAL_7E1/SERIAL_8E1/SERIAL_5E2/SERIAL_6E2/
 * SERIAL_7E2/SERIAL_8E2/SERIAL_5O1/SERIAL_6O1/SERIAL_7O1/SERIAL_8O1/SERIAL_5O2/SERIAL_6O2/SERIAL_7O2/SERIAL_8O2
 * for example : The number in SERIAL_8N1 is Data bits and the letter immediately following a number is the parity bit(E for even number/N means no/O for odd number)
 * Then the number immediately before indicates the stop bit
 */

//define an array to store Datas
char buf[64];

void setup() {
  //initialize serial communication
  Serial.begin(9600);
  //The 100ms delay prevents data loss in the serial port window
  delay(100);
  //print communication
  Serial.print("starting");
  Serial.println("......");
}

void loop() {
  //receive communication
  int i = Serial.available();
  if(i != 0){
    while(i--){
      int temp = Serial.read();
      Serial.printf("%c",temp);
    }
  }
}
