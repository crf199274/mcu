#include "audioFFT2MatrixLed.h"

static uint16_t g_iWRGBSeed=0; /* 该变量用于把HSV色域转换为RGB色域 */
static uint16_t columnar_part1=0,columnar_part2=0;

/*
 * @brief     HSV色域转RGB色域
 * 
 * @param[h]     h
 * @param[h]     s
 * @param[h]     v
 * 
 * @param[r]     r
 * @param[g]     g
 * @param[b]     b
 */
static void HSV2RGB(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
   float f, x, y, z;
   int i;
   v *= 255.0;
   if (s == 0.0) {
      *r = *g = *b = (int)v;
   } else {
      while (h < 0)
      h += 360;
      h = fmod(h, 360) / 60.0;
      i = (int)h;
      f = h - i;
      x = v * (1.0 - s);
      y = v * (1.0 - (s * f));
      z = v * (1.0 - (s * (1.0 - f)));
      switch (i) {
         case 0: *r = v; *g = z; *b = x; break;
         case 1: *r = y; *g = v; *b = x; break;
         case 2: *r = x; *g = v; *b = z; break;
         case 3: *r = x; *g = y; *b = v; break;
         case 4: *r = z; *g = x; *b = v; break;
         case 5: *r = v; *g = x; *b = y; break;
      }
   }
}

/*
 * @brief     频谱柱绘制
 * 
 * @param[num]     频谱柱序号(0-31)
 * 
 * @param[mag]   频谱柱大小(0-LED_FFT_MAG)
 */
static void matrix_led_draw_columnar(uint8_t num,uint8_t mag,uint8_t r,uint8_t g,uint8_t b)
{
  uint16_t i,j;
  uint8_t high = (mag>LED_FFT_MAG)?LED_FFT_MAG:mag;
  uint8_t num1 = num*2+1;
  uint8_t num2 = ((num1+1)==64)?0:(num1+1);

  for(i=0;i<high;i++)
  {
    dma_display->drawPixelRGB888(num1,i,r,g,b);
    dma_display->drawPixelRGB888(num2,i,r,g,b);
  }

  for(i=high;i<(high+5);i++)
  {
    dma_display->drawPixelRGB888(num1,i,0,0,0);
    dma_display->drawPixelRGB888(num2,i,0,0,0);
  }

  for(i=(high+5);i<(high+6);i++)
  {
    dma_display->drawPixelRGB888(num1,i,b,g,r);
    dma_display->drawPixelRGB888(num2,i,b,g,r);
  }

  for(i=(high+6);i<(LED_FFT_MAG+6);i++)
  {
    dma_display->drawPixelRGB888(num1,i,0,0,0);
    dma_display->drawPixelRGB888(num2,i,0,0,0);
  }
}

static void matrix_led_draw_columnar_(uint8_t num,uint8_t mag)
{
  uint16_t i,j;
  uint8_t high = (mag>LED_FFT_MAG)?LED_FFT_MAG:mag;
  uint8_t num1 = num*2+1;
  uint8_t num2 = ((num1+1)==64)?0:(num1+1);
  uint8_t r=0,g=0,b=0;

  for(i=0;i<high;i++)
  {
    if(high!=0)
      HSV2RGB(columnar_part1,(i/(float)high),1.0,&r,&g,&b);
    dma_display->drawPixelRGB888(num1,i,r,g,b);
    dma_display->drawPixelRGB888(num2,i,r,g,b);
  }

  for(i=high;i<(high+5);i++)
  {
    dma_display->drawPixelRGB888(num1,i,0,0,0);
    dma_display->drawPixelRGB888(num2,i,0,0,0);
  }

  for(i=(high+5);i<(high+6);i++)
  {
    HSV2RGB(columnar_part2,1.0,1.0,&r,&g,&b);
    dma_display->drawPixelRGB888(num1,i,b,g,r);
    dma_display->drawPixelRGB888(num2,i,b,g,r);
  }

  for(i=(high+6);i<(LED_FFT_MAG+6);i++)
  {
    dma_display->drawPixelRGB888(num1,i,0,0,0);
    dma_display->drawPixelRGB888(num2,i,0,0,0);
  }
}

/*
 * @brief     幅值映射-最大值
 * 
 * @param[mag]     幅值地址
 */
//static void led_fft_mag_limiting_max(float *mag)
//{
//  uint16_t i;
//  float mag_max=mag[0];
//  float weight;
//
//  for(i=0;i<LED_FFT_BAR_NUM;i++)
//  {
//    if(mag_max<mag[i*LED_FFT_BAR_FREQ_DIST])
//      mag_max = mag[i*LED_FFT_BAR_FREQ_DIST];
//  }
//
//  weight = ((mag_max/LED_FFT_MAG)<1)?1:(mag_max/LED_FFT_MAG);
//
//  for(i=0;i<LED_FFT_BAR_NUM;i++)
//  {
//      mag[i*LED_FFT_BAR_FREQ_DIST]=mag[i*LED_FFT_BAR_FREQ_DIST]/weight;
//  }
//}

/*
 * @brief     幅值映射-平均值
 * 
 * @param[mag]     幅值地址
 */
static void led_fft_mag_limiting_avr(float *mag)
{
  uint16_t i;
  float avr;
  float mag_max=mag[0];
  float weight;
  float mag_ratio = LED_FFT_MAG*2/3;
  float mag_diff = LED_FFT_MAG-mag_ratio;

  for(i=0;i<LED_FFT_BAR_NUM;i++)
  {
    if(mag_max<mag[i*LED_FFT_BAR_FREQ_DIST])
      mag_max = mag[i*LED_FFT_BAR_FREQ_DIST];
  }

  for(i=0;i<LED_FFT_BAR_NUM;i++)
  {
      avr = avr + mag[i*LED_FFT_BAR_FREQ_DIST];
  }
  avr = avr/LED_FFT_BAR_NUM;

  weight = ((avr/mag_ratio)<1)?1:(avr/mag_ratio);

  for(i=0;i<LED_FFT_BAR_NUM;i++)
  {
    if(mag[i*LED_FFT_BAR_FREQ_DIST]>avr)
      mag[i*LED_FFT_BAR_FREQ_DIST]=mag_ratio+mag_diff*avr/mag[i*LED_FFT_BAR_FREQ_DIST];
    else
      mag[i*LED_FFT_BAR_FREQ_DIST]=mag[i*LED_FFT_BAR_FREQ_DIST]/weight;
  }
}

/*
 * @brief     绘制频谱
 * 
 * @param[mag]     幅值地址
 * 
 * @param[r]     r
 * @param[g]     g
 * @param[b]     b
 */
void led_fft_drawing(float *mag)
{
  uint16_t i;
  uint8_t r,g,b;

  g_iWRGBSeed++;

  if(g_iWRGBSeed >= 360)
    g_iWRGBSeed = 0;

  HSV2RGB(g_iWRGBSeed,1.0,1.0,&r,&g,&b);

//  led_fft_mag_limiting_max(mag);
  led_fft_mag_limiting_avr(mag);
  
  for(i=0;i<LED_FFT_BAR_NUM;i++)
  {
    matrix_led_draw_columnar((LED_FFT_BAR_NUM-1-i),(uint8_t)(mag[i*LED_FFT_BAR_FREQ_DIST]),r,g,b);
  }
}

void led_fft_drawing_(float *mag)
{
  uint16_t i;
  
  columnar_part1=getARandom(0,361);
  columnar_part2=getARandom(0,361);

  led_fft_mag_limiting_avr(mag);
  
  for(i=0;i<LED_FFT_BAR_NUM;i++)
  {
    matrix_led_draw_columnar_((LED_FFT_BAR_NUM-1-i),(uint8_t)(mag[i*LED_FFT_BAR_FREQ_DIST]));
  }
}
