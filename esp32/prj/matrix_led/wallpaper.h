#ifndef __wallpaper_H
#define __wallpaper_H

#include "matrix_led_init.h"
#include "Arduino.h"
#include "audio.h"

typedef enum wallpaper_{

  ZHONGLI=0,
  TV,
  CAISHEN,
  
}wallpaper;

void matrix_led_startup_cartoon(void);
void matrix_led_draw_wallpaper(uint8_t wp);
void matrix_led_clear_wallpaper(void);

#endif
