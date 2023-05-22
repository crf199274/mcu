#define BLINKER_WIFI //以WIFI方式接入，BLINKER_BLE以蓝牙方式接入
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>
//配置wifi联网模块
char auth[] = "73b5e0dece32";
char ssid[] = "MERCURY_4604";
char pswd[] = "1994360101+-..";
int GPIO = 0;

int wsMode=0;

int switch_state = 0;

unsigned char colorR = 0;
unsigned char colorG = 0;
unsigned char colorB = 0;

BlinkerButton Button1("switch");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button2("R");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button3("G");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button4("B");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button5("Y");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button6("P");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button7("C");//新建组件和Blinker app上的数据键名一样
BlinkerButton Button8("change");//新建组件和Blinker app上的数据键名一样

void data_write(int time)
{
  digitalWrite(GPIO, HIGH);
  delayMicroseconds(time*100);
  digitalWrite(GPIO, LOW);
}

// 按下按键就会执行该函数
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    if(switch_state==0)
    {
      data_write(50);
      switch_state=1;
    }
    else
    {
      data_write(40);
      switch_state=0;
    }
    Blinker.delay(500);//操作延时
}

void button2_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    data_write(10);
    switch_state=1;
    Blinker.delay(500);//操作延时
}

void button3_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    data_write(20);
    switch_state=1;
    Blinker.delay(500);//操作延时
}

void button4_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    data_write(30);
    switch_state=1;
    Blinker.delay(500);//操作延时
}

void button5_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    data_write(60);
    switch_state=1;
    Blinker.delay(500);//操作延时
}

void button6_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    data_write(70);
    switch_state=1;
    Blinker.delay(500);//操作延时
}

void button7_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    data_write(80);
    switch_state=1;
    Blinker.delay(500);//操作延时
}

void button8_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    data_write(90);
    switch_state=1;
    Blinker.delay(500);//操作延时
}

void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        data_write(50);
        switch_state=1;
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
    }
    else if (state == BLINKER_CMD_OFF) {
        data_write(40);
        switch_state=0;
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
}

void miotColor(int32_t color)
{
    BLINKER_LOG("need set color: ", color);

    colorR = color >> 16 & 0xFF;
    colorG = color >>  8 & 0xFF;
    colorB = color       & 0xFF;

    BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);

    if(colorR!=0x00&&colorG!=0x00&&colorB!=0x00)
      data_write(50);//白色
    else if(colorR==0x00&&colorG!=0x00&&colorB!=0x00)
      data_write(80);//青色
    else if(colorR!=0x00&&colorG==0x00&&colorB!=0x00)
      data_write(70);//紫色
    else if(colorR!=0x00&&colorG!=0x00&&colorB==0x00)
      data_write(60);//黄色
    else if(colorR==0x00&&colorG==0x00&&colorB!=0x00)
      data_write(30);//蓝色
    else if(colorR!=0x00&&colorG==0x00&&colorB==0x00)
      data_write(10);//红色
    else if(colorR==0x00&&colorG!=0x00&&colorB==0x00)
      data_write(20);//绿色

    switch_state=1;
    
    BlinkerMIOT.color(color);
    BlinkerMIOT.print();
}

void miotMode(uint8_t mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_MIOT_DAY) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_NIGHT) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COLOR) {
        data_write(90);//流水灯
    }
    else if (mode == BLINKER_CMD_MIOT_WARMTH) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_TV) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_READING) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COMPUTER) {
        // Your mode function
    }

    switch_state=1;

    wsMode = mode;

    BlinkerMIOT.mode(mode);
    BlinkerMIOT.print();
}

void setup() {
    // 初始化串口
    Serial.begin(115200);
    // 初始化控制继电器的GPIO引脚
    pinMode(GPIO, OUTPUT);
    digitalWrite(GPIO, LOW);//低电平默认继电器关闭
    // 初始化WIFI设置
    Blinker.begin(auth, ssid, pswd);
   //数据管理，当收到指令的时候会调用该回调函数
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
    Button5.attach(button5_callback);
    Button6.attach(button6_callback);
    Button7.attach(button7_callback);
    Button8.attach(button8_callback);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachColor(miotColor);
    BlinkerMIOT.attachMode(miotMode);
}
//让程序一直运行
void loop() {
    Blinker.run();
}
