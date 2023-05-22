#include "Aeduino.h"
#include "BluetoothA2DPSink.h"

BluetoothA2DPSink a2dpSink;

void setup() {
  //create a broadcast called MyMusic
  a2dp_sink.start("MyMusic");

}

void loop() {
  // put your main code here, to run repeatedly:

}
