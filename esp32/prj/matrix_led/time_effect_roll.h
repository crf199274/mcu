#ifndef __time_effect_roll_H
#define __time_effect_roll_H

#include "matrix_led_init.h"
#include "Arduino.h"
#include "clock_common.h"

void matrix_led_draw_char_roll_(uint16_t x,uint16_t y,uint8_t num,CRGB fc,CRGB bc);
void matrix_led_draw_time_roll(uint16_t x,uint16_t y,CRGB fc,CRGB bc);

#endif
