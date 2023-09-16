#ifndef __time_effect_default_H
#define __time_effect_default_H

#include "matrix_led_init.h"
#include "Arduino.h"
#include "clock_common.h"

void matrix_led_draw_char_default(uint16_t x,uint16_t y,uint8_t num,CRGB fc,CRGB bc);
void matrix_led_draw_time_default(uint16_t x,uint16_t y,CRGB fc,CRGB bc);

#endif
