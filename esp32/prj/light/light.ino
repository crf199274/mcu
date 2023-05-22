#define BLINKER_WIFI //以WIFI方式接入，BLINKER_BLE以蓝牙方式接入
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>
//配置wifi联网模块
char auth[] = "8e6b0e92b0c5";
char ssid[] = "MERCURY_4604";
char pswd[] = "1994360101+-..";
int GPIO = 0;

void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(GPIO, HIGH);

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(GPIO, LOW);

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
}

BlinkerButton Button1("btn-abc");//新建组件和Blinker app上的数据键名一样
// 按下按键就会执行该函数
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(GPIO, !digitalRead(GPIO));
    Blinker.delay(500);//操作延时
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
   //
    BlinkerMIOT.attachPowerState(miotPowerState);
}
//让程序一直运行
void loop() {
    Blinker.run();
}
