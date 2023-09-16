#ifndef __clock_common_H
#define __clock_common_H

#include "arduino.h"
#include "ds1339.h"

typedef struct CRGB_{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
}CRGB;

typedef struct matrix_led_clock_{
  uint16_t x;
  uint16_t y;
  CRGB fc;
  CRGB bc;
}matrix_led_clock;

typedef struct clock_message_notification{
  bool clock1;
  bool clock2;
  bool clock3;
  bool clock4;
  bool clock5;
  bool clock6;
}CMO;

extern CMO cmo;
extern T_DS1339TIME ds1339_time;

#endif
