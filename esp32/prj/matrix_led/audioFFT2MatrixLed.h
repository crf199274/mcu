#ifndef __audioFFT2MatrixLed_H
#define __audioFFT2MatrixLed_H

#include "matrix_led_init.h"
#include "myRandom.h"

#define LED_FFT_MAG 30
#define LED_FFT_BAR_NUM 32
#define LED_FFT_BAR_FREQ_DIST 4

void led_fft_drawing(float *mag);
void led_fft_drawing_(float *mag);

#endif
