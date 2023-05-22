#include <Arduino.h>
#include <Ticker.h>                       // 使用Ticker库，需要包含头文件
Ticker timer3; 

uint8_t cap = 0;

uint16_t i = 0;
uint16_t i_old = 0;

uint8_t heaterStatus = 0;

//定义heater开关引脚
#define SWITCH        17

void setup() {
  // put your setup code here, to run once:
  //初始化开关

  Serial.begin(115200);
  
  pinMode(SWITCH,INPUT);
  attachInterrupt(digitalPinToInterrupt(SWITCH), switchINT, CHANGE);    // 设置外部中断

  timer3.attach_ms(100,cap_callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("i:%d",i);
  delay(500);
}

void cap_callback(void)
{
  if(i==i_old&&i!=0)
  {
    i=0;
    i_old=0;
    heaterStatus = 1 - heaterStatus;
    Serial.printf("heaterStatus:%d \r\n",heaterStatus);
  }else{
    i_old = i;
  }
}

void switchINT(void)
{
  i++;
}
