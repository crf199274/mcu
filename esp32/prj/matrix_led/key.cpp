#include "key.h"

#define MUTE_PIN  34
#define WAKE_PIN  GPIO_NUM_35
#define SD_MODE_PIN 33

#define PIN_ADC1_CH7 39   //定义引脚宏 

uint8_t sd_mode=1;
uint16_t analogVoltsValue=0; 

void IRAM_ATTR mute_callback(void);
static key_value for_adc_get_key_value(uint16_t VoltsValue);
static key_value get_key_value(void);

void key_task(void)
{

    if(a2dp_sink.get_connection_state()==ESP_A2D_CONNECTION_STATE_CONNECTED)
    {
//      Serial.printf("analogVoltsValue=%d\r\n",analogVoltsValue);
      switch(get_key_value())
      {
        case AVRC_PLAY:
          a2dp_sink.play();
          Serial.println("play");
          break;
        case AVRC_PAUSE:
          a2dp_sink.pause();
          Serial.println("pause");
          break;
        case ARVC_NEXT:
          a2dp_sink.next();
          Serial.println("next");
          break;
        case ARVC_PREVIOUS:
          a2dp_sink.previous();
          Serial.println("previous");
          break;
        case ARVC_NONE:
          break;
      }
    }
}

void key_init(void)
{
  esp_sleep_enable_ext0_wakeup(WAKE_PIN,0); /* RTC_GPIO 唤醒 */
  pinMode(MUTE_PIN, INPUT_PULLUP); /* 设置上拉 */
  attachInterrupt(MUTE_PIN,mute_callback,FALLING); /* 外部中断 */
  pinMode(SD_MODE_PIN,OUTPUT); /* 配置输出 */
  digitalWrite(SD_MODE_PIN,HIGH);
  analogReadResolution(12);  //设置ADC的分辨率为12位(0-4096)
}

void IRAM_ATTR mute_callback(void)
{
//  delay(20); /* 软件消抖 */
  if(digitalRead(MUTE_PIN)==0)
  {
    sd_mode =1-sd_mode;
    digitalWrite(SD_MODE_PIN,sd_mode);
  }
}

static key_value for_adc_get_key_value(uint16_t VoltsValue)
{
  if(VoltsValue>=0&&VoltsValue<=500)
  {
    return AVRC_PLAY;
  }else if(VoltsValue>=825&&VoltsValue<=1325)
  {
    return AVRC_PAUSE;
  }else if(VoltsValue>=1650&&VoltsValue<=2150)
  {
    return ARVC_NEXT;
  }else if(VoltsValue>=2475&&VoltsValue<=2975)
  {
    return ARVC_PREVIOUS;
  }else{
    return ARVC_NONE;
  }
}

static key_value get_key_value(void)
{
  analogVoltsValue = analogReadMilliVolts(PIN_ADC1_CH7);

  return for_adc_get_key_value(analogVoltsValue);
}
