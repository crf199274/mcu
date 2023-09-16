#include "FFT.h"

//float signal_data[FFT_N*2];

/*
 * @brief     错误打印
 */

static void dsps_error_print(unsigned int error_info)
{
  switch(error_info)
  {
    case ESP_ERR_DSP_INVALID_LENGTH:
      Serial.printf("ESP_ERR_DSP_INVALID_LENGTH\r\n");
      break;
    case ESP_ERR_DSP_INVALID_PARAM:
      Serial.printf("ESP_ERR_DSP_INVALID_PARAM\r\n");
      break;
    case ESP_ERR_DSP_PARAM_OUTOFRANGE:
      Serial.printf("ESP_ERR_DSP_PARAM_OUTOFRANGE\r\n");
      break;
    case ESP_ERR_DSP_UNINITIALIZED:
      Serial.printf("ESP_ERR_DSP_INVALID_LENGTH\r\n");
      break;
    case ESP_ERR_DSP_REINITIALIZED:
      Serial.printf("ESP_ERR_DSP_REINITIALIZED\r\n");
      break;
//    case ESP_ERR_DSP_ARRAY_NOT_ALIGNED:
//      Serial.printf("ESP_ERR_DSP_ARRAY_NOT_ALIGNED\r\n");
//      break;
  }
}

/*
 * @brief       计算浮点fft的幅值
 * 
 * @param[fft_result]      fft的结果
 * 
 * @param[N]               fft的长度
 */
static void dsps_fft_mag_fc32(float *fft_result,unsigned int N)
{
  int i;
  
  for(i=0;i<N;i++)
  {
    fft_result[i] = sqrt(fft_result[2*i+0]*fft_result[2*i+0]+fft_result[2*i+1]*fft_result[2*i+1])/N;
  }

  for(i=1;i<N;i++)
  {
    fft_result[i] = 2*fft_result[i];
  }
}

/*
 * @brief     fft初始化
 */
void dsps_fft_init(void)
{
  // init table
  esp_err_t ret = dsps_fft4r_init_fc32(NULL,FFT_N);

  while(ret!=ESP_OK)
  {
    Serial.printf("init table error\r\n");
    delay(1000);
  }

}

/*
 * @brief     fft测试
 */
//void dsps_fft_test(void)
//{
//  int i=0;
//  unsigned long fft_time_start;
//  unsigned long fft_time_end;
//  esp_err_t ret;
//  
//  for(i=0;i<FFT_N;i++)
//  {
//    signal_data[2*i+0] = sinf(2*FFT_PI*F1/FS*i) + 2*sinf(2*FFT_PI*F2/FS*i) + sinf(2*FFT_PI*F3/FS*i);
//    signal_data[2*i+1] = 0;
//  }
//  
//  fft_time_start = millis();
//  
//  // FFT Radix-4
//  ret = dsps_fft4r_fc32_ae32(signal_data,FFT_N);
//
//  while(ret!=ESP_OK)
//  {
//    Serial.printf("FFT error\r\n");
//    delay(1000);
//  }
//
//  // Bit reverse
//  ret = dsps_bit_rev4r_fc32_ae32(signal_data,FFT_N);
//
//  while(ret!=ESP_OK)
//  {
//    Serial.printf("Bit reverse error\r\n");
//    delay(1000);
//  }
//
//  // Convert one complex vector with length N/2 to one real spectrum vector with length N/2
//  FFT_TYPE==FFT_TYPE_FFT4R_FC32
//  ret = dsps_cplx2real_fc32_ae32(signal_data,FFT_N);
//
//  while(ret!=ESP_OK)
//  {
//    dsps_error_print(ret);
//    delay(1000);
//  }
//  
//  dsps_fft_mag_fc32(signal_data,FFT_N);
//
//  fft_time_end = millis();
//
//  Serial.printf("fft_time = %f\r\n",fft_time_end-fft_time_start);
//  
//  Serial.printf("FFT result\r\n");
//  
//  for(i=0;i<FFT_N;i++)
//  {
//    Serial.printf("%f\r\n",signal_data[i]);
////    Serial.printf("%f + %fi\r\n",signal_data[2*i+0],signal_data[2*i+1]);
//  }
//
//}

/*
 * @brief      浮点fft
 * 
 * @param[fft_data]     需要fft的数据存放地址,fft结束数据存放地址
 * 
 * @param[N]            fft的数据长度
 */
void dsps_fft(float *fft_data,unsigned int N)
{
  // FFT Radix-4
  esp_err_t ret = dsps_fft4r_fc32_ae32(fft_data,N);

  while(ret!=ESP_OK)
  {
    Serial.printf("FFT Radix-4 error\r\n");
    delay(1000);
  }

  // Bit reverse 
  ret = dsps_bit_rev4r_fc32_ae32(fft_data,N);

  while(ret!=ESP_OK)
  {
    Serial.printf("Bit reverse error\r\n");
    delay(1000);
  }

  // Convert one complex vector with length N/2 to one real spectrum vector with length N/2
  ret = dsps_cplx2real_fc32_ae32(fft_data,N);

  while(ret!=ESP_OK)
  {
    dsps_error_print(ret);
    delay(1000);
  }

  dsps_fft_mag_fc32(fft_data,N);
}
