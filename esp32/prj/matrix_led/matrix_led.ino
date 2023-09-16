#include "FFT.h"
#include "audio.h"
#include "matrix_led_init.h"
#include "clock_v2.h"
#include "key.h"

uint16_t lowPower_cnt=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); /* 串口初始化 */
  setCpuFrequencyMhz(160); /* 设置CPU主频为160MHz */
  matrix_led_init(); /* led点阵屏初始化 */
  matrix_led_startup_cartoon(); /* 启动动画 */
  dsps_fft_init(); /* fft初始化 */
  audio_init(); /* 音频初始化 */
  matrix_led_clock_init(); /* 时间显示初始化 */
  key_init(); /* 按键任务初始化 */
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  key_task();
  if(a2dp_sink.get_connection_state()==ESP_A2D_CONNECTION_STATE_DISCONNECTED)
  {
    lowPower_cnt++;
    if(lowPower_cnt>=3000)
    {
      lowPower_cnt=0;
      esp_deep_sleep_start();
    }
  }
//  Serial.print("APP_CPU正在运行：loop 核心：");
//  Serial.println(xPortGetCoreID());
}
