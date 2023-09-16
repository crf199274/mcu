#ifndef __clock_H
#define __clock_H

#include "matrix_led_init.h"
#include "Arduino.h"
#include "ds1339.h"

#define CLOCK_DEFAULT     0
#define CLOCK_ROLL        1
#define CLOCK_NIXIE_TUBE  2

#define CLOCK_TYPE      CLOCK_NIXIE_TUBE

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

extern uint8_t matrix_led_time[6];
extern matrix_led_clock clock_info;
extern CMO cmo;

void matrix_led_clock_init(void);

#endif
