#ifndef __time_effect_nixie_tube_H
#define __time_effect_nixie_tube_H

#include "matrix_led_init.h"
#include "Arduino.h"
#include "clock_common.h"

void matrix_led_draw_char_nixie_tube(uint16_t x,uint16_t y,uint8_t num,CRGB fc,CRGB bc);
void matrix_led_draw_time_seven_segment(uint16_t x,uint16_t y,CRGB fc,CRGB bc);

#endif
