#ifndef __clock_v2_H
#define __clock_v2_H

#define ENABLE_RANDOM_EFFECT  0

#include "matrix_led_init.h"
#include "Arduino.h"
#include "ds1339.h"
#include "myRandom.h"
#include "clock_common.h"
#include "time_effect_default.h"
#include "time_effect_roll.h"
#include "time_effect_nixie_tube.h"

#define TIME_EFFECT_DEFAULT     0
#define TIME_EFFECT_ROLL        1
#define TIME_EFFECT_BIXIE_TUBE  2

void matrix_led_clock_init(void);

#endif
