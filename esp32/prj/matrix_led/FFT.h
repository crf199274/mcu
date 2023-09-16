#ifndef __FFT_H
#define __FFT_H

#include <Arduino.h>
#include <dsp_err.h>
#include <dsps_fft4r.h>

#define FFT_PI        PI
#define FFT_N         1024

//#define F1            100
//#define F2            500
//#define F3            1000
//#define FS            (16*F3)
//
//extern float signal_data[FFT_N*2];

void dsps_fft_init(void);
void dsps_fft_test(void);
void dsps_fft(float *fft_data,unsigned int N);

#endif
