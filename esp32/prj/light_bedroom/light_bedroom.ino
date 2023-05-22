#define BLINKER_WIFI //以WIFI方式接入，BLINKER_BLE以蓝牙方式接入
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>
#include <Adafruit_NeoPixel.h>                  // 操作WS2812B需要使用的库

#include <Ticker.h>                       // 使用Ticker库，需要包含头文件

//配置wifi联网模块
char auth[] = "9d9ef3251d3b";
char ssid[] = "MERCURY_4604";
char pswd[] = "1994360101+-..";

Ticker ticker;                            // 创建一个定时器对象

#define PIN         0     //GPIO0接口
#define NUMPIXELS   20    //彩灯个数

Adafruit_NeoPixel led(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//开关状态
int switch_state = 0;

//改色状态记录
int C_color=0;

//滑动条组件相关变量
//亮度(数量)滑动条使能
int Slider1_En = 0;
//亮度(数量)滑动条记录值
int32_t ledNum = 0;

//模式状态记录
//int wsMode=0;

//呼吸灯使能
int COLORS_BREATHE_ENABLE = 0;

//颜色-小爱返回
uint8_t colorR = 0;
uint8_t colorG = 0;
uint8_t colorB = 0;

//颜色-app返回
uint8_t r_value_app = 0;
uint8_t g_value_app = 0;
uint8_t b_value_app = 0;

//颜色-呼吸灯临时值
float r_value_breathe = 0;
float g_value_breathe = 0;
float b_value_breathe = 0;

//颜色-呼吸灯分辨率
float r_value_PIXELS = 0;
float g_value_PIXELS = 0;
float b_value_PIXELS = 0;

//按键
BlinkerButton Button1("switch");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button2("color");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button3("night");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button4("breathe");//新建组件和Blinker app上的数据键名一样

//滑动条
BlinkerSlider Slider1("ledNumber");
BlinkerSlider Slider2("brightness");

//拾色器
BlinkerRGB RGB1("col-3iu");

//开关灯
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    if(switch_state==0)
    {
      for(int i=0; i<NUMPIXELS; ++i)
      {
        led.setPixelColor(i,led.Color(255, 255, 255)); //白色
      }
      led.show();   //刷新显示
      switch_state=1;
    }
    else
    {
      for(int i=0; i<NUMPIXELS; ++i)
      {
        led.setPixelColor(i,led.Color(0, 0, 0)); //白色
      }
      led.show();   //刷新显示
      if(COLORS_BREATHE_ENABLE)
      {
        COLORS_BREATHE_ENABLE=0;
      }
      Slider1_En = 0;
      switch_state=0;
    }
    Blinker.delay(500);//操作延时
}

//设置颜色
void setColor(unsigned char R,unsigned char G,unsigned char B)
{
  for(int i=0; i<NUMPIXELS; ++i)
  {
    led.setPixelColor(i,led.Color(R, B, G));
  }
}

//改色
void button2_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    if(switch_state)
    {
      if(COLORS_BREATHE_ENABLE)
      {
        COLORS_BREATHE_ENABLE=0;
      }
      switch(C_color)
      {
        case 0:setColor(255,0,0);break;//红
        case 1:setColor(0,255,0);break;//绿
        case 2:setColor(0,0,255);break;//蓝
        case 3:setColor(255,255,0);break;//黄
        case 4:setColor(255,0,255);break;//紫
        case 5:setColor(0,255,255);break;//青
      }
      if(++C_color>5)
        C_color=0;
      led.show();   //刷新显示
      Slider1_En = 0;
    }
    Blinker.delay(500);//操作延时
}

//设置睡眠灯
void setNightMode(void)
{
  if(switch_state)
  {
    if(COLORS_BREATHE_ENABLE)
    {
      COLORS_BREATHE_ENABLE=0;
    }
    Slider1_En = 0;
    for(int i=0; i<NUMPIXELS; ++i)
    {
      if(i==4||i==14)
        led.setPixelColor(i,led.Color(255, 255, 255));
      else
        led.setPixelColor(i,led.Color(0, 0, 0));
    }
    led.show();   //刷新显示
  }
}

//睡眠灯
void button3_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    setNightMode();
    Blinker.delay(500);//操作延时
}

//呼吸灯
void button4_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    if(switch_state)
    {
      Slider1_En = 0;
      COLORS_BREATHE_ENABLE=1;
    }
    Blinker.delay(500);//操作延时
}

//亮度(灯数目)
void slider1_callback(int32_t value)
{
  if(switch_state)
  {
    ledNum = value;

    Slider1_En = 1;

    if(COLORS_BREATHE_ENABLE)
    {
      COLORS_BREATHE_ENABLE=0;
    }
    
    for(int i=0; i<NUMPIXELS; ++i)
    {
      if(value==0)
        led.clear();      //将颜色值清除到0
      else
      {
        if(value%2!=0)
        {
          if(i>=(4-(value/2))&&i<=(4+(value/2)))
            led.setPixelColor(i,led.Color(255, 255, 255)); //白色
          else if(i>=(14-(value/2))&&i<=(14+(value/2)))
            led.setPixelColor(i,led.Color(255, 255, 255)); //白色
          else
            led.setPixelColor(i,led.Color(0, 0, 0)); //白色
        }
        else
        {
          if(i>=(4-(value/2-1))&&i<=(4+(value/2)))
            led.setPixelColor(i,led.Color(255, 255, 255)); //白色
          else if(i>=(14-(value/2-1))&&i<=(14+(value/2)))
            led.setPixelColor(i,led.Color(255, 255, 255)); //白色
          else
            led.setPixelColor(i,led.Color(0, 0, 0)); //白色
        }
      }
    }
    led.show();   //刷新显示
  }
  BLINKER_LOG("get slider value: ", value);
}

//亮度(pwm输出)
void slider2_callback(int32_t value)
{
  if(Slider1_En)
  {
    for(int i=0; i<NUMPIXELS; ++i)
    {
      if(ledNum==0)
        led.clear();      //将颜色值清除到0
      else
      {
        if(ledNum%2!=0)
        {
          if(i>=(4-(ledNum/2))&&i<=(4+(ledNum/2)))
            led.setPixelColor(i,led.Color(value, value, value));
          else if(i>=(14-(ledNum/2))&&i<=(14+(ledNum/2)))
            led.setPixelColor(i,led.Color(value, value, value));
          else
            led.setPixelColor(i,led.Color(0, 0, 0));
        }
        else
        {
          if(i>=(4-(ledNum/2-1))&&i<=(4+(ledNum/2)))
            led.setPixelColor(i,led.Color(value, value, value));
          else if(i>=(14-(ledNum/2-1))&&i<=(14+(ledNum/2)))
            led.setPixelColor(i,led.Color(value, value, value));
          else
            led.setPixelColor(i,led.Color(0, 0, 0)); //白色
        }
      }
    }
    led.show();   //刷新显示
  }
  BLINKER_LOG("get slider value: ", value);
}

//电源-小爱语音
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        for(int i=0; i<NUMPIXELS; ++i)
        {
          led.setPixelColor(i,led.Color(255, 255, 255)); //白色
        }
        led.show();   //刷新显示
        switch_state=1;
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
    }
    else if (state == BLINKER_CMD_OFF) {
        for(int i=0; i<NUMPIXELS; ++i)
        {
          led.setPixelColor(i,led.Color(0, 0, 0)); //白色
        }
        led.show();   //刷新显示
        switch_state=0;
        Slider1_En = 0;
        if(COLORS_BREATHE_ENABLE)
        {
          COLORS_BREATHE_ENABLE=0;
        }
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
}

//颜色-小爱语音
void miotColor(int32_t color)
{
    BLINKER_LOG("need set color: ", color);

    colorR = color >> 16 & 0xFF;
    colorG = color >>  8 & 0xFF;
    colorB = color       & 0xFF;

    BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);

    if(switch_state)
    {
      if(COLORS_BREATHE_ENABLE)
      {
        COLORS_BREATHE_ENABLE=0;
      }
      
      Slider1_En = 0;
      
      for(int i=0; i<NUMPIXELS; ++i)
      {
        led.setPixelColor(i,led.Color(colorR, colorB, colorG)); //白色
      }
      led.show();   //刷新显示
    }
    
    BlinkerMIOT.color(color);
    BlinkerMIOT.print();
}

//模式-小爱语音
void miotMode(uint8_t mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if(switch_state)
    {
      Slider1_En = 0;
    
      if (mode == BLINKER_CMD_MIOT_DAY) {
          // Your mode function
      }
      else if (mode == BLINKER_CMD_MIOT_NIGHT) {
          setNightMode();
      }
      else if (mode == BLINKER_CMD_MIOT_COLOR) {
          // Your mode function
      }
      else if (mode == BLINKER_CMD_MIOT_WARMTH) {
          // Your mode function
      }
      else if (mode == BLINKER_CMD_MIOT_TV) {
          if(switch_state)
          {
            Slider1_En = 0;
            COLORS_BREATHE_ENABLE=1;
          }
      }
      else if (mode == BLINKER_CMD_MIOT_READING) {
          // Your mode function
      }
      else if (mode == BLINKER_CMD_MIOT_COMPUTER) {
          // Your mode function
      }
    }

//    wsMode = mode;

    BlinkerMIOT.mode(mode);
    BlinkerMIOT.print();
}

void rgb1_callback(uint8_t r_value, uint8_t g_value, 
                    uint8_t b_value, uint8_t bright_value)
{
    if(COLORS_BREATHE_ENABLE)
    {
      r_value_app=r_value;
      g_value_app=g_value;
      b_value_app=b_value;
      r_value_PIXELS=r_value_app/51;
      g_value_PIXELS=g_value_app/51;
      b_value_PIXELS=b_value_app/51;
    }
    BLINKER_LOG("R value: ", r_value);
    BLINKER_LOG("G value: ", g_value);
    BLINKER_LOG("B value: ", b_value);
    BLINKER_LOG("Rrightness value: ", bright_value);
}


void LED_COLORS_BREATHE()
{
  if(COLORS_BREATHE_ENABLE)
  {
    r_value_breathe+=r_value_PIXELS;
    if(r_value_breathe>=(r_value_app-1))
      r_value_breathe=0;
      
    g_value_breathe+=g_value_PIXELS;
    if(g_value_breathe>=(g_value_app-1))
      g_value_breathe=0;
      
    b_value_breathe+=b_value_PIXELS;
    if(b_value_breathe>=(b_value_app-1))
      b_value_breathe=0;
      
    setColor(int(r_value_breathe),int(g_value_breathe),int(b_value_breathe));
    led.show();   //刷新显示
  }
  
}

void setup() {
    // 初始化串口
    Serial.begin(115200);
    // 初始化控制继电器的GPIO引脚
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);//低电平默认继电器关闭
    // 初始化WIFI设置
    Blinker.begin(auth, ssid, pswd);
   //数据管理，当收到指令的时候会调用该回调函数
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
    Slider1.attach(slider1_callback);
    Slider2.attach(slider2_callback);
    RGB1.attach(rgb1_callback);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachColor(miotColor);
    BlinkerMIOT.attachMode(miotMode);

    led.begin();
    led.setBrightness(255); //设置亮度 (0~255)
    led.clear();      //将颜色值清除到0
    led.show();   //刷新显示

    r_value_app=0;
    g_value_app=0;
    b_value_app=255;

    r_value_PIXELS=r_value_app/51;
    g_value_PIXELS=g_value_app/51;
    b_value_PIXELS=b_value_app/51;

    ticker.attach_ms(16, LED_COLORS_BREATHE);
    
}
//让程序一直运行
void loop() {
    Blinker.run();
}
