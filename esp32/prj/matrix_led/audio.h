#ifndef __audio_H
#define __audio_H

#include <BluetoothA2DPSink.h>
#include "FFT.h"
#include "audioFFT2MatrixLed.h"
#include <Arduino.h>
#include "wallpaper.h"
#include "myRandom.h"

typedef struct audio_status_{
  bool stop;

  bool run;
}audio_status;

extern BluetoothA2DPSink a2dp_sink;
extern audio_status audio;

void audio_init(void);

#endif
