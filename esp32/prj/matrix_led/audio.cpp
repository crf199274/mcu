#include "audio.h"

BluetoothA2DPSink a2dp_sink; /* 实例化一个a2dp接收器 */

int16_t *audio_data=NULL; /* 从接收器获取的音频数据 */
float audio_fft_data[FFT_N*2]; /* fft数据 */

audio_status audio={0,0};
bool audio_busy=false;
bool show_enable=false;

extern uint16_t lowPower_cnt;

static void timer_init(void);
static void read_data_stream(const uint8_t *data, uint32_t length);
static void connection_state_changed(esp_a2d_connection_state_t state, void *ptr);
//static void audio_state_changed(esp_a2d_audio_state_t state, void *ptr);
static void audioFFT2MatrixLed_task(void *parameter);

/*
 * @brief     音频接收初始化
 * 
 */
void audio_init(void)
{
  /* 创建频谱显示任务 */
  xTaskCreatePinnedToCore(
              audioFFT2MatrixLed_task,      /*任务函数*/
              "audioFFT2MatrixLed_task",    /*带任务名称的字符串*/
              2000,                       /*堆栈大小，单位为字节*/
              NULL,                /*作为任务输入传递的参数*/
              1,                          /*任务的优先级*/
              NULL,                       /*任务句柄*/
              1);                         /*核心*/
  
  /* 配置I2S引脚 */
  i2s_pin_config_t my_pin_config = {
        .bck_io_num = 19, /* 默认 26 */
        .ws_io_num = 18, /* 默认 25 */
        .data_out_num = 21, /* 默认 22 */
        .data_in_num = I2S_PIN_NO_CHANGE
    };
  a2dp_sink.set_pin_config(my_pin_config); /* 配置i2s */
  a2dp_sink.set_i2s_port((i2s_port_t)1); /* 设置i2s端口 */

  a2dp_sink.set_on_connection_state_changed(connection_state_changed); /* 设置连接状态回调 */
  
//  a2dp_sink.set_on_audio_state_changed(audio_state_changed); /* 设置音频流状态回调 */
  
  a2dp_sink.set_stream_reader(read_data_stream,true); /* 设置接收器紧接收数据(不把数据发送到I2S) */

  a2dp_sink.start("HP_2023_07_28"); /* 开始广播 */

  a2dp_sink.set_volume(20); /* 设置初始音量 */

  /* 启动显示 */
  show_enable = true;
}

/*
 * @brief     音频数据接收回调函数
 * 
 * @param[data]     音频数据
 * 
 * @param[length]   音频数据长度
 */
static void read_data_stream(const uint8_t *data, uint32_t length)
{
//  int16_t *samples = (int16_t*) data;
//  uint32_t sample_count = length/2;
  // Do something with the data packet
  uint16_t i,j;

//  Serial.print("APP_CPU正在运行：read_data_stream 核心：");
//  Serial.println(xPortGetCoreID());

  /* 使能显示且显示不忙碌 */
  if((audio_busy==false)&&show_enable)
  {
    audio_data = (int16_t*) data;
    audio_busy = true;
  }
  
//  if(audio_busy==0&&show_enable)
//  {
//    audio_busy = 1;
//    
//    audio_data = (int16_t*) data;
//    
//    for(i=0,j=0;i<FFT_N;i++,j+=2)
//    {
//      audio_fft_data[2*i] = audio_data[j];
//      audio_fft_data[2*i+1] = 0;
//    }
//    
//    dsps_fft(audio_fft_data,FFT_N);
//    led_fft_drawing(audio_fft_data,0,0,255);
//  }
}

//static void audio_state_changed(esp_a2d_audio_state_t state, void *ptr){
////  Serial.println(a2dp_sink.to_str(state));
//  switch(state)
//  {
//    case ESP_A2D_AUDIO_STATE_REMOTE_SUSPEND:
//      audio.stop=1;
//      Serial.printf("定时器停止1\r\n");
//      timerAlarmDisable(timer);
//      break;
//    case ESP_A2D_AUDIO_STATE_STOPPED:
//      audio.stop=1;
//      Serial.printf("定时器停止2\r\n");
//      timerAlarmDisable(timer);
//      break;
//    case ESP_A2D_AUDIO_STATE_STARTED:
//      audio.run=1;
//      Serial.printf("定时器启动\r\n");
//      timerAlarmEnable(timer);
//      break;
//    default:
//      break;
//  }
//}

static void connection_state_changed(esp_a2d_connection_state_t state, void *ptr){
//  Serial.println(a2dp_sink.to_str(state));
  switch(state)
  {
    case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
//      audio.stop=1;
      matrix_led_draw_wallpaper(CAISHEN);
      break;
    case ESP_A2D_CONNECTION_STATE_CONNECTING:
      break;
    case ESP_A2D_CONNECTION_STATE_CONNECTED:
      lowPower_cnt=0;
//      audio.run=1;
      matrix_led_clear_wallpaper();
      break;
    case ESP_A2D_CONNECTION_STATE_DISCONNECTING:
      break;
    default:
      break;
  }
}

void audioFFT2MatrixLed_task(void *parameter)
{
  uint16_t i,j;
  uint16_t fft_show_effect=getARandom(0,2);
  
  while(1)
  {
    delay(100);
    if(audio_busy)
    {
      /* FFT数据预处理 */
      for(i=0,j=0;i<FFT_N;i++,j+=2)
      {
        audio_fft_data[2*i] = audio_data[j];
        audio_fft_data[2*i+1] = 0;
      }

      dsps_fft(audio_fft_data,FFT_N); /* fft */

      if(fft_show_effect)
        led_fft_drawing_(audio_fft_data); /* 显示频谱 */
      else
        led_fft_drawing(audio_fft_data); /* 显示频谱 */

      audio_busy = false;
    }
  }
  vTaskDelete( NULL );
}
