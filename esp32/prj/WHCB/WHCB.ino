#include "RUN_LED.h"
#include "LCD.h"
#include "rotary_knob.h"
#include "info.h"
#include "WHCB_Node.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  run_led_init(); /* 状态指示灯 */
  lcd_init();
  rk_init(); /* 旋钮初始化 */
  key_init(); /* 按键初始化 */
  info_init(); /* 信息打印 */
  heat_init(); /* 加热初始化 */
  WHCB_Node_Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  WHCB_Node_update();
}  
