#ifndef __key_H
#define __key_H

#include "arduino.h"
#include "audio.h"
#include <esp_sleep.h>

typedef enum key_value_{
  AVRC_PLAY,
  AVRC_PAUSE,
  ARVC_NEXT,
  ARVC_PREVIOUS,
  ARVC_NONE,
}key_value;

void key_init(void);
void key_task(void);

#endif
