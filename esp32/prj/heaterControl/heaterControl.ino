#include <Arduino.h>
#include <EEPROM.h>
#define BLINKER_WIFI //以WIFI方式接入，BLINKER_BLE以蓝牙方式接入
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>
#include <Ticker.h>                       // 使用Ticker库，需要包含头文件

#define DEBUG         0

extern "C"
{
  #include "stdio.h"
  #include "string.h"
}

#include <Adafruit_ST7735.h>//为ST7735硬件专用库

//定义SPI引脚
#define TFT_CLK       18
#define TFT_SDA       23
#define TFT_CS        5
#define TFT_RST       19
#define TFT_DC        22
#define TFT_BLK       21

//定义heater开关引脚
#define SWITCH        17

//定义继电器控制引脚
#define RELAY         16

//功能按键
//模式引脚定义
#define RED_PORT      13          //设置时间
#define YELLOW_PORT   27          //切换时间刻度
//时间控制引脚定义
#define ADD_PORT      26          //增加时间
#define SUB_PORT      25          //减少时间

//注册ST7735驱动类
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_SDA, TFT_CLK, TFT_RST);

// 创建定时器对象
Ticker timer1; 
Ticker timer2;   
Ticker timer3; 
Ticker timer4;

//新建Blinker组件-按钮
BlinkerButton Button1("btn-65b");
BlinkerButton Button2("btn-777");

//新建Blinker组件-数据
BlinkerNumber TIME1("time_remaining");
BlinkerNumber TIME2("set-time");
BlinkerNumber START("num-346");

//配置wifi联网模块
char auth[] = "0e4a8a69a7e3";
char ssid[] = "MERCURY_4604";
char pswd[] = "1994360101+-..";

//显示刷新函数
void updataLCDShow(void);

//timeSet的值更新到flash
void updata_timeSetToFlash(void);

//继电器状态更新
void updataRelayStatus(void);

//LCD背光状态更新
void updataLCDStatus(void);

//外部中断函数-开关
void IRAM_ATTR switchINT(void);

//Blinker-数据接收中断函数
void dataRead(const String & data);

//Heater定时中断函数
void timer_callback(void);

//Blinker组件状态更新中断函数
void updataBlinkerStatus(void);

//LCD背光-BlinkerSwitch回调函数
void button2_callback(const String & state);

//Heater-BlinkerSwitch回调函数
void button1_callback(const String & state);

//稳定回调函数
void stabilization_callback(void);

//防干扰回调函数
void disturb_callback(void);

//按键扫描函数
void keyScan(void);
//按键执行函数
void keyExecute(void);

//定时时间
int16_t timeSet = 0;
//剩余时间
int16_t timeREM = 0;
//运行状态
uint8_t HeaterStatus = 0;
//运行状态改变标志
uint8_t HeaterStatusChange = 0;

//LCD背光状态
uint8_t LCD_lighgtStatus = 0;
//LCD背光状态改变标志
uint8_t LCD_lighgtStatusChange = 0;

//功能按键状态
uint8_t mode_setTime = 0;
uint8_t mode_timeRate = 0;

//按键值
uint8_t key = 0;

//滤波标志
uint8_t stabilization = 0;

//稳定计数
uint8_t i = 0;
uint8_t i_old = 0;

//防干扰
uint8_t disturb = 0;

//热水器启动次数记录
uint8_t startNUM = 0;

//推出设置模式
uint8_t setOFF = 0;

//定时时间
uint16_t time_cnt1 = 0;
//uint16_t time_cnt2 = 0;

void setup() {
  // put your setup code here, to run once:

#ifdef DEBUG
  Serial.begin(115200);
#endif

  if(EEPROM.begin(4))    //申请空间，传入参数为size，为需要读写的数据字节最大地址+1，取值1~4096；
  {
    if(EEPROM.readShort(0)>100)
    {
      timeSet = 50;
      EEPROM.writeShort(0, timeSet);
      delay(1); 
      EEPROM.commit();
      delay(1);
    }
  
    timeSet=EEPROM.readShort(0);
    EEPROM.end(); //结束访问
  }

  //初始化继电器
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY,LOW);
  HeaterStatus = 0;

  //初始化开关
  pinMode(SWITCH,INPUT);
  attachInterrupt(digitalPinToInterrupt(SWITCH), switchINT, CHANGE);    // 设置外部中断

  //初始化功能按键
  //模式
  pinMode(RED_PORT,INPUT_PULLUP);
  pinMode(YELLOW_PORT,INPUT_PULLUP);
  //加减
  pinMode(ADD_PORT,INPUT_PULLUP);
  pinMode(SUB_PORT,INPUT_PULLUP);
  
  //打开背光
  pinMode(TFT_BLK,OUTPUT);
  digitalWrite(TFT_BLK,HIGH);
  LCD_lighgtStatus = 1;
  
  //初始化LCD
  tft.initR(INITR_MINI160x80_PLUGIN);

  //设置屏幕横向
  tft.setRotation(3);

  //全屏黑色
  tft.fillScreen(ST77XX_BLACK);
  
  // 初始化WIFI设置
  Blinker.begin(auth, ssid, pswd);

  //数据管理，当收到指令的时候会调用该回调函数
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);

  Blinker.attachData(dataRead);

  TIME1.print(timeREM);
  TIME2.print(timeSet);
  START.print(startNUM);

  //数据更新-定时
  timer2.attach_ms(5000,updataBlinkerStatus);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Blinker.run();
  keyScan();
  keyExecute();
  updataLCDShow();
  updataRelayStatus();
  updataLCDStatus();
  updata_timeSetToFlash();
  
}

void updata_timeSetToFlash(void)
{
  if(setOFF)
  {
    setOFF = 0;
    TIME1.print(timeREM);
    TIME2.print(timeSet); 
    if(EEPROM.begin(4))    //申请空间，传入参数为size，为需要读写的数据字节最大地址+1，取值1~4096；
    {
      EEPROM.writeShort(0, timeSet);
      delay(1); 
      EEPROM.commit();
      delay(1);
      EEPROM.end();
    }
  }
}

void updataRelayStatus(void)
{
  if(HeaterStatusChange)
  {
    HeaterStatusChange = 0;
    if(HeaterStatus)
    {
      digitalWrite(RELAY,HIGH);
      timeREM = timeSet;
      time_cnt1 = 0; 
      timer1.attach_ms(1000,timer_callback); 
      Button1.text("热水器状态:开");
      Button1.print("on");
      if(++startNUM>=100)
        startNUM = 0;
      START.print(startNUM);
    }
    else
    {
      digitalWrite(RELAY,LOW); 
      timer1.detach();
      timeREM = 0;
      Button1.text("热水器状态:关");
      Button1.print("off");
    }
  }
}

void updataLCDStatus(void)
{
  if(LCD_lighgtStatusChange)
  {
    LCD_lighgtStatusChange = 0;
    if(LCD_lighgtStatus)
    {
      digitalWrite(TFT_BLK,HIGH);
      Button2.text("LCD背光:开");
      Button2.print("on"); 
    }
    else
    {
      digitalWrite(TFT_BLK,LOW);
      Button2.text("LCD背光:关");
      Button2.print("off"); 
    }
  }
}

void dataRead(const String & data){
  BLINKER_LOG("Blinker readString: ", data);

  if(HeaterStatus==0)
  {
    if(data.toInt()<100&&data.toInt()>0)
    {
      timeSet = data.toInt();
      setOFF = 1;
    }
  }
  
}

//定时1回调函数
void timer_callback(void)
{
  time_cnt1 = time_cnt1 + 1;
  if(time_cnt1%60 == 0)
    timeREM = timeREM - 1;
  if(timeREM==0)
  {
    HeaterStatus = 1 - HeaterStatus;
    HeaterStatusChange = 1;
  }
}

//定时2回调函数
void updataBlinkerStatus(void)
{
  TIME1.print(timeREM);
  TIME2.print(timeSet);
  START.print(startNUM);

  if(HeaterStatus)
  {
    Button1.text("热水器状态:开");
    Button1.print("on");
  }
  else
  {
    Button1.text("热水器状态:关");
    Button1.print("off");
  }
  
  if(LCD_lighgtStatus)
  {
    Button2.text("LCD背光:开");
    Button2.print("on"); 
  }
  else
  {
    Button2.text("LCD背光:关");
    Button2.print("off"); 
  }
  
}

//定时3回调函数
void stabilization_callback(void)
{
  if(i==i_old&&i!=0)
  {
    timer3.detach();
    i=0;
    i_old=0;
    stabilization = 0;
    HeaterStatus = 1 - HeaterStatus;
    HeaterStatusChange = 1;
#ifdef DEBUG
    Serial.println(HeaterStatus);
#endif
  }else{
    i_old = i;
  }
}

//定时4回调函数
void disturb_callback(void)
{
  disturb = 0;
  timer4.detach();
}

//LCD背光开关回调函数
void button2_callback(const String & state){
//  BLINKER_LOG("get button state: ", state);
  
  LCD_lighgtStatus = 1 - LCD_lighgtStatus;
  LCD_lighgtStatusChange = 1;
  
//  Blinker.delay(500);//操作延时
}

// 热水器开关回调函数
void button1_callback(const String & state) {
//    BLINKER_LOG("get button state: ", state);
    HeaterStatus = 1 - HeaterStatus;
    HeaterStatusChange = 1;
    disturb = 1;
    timer4.attach_ms(2000,disturb_callback);
//    Blinker.delay(500);//操作延时
}

void IRAM_ATTR switchINT(void)
{
  if(disturb==0)
  {
    i++;
    if(stabilization==0)
    {
      stabilization = 1;
      timer3.attach_ms(50,stabilization_callback);
    }
  }
}

//按键扫描
void keyScan(void)
{
  if(digitalRead(RED_PORT)==0)
  {
    delay(10);
    if(digitalRead(RED_PORT)==0)
      key = 1;
  }
  else if(digitalRead(YELLOW_PORT)==0)
  {
    delay(10);
    if(digitalRead(YELLOW_PORT)==0)
      key = 2;
  }
  else if(digitalRead(ADD_PORT)==0)
  {
    delay(10);
    if(digitalRead(ADD_PORT)==0)
      key = 3;
  }
  else if(digitalRead(SUB_PORT)==0)
  {
    delay(10);
    if(digitalRead(SUB_PORT)==0)
      key = 4;
  }
  else
    key = 0;
}

//按键执行
void keyExecute(void)
{
  switch(key)
  {
    case 1:
      if(HeaterStatus==0)
      {
        mode_setTime = 1 - mode_setTime;
        if(mode_setTime==0)
          setOFF = 1;
      }
      break;
    case 2:
      if(mode_setTime)
        mode_timeRate = 1 - mode_timeRate;
      break;
    case 3:
      if(mode_setTime)
      {
        if(mode_timeRate==0)
          timeSet = timeSet + 10;
        else
          timeSet = timeSet + 1;
          
        if(timeSet>=100)
          timeSet = 100;
        
      }
      break;
    case 4:
      if(mode_setTime)
      {
        if(mode_timeRate==0)
          timeSet = timeSet - 10;
        else
          timeSet = timeSet - 1;
          
        if(timeSet<=0)
          timeSet = 0;
        
      }
      break;
    default:break;
  }
}

//显示刷新函数
void updataLCDShow(void)
{
    char buf[20];

    tft.setTextSize(2);
    tft.setTextWrap(true);
    
    //定时时间
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_BLUE,ST77XX_BLACK);
    memset(buf,0,20);
    sprintf(buf,"SetTIME: %3d",timeSet);
    tft.print(buf);

    if(mode_setTime==0)
    {
      //剩余时间
      tft.setCursor(0, 20);
      tft.setTextColor(ST77XX_YELLOW,ST77XX_BLACK);
      memset(buf,0,20);
      sprintf(buf,"timeREM: %3d",timeREM);
      tft.print(buf);
  
      //状态
      tft.setCursor(0, 40);
      tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
      tft.print("Status: ");
  
      tft.setCursor(96, 40);
      if(HeaterStatus)
        tft.setTextColor(ST77XX_GREEN,ST77XX_BLACK);
      else
        tft.setTextColor(ST77XX_RED,ST77XX_BLACK);
      memset(buf,0,20);
      if(HeaterStatus)
        sprintf(buf," RUN");
      else
        sprintf(buf,"STOP");
      tft.print(buf);
      
    }else{
      tft.setCursor(0, 20);
      tft.setTextColor(ST77XX_YELLOW,ST77XX_BLACK);
      memset(buf,32,13);
      tft.print(buf);

      tft.setCursor(0, 40);
      tft.setTextColor(ST77XX_YELLOW,ST77XX_BLACK);
      memset(buf,32,13);
      tft.print(buf);

      if(mode_timeRate==0)
      {
        tft.setCursor(0, 20);
        tft.setTextColor(ST77XX_YELLOW,ST77XX_BLACK);
        tft.print("+10");
      }
      else
      {
        tft.setCursor(0, 20);
        tft.setTextColor(ST77XX_YELLOW,ST77XX_BLACK);
        tft.print("+1");
      }
      
    }
    
}
