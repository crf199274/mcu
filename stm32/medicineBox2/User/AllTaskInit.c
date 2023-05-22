#include "AllTaskInit.h"
#include "sd.h"
#include "rgbTFT.h"
#include "ff.h"
#include "stdelay.h"
#include "GUI.h"
#include "fingerprint.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "esp8266.h"
#include "service.h"
#include "MqttKit.h"
#include "42motor.h" 
#include "medicineBoxBack.h"
#include "voiceBroadcast.h"
#include "medicineBoxPosition.h"
#include "userKey.h"
#include "userLed.h"
#include "tim_interrupt.h"

const char *topics[] = {"box"};
PATIENT_INFO patient_Info={0,0,0,0,0,0,0,0,0};
Control control = {0,0};
uint8_t ping_cnt=0;
uint8_t time_cnt=0;
uint8_t date[11]={0},time[9]={0};
uint8_t Service_OK = 0;
StatusC SVC = {0,0};
uint8_t Online_OK = 0;
StatusC OLC = {0,0};
uint8_t SUBSCRIBE_OK = 0;
//StatusC guiwei = {0,0};
uint8_t guiwei_OK = 0;
StatusC QYC = {0,0};
uint8_t QY = 0;
uint8_t QYR = 0;
uint8_t QY_cnt = 0;
LED_Status LedStatus = {0};
uint8_t LedWait = 0;
uint8_t LedC = 0;
uint8_t LEDS = 0;
uint8_t LedWaitCnt = 0;

/*************************************************
函数名: peripheralDriverINIT()
功能: 初始化(所有功能的初始化都可以选择放在这个函数里面)
入口参数: 无
返回值: 无
*************************************************/
void peripheralDriverINIT()
{
	//配置滴答定时器时钟
	stdelay_init(72);
	
	//取药IO初始化
	medicineBoxBackIO_INIT();
	
	//判断药仓归位IO初始化
	medicineBoxPositionIO_INIT();
	
	//用户按键IO初始化
	userKeyINIT();
	
	//语音串口初始化
	voiceBroadcastINIT(9600);
	
	//指纹串口初始化
	fingerprintINIT(19200);
	
	//定时器4初始化
	TIM4_init(1996,35999);
	
	//定时器2初始化
	TIM2_init(999,35999);
	
	//步进电机初始化
	_42motorInit(1,1,1);
	
	//LED初始化
	LED_INIT();
	
	QY = medicineBoxBackStaus();
	
}

/*************************************************
函数名: GUI_TASK()
功能: 显示图形界面
入口参数: 无
返回值: 无
*************************************************/
void GUI_TASK(void)
{
	LCD_Init2();//配置LCDIO并初始化
	
	while(GUI_Backgroud("1.bin"));//读取TF卡的背景图并把它显示出来
	
	GUI_WIFI_LOGO((uint8_t *)WIFI_LOGO_D);//界面初始化先显示未连接的WIFI图标
	
	ONENET_LOGO("ONENETD.bin");//界面初始化先显示未连接的服务器图标
	
}

/*************************************************
函数名: WEB_TASK()
功能: 接入网络
入口参数: 无
返回值: 无
*************************************************/
void WEB_TASK(void)
{
	ESP8266_Init();//配置esp8266IO、初始化esp8266和连上路由器或者基站
	
	GUI_WIFI_LOGO((uint8_t *)WIFI_LOGO_S);//把WIFI图标更新为已连接状态
	
	Online_OK = 1;//把状态变量置1
	
}

/*************************************************
函数名: ConnectONENET_TASK()
功能: 接入onenet
入口参数: 无
返回值: 无
*************************************************/
void ConnectService_TASK(void)
{
	uint8_t COT = 4;
	
	//重新初始化esp8266
	ESP8266_Init();
	
	//建立TCP/IP网络连接
	while(ESP8266_SendCmd(Service_INFO, "CONNECT"))
		stdelay_ms(500);
	
	//建立mqtt连接
	while(Service_DevLink()&&--COT)
	{
		
		if(Service_PING()==0)
		{
			break;
		}
		
		stdelay_ms(10);
				
	}
			
	//没有超时
	if(COT!=0)
	{
		//连接成功，把图标更新为连接状态
		ONENET_LOGO("ONENET.bin");
		
		Service_OK=1;//标记为连接状态
	}
	
}

/*************************************************
函数名: ServicePing(uint8_t oTime)
功能: 检查Service连接状态
入口参数: uint8_t oTime 超时时间---10ms*oTime
返回值: 0---ping包成功 1---ping包失败 2---时间未到
*************************************************/
void ServicePing_TASK(char oTime)
{
	oTime = oTime + 1;
	
	//40秒ping包一次
	if(ping_cnt>=80)
	{
		ping_cnt = 0;//ping包标志置零
		
		if(Service_OK==1)
		{
			do{
				
				if(Service_PING()==0)
				{
					//ping包成功
					Service_OK = 1;
					break;
				}
				
				stdelay_ms(10);													//延时等待
				
			}while(--oTime);
			 
			//ping包失败
			if(oTime==0)
			{
				Service_OK = 0;
				SVC.StatusCF = 1;
			}
		}
		
	}
	
	//时间未到
	
}

/*************************************************
函数名: fingerprint_TASK()
功能: 指纹
入口参数: 无
返回值: 无
*************************************************/
uint8_t fingerprint_TASK(void)
{
//	uint8_t Eigenvalue[207]={0};
	
//	while(fingerprintDownloadEigenvalue(Eigenvalue,0x00,0x01,sizeof(Eigenvalue)));
//	
//	OneNet_SendFingerprint_data((char *)Eigenvalue);
	
	LED_B_Staus(0);
	LED_R_Staus(1);
	
	while(fingerprintCompare())
		stdelay_ms(10);	
	
	LED_B_Staus(1);
	LED_R_Staus(0);
	
	return TASK_OK;
}

/*************************************************
函数名: GetTime_TASK()
功能: 获取时间和显示时间
入口参数: 无
返回值: 无
*************************************************/
void GetTime_TASK(void)
{
	uint8_t *dataPtr = NULL;
	uint8_t i = 0;
	
	if(Service_OK==1)
	{
		//把状态变量置0
		Service_OK = 0;
		
		SVC.StatusCF = 1;
		
		ESP8266_Init();
		
	}
	
	//配置esp8266模式
	while(ESP8266_SendCmd("AT+CIPMODE=1\r\n", "OK"))
		stdelay_ms(500);
	
	//设置为单连接模式
	while(ESP8266_SendCmd("AT+CIPMUX=0\r\n", "OK"))
		stdelay_ms(500);
	
	//检查网络
	while(ESP8266_SendCmd("AT+CWJAP?\r\n","+CWJAP:"));
		stdelay_ms(500);
	
	//建立TCP/IP网络连接
	while(ESP8266_SendCmd(TIME_INFO, "CONNECT"))
		stdelay_ms(500);
	
	//开始透传
	while(ESP8266_SendCmd("AT+CIPSEND\r\n", ">"))
		stdelay_ms(500);
	
	//GET---获取网络时间
	ESP8266_SendData_N("GET http://api.k780.com/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json\r\n",105);
	
	//等待回复和解包
	while(1)
	{
		//等待回复
		dataPtr = ESP8266_GetIPD_N(50);
		if(dataPtr!=NULL)
		{
			//开启定时器
			TIM_Cmd(TIM4,ENABLE);
			
			//解包
			for(i=0;i<19;i++)
			{
				if(i<10)
				{
					date[i]=*dataPtr;
				}else if(i>10)
				{
					time[i-11]=*dataPtr;
				}
				dataPtr++;
			}
			
			//更新时间
			GUI_realtime(date,time);
			
			break;
			
		}
	}
	
}

/*************************************************
函数名: _42motor_TASK()
功能: 步进电机步进90度
入口参数: 无
返回值: 无
*************************************************/
void _42motor_TASK(_Bool TorF)
{
	//转90度
	if(TorF)
		step90T(1);
	else
		step90F(1);
	
}

/*************************************************
函数名: Test_TASK()
功能: 测试
入口参数: 无
返回值: 无
*************************************************/
void Tset_TASK(void)
{
//	if(control.motor==1)
//	{
//		step90T(1);
//	}
//	if(control.voiceBroadcast==1)
//	{
//		control.voiceBroadcast = 0;
//		if(GetpVStatus()==1)
//		{
//			voiceBroadcastEnterAndExitLowPowerMode();
//			SetpVStatus(0);
//			stdelay_ms(30);
//		}
//		voiceBroadcastPointStartPosition(0x02);
//	}
//	if(control.guiwei==1)
//	{
//		if(guiwei_OK==0)
//		{
//			if(medicineBoxPositionStaus()==1)
//			{
//				guiwei_OK = 1;
//				GUI_INFOPRINT("未归位");
//			}
//		}
//		
//		if(guiwei_OK==1)
//		{
//			if(medicineBoxPositionStaus()==0)
//			{
//				guiwei_OK = 0;
//				GUI_INFOPRINT("归位");
//			}
//		}
//	}
}

/*************************************************
函数名: getPatient_Info()
功能: 更新patient_Info
入口参数: 无
返回值: 无
*************************************************/
void getPatient_Info(void)
{
	//定义两个数据指针
	char *patient_InfoPtr1 = NULL;
	char *patient_InfoPtr2 = NULL;
	uint8_t i = 0;
	
	patient_Info.EAT1 = 0;
	patient_Info.EAT2 = 0;
	patient_Info.EAT3 = 0;
	patient_Info.QY1 = 0;
	patient_Info.QY2 = 0;
	patient_Info.QY3 = 0;
	patient_Info.record = 0;
	
	//解包
	patient_InfoPtr1 = strstr((const char *)msgBuf,"name:");
	patient_InfoPtr2 = strstr((const char *)msgBuf,",");
		
	if(patient_InfoPtr1!=NULL && patient_InfoPtr2!=NULL)
	{
		patient_InfoPtr1+=5;
		
		while(!(*patient_InfoPtr1==*patient_InfoPtr2))
		{
			patient_Info.NAME[i++] = *patient_InfoPtr1++;
		}
		
	}
	
	patient_InfoPtr1 = strstr((const char *)msgBuf,"time1:");
	
	if(patient_InfoPtr1!=NULL)
	{
		patient_InfoPtr1+=6;
		for(i=0;i<5;i++)
			patient_Info.EAT_TIME1[i] = *patient_InfoPtr1++;
		
	}
	
	patient_InfoPtr1 = strstr((const char *)msgBuf,"time2:");
	
	if(patient_InfoPtr1!=NULL)
	{
		patient_InfoPtr1+=6;
		for(i=0;i<5;i++)
			patient_Info.EAT_TIME2[i] = *patient_InfoPtr1++;
		
	}
	
	patient_InfoPtr1 = strstr((const char *)msgBuf,"time3:");
	
	if(patient_InfoPtr1!=NULL)
	{
		patient_InfoPtr1+=6;
		for(i=0;i<5;i++)
			patient_Info.EAT_TIME3[i] = *patient_InfoPtr1++;
		
	}
	
	//删包
	cleanMsgBuf();
	
}

/*************************************************
函数名: getcontrolMSG()
功能: 等待控制信息
入口参数: 无
返回值: 无
*************************************************/
void getcontrolMSG()
{
	//定义两个数据指针
	char *patient_InfoPtr1 = NULL;
	char *patient_InfoPtr2 = NULL;
	
	//解包
	patient_InfoPtr1 = strstr((const char *)msgBuf,"control:");
	patient_InfoPtr2 = strstr((const char *)msgBuf,"/");
	
	if(patient_InfoPtr1!=NULL && patient_InfoPtr2!=NULL)
	{
		patient_InfoPtr1+=8;
		
		while(!(*patient_InfoPtr1==*patient_InfoPtr2))
		{
			control.guiwei = (*patient_InfoPtr1++)-0x30;
		}
		
	}
	
}

/*************************************************
函数名: waitMSG_TASK()
功能: 等待信息
入口参数: 无
返回值: 无
*************************************************/
void msgUnPacket(uint8_t * msg)
{
	uint8_t i=0,MSG[4]={0};
	
	for(i=0;i<4;i++)
	{
		MSG[i]=*msg++;
	}
	
	msg++;
	
	switch(MSG[3])
	{
		case '1':getPatient_Info();break;
		
		case '2':getcontrolMSG();break;
		
		default :break;
	}
	
}

/*************************************************
函数名: waitMSG_TASK()
功能: 等待信息
入口参数: 无
返回值: 无
*************************************************/
void waitMSG_TASK(void)
{
	//定义一个指针准备接收数据的地址
	uint8_t *dataPtr = NULL;
	
	//检查设备与服务器是否处于连接状态
	if(Service_OK==1)
	{
		//等待消息
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
		{
			//mqtt解包
			Service_RevPro(dataPtr);
			
			//消息解包
			msgUnPacket(msgBuf);
			
		}
	}
}

/*************************************************
函数名: checkVoiceBraodcastStatus()
功能: 检查语音播报是否需要进入待机状态
入口参数: 无
返回值: 无
*************************************************/
void checkVoiceBraodcastStatus_TASK(void)
{
	//判断是否处于低功耗状态，如果处于正常状态就5秒检查一次语音模块的播放状态
	if(GetpVStatus()==0)
	{
		if(time_cnt>=5)
		{
			time_cnt=0;//语音模块状态检测变量置零
			//判断播报状态，如果处于空闲状态就使语音模块进入低功耗状态
			if(voiceBroadcastStatus()==0)
			{
				voiceBroadcastEnterAndExitLowPowerMode();
				SetpVStatus(1);
			}
		}
	}
}

/*************************************************
函数名: alarmClock_TASK()
功能: 闹钟
入口参数: 无
返回值: 无
*************************************************/
void alarmClock_TASK(void)
{
	uint8_t H0=0,M0=0;
	uint8_t H1=0,M1=0,H2=0,M2=0,H3=0,M3=0;
	uint8_t i = 0;
	
	//时间格式转换(字符转数字)
	H0=(time[0]-0x30)*10+(time[1]-0x30);
	M0=(time[3]-0x30)*10+(time[4]-0x30);
	
	H1=(patient_Info.EAT_TIME1[0]-0x30)*10+(patient_Info.EAT_TIME1[1]-0x30);
	M1=(patient_Info.EAT_TIME1[3]-0x30)*10+(patient_Info.EAT_TIME1[4]-0x30);
	
	H2=(patient_Info.EAT_TIME2[0]-0x30)*10+(patient_Info.EAT_TIME2[1]-0x30);
	M2=(patient_Info.EAT_TIME2[3]-0x30)*10+(patient_Info.EAT_TIME2[4]-0x30);
	
	H3=(patient_Info.EAT_TIME3[0]-0x30)*10+(patient_Info.EAT_TIME3[1]-0x30);
	M3=(patient_Info.EAT_TIME3[3]-0x30)*10+(patient_Info.EAT_TIME3[4]-0x30);
	
	//时间判断
	if((H0==H1&&M0==M1)&&patient_Info.EAT1==0)
	{
		patient_Info.EAT1=1;
//		if(GetpVStatus()==1)
//		{
//			voiceBroadcastEnterAndExitLowPowerMode();
//			SetpVStatus(0);
//			stdelay_ms(30);
//		}
		
		voiceBroadcastPointStartPosition(0x08);
		
		while(fingerprint_TASK());
		
		if(patient_Info.record==0)
		{
			i = 1;
			patient_Info.record = 1;
			
			while(i--)
			{
				_42motor_TASK(1);
			}
		}
		
	}
	
	if((H0==H2&&M0==M2)&&patient_Info.EAT2==0)
	{
		patient_Info.EAT2=1;
//		if(GetpVStatus()==1)
//		{
//			voiceBroadcastEnterAndExitLowPowerMode();
//			SetpVStatus(0);
//			stdelay_ms(30);
//		}
		voiceBroadcastPointStartPosition(0x0B);
		
		while(fingerprint_TASK());
		
		if(patient_Info.record==1)
		{
			i = 2;
			patient_Info.record = 2;
			
			while(i--)
			{
				_42motor_TASK(1);
			}
		}
		
	}
	
	if((H0==H3&&M0==M3)&&patient_Info.EAT3==0)
	{
		patient_Info.EAT3=1;
//		if(GetpVStatus()==1)
//		{
//			voiceBroadcastEnterAndExitLowPowerMode();
//			SetpVStatus(0);
//			stdelay_ms(30);
//		}
		voiceBroadcastPointStartPosition(0x0B);
		
		while(fingerprint_TASK());
		
		if(patient_Info.record==2)
		{
			i = 3;
			patient_Info.record = 3;
			
			while(i--)
			{
				_42motor_TASK(1);
			}
		}
		
	}
	
}

/*************************************************
函数名: key_TASK()
功能: 复位(可修改)
入口参数: 无
返回值: 无
*************************************************/
void key_TASK(void)
{
	//软件复位
	if(userKeyStaus()==0)
	{
		stdelay_ms(10);
		if(userKeyStaus()==0)
		{
			__set_FAULTMASK(1);
			NVIC_SystemReset();
		}
	}
}

/*************************************************
函数名: checkMoveOrReturn_TASK()
功能: 取药判断
入口参数: 无
返回值: 无
*************************************************/
void checkMoveOrReturn_TASK(void)
{
	uint8_t i = 0;
	if(QY == 0)
	{
		if(medicineBoxBackStaus()==1)
		{
			stdelay_ms(10);
			if(medicineBoxBackStaus()==1)
			{
				QY = 1;
				QYR = 1;
				if(patient_Info.QY1==0)
				{
					Service_Publish("hwf","1");
					patient_Info.QY1 = 1;
				}else if(patient_Info.QY1==1&&patient_Info.QY2==0)
				{
					Service_Publish("hwf","2");
					patient_Info.QY2 = 1;
				}else if(patient_Info.QY1==1&&patient_Info.QY2==1&&patient_Info.QY3==0)
				{
					Service_Publish("hwf","3");
					patient_Info.QY3 = 1;
				}
			}
		}
	}else{
		if(medicineBoxBackStaus()==0)
		{
			stdelay_ms(10);
			if(medicineBoxBackStaus()==0)
			{
				if(QYR==1)
				{
					QY_cnt=0;
					QYR =0;
				}
				
				if(QY_cnt>=3)
				{
					QY = 0;
					i = patient_Info.record;
					while(i--)
					{
						_42motor_TASK(0);
					}
					LedStatus.Back = 1;
				}
			}
		}
	}
}

/*************************************************
函数名: LED_TASK()
功能: 状态指示
入口参数: 无
返回值: 无
*************************************************/
void TimeUpdate_TASK(void)
{
	GUI_realtime(date,time);//更新时间
}

/*************************************************
函数名: LED_TASK()
功能: 状态指示
入口参数: 无
返回值: 无
*************************************************/
void LED_TASK(void)
{
	if(LedStatus.Subscribe == 1)
	{
		LED_B_Staus(1);
		LED_R_Staus(0);
	}
	if(LedStatus.MSG == 1)
	{
		LedStatus.Subscribe = 0;
		LedStatus.MSG = 0;
		LedWait = 6;
		LedWaitCnt = 0;
		LEDS = 1;
	}
	if(LedStatus.Back == 1)
	{
		LedWait = 6;
		LedWaitCnt = 0;
		LedStatus.Subscribe = 0;
		LedStatus.Back = 0;
		LED_B_Staus(1);
		LED_R_Staus(1);
	}
	if(LedWait!=0&&LedWaitCnt >= LedWait)
	{
		LedStatus.Subscribe = 1;
		LedWait = 0;
		LEDS = 0;
	}
	if(LEDS)
	{
		if(LedC)
		{
			LED_B_Staus(0);
			LED_R_Staus(1);
		}else{
			LED_B_Staus(1);
			LED_R_Staus(0);
		}
	}
	
}

/*************************************************
函数名: CK_Ping()
功能: 利用ping包检测连接状态
入口参数: 无
返回值: 无
*************************************************/
_Bool CK_Ping(uint8_t cnt)
{
	do{
		
		if(Service_PING()==0)
		{
			
			return 0;
			
		}
		
		stdelay_ms(10);
		
	}while(cnt--);
	
	return 1;
}

/*************************************************
函数名: LOGO_Changed()
功能: 更新图标
入口参数: 无
返回值: 无
*************************************************/
void LOGO_Changed(void)
{
	//状态改变
	if(SVC.StatusCO)
	{
		
		SVC.StatusCO = 0;
		
		//把图标更新为连接状态
		ONENET_LOGO("ONENET.bin");
		
	}else if(SVC.StatusCF){
		
		SVC.StatusCF = 0;
		
		//更新服务器连接图标为未连接状态
		ONENET_LOGO("ONENETD.bin");
		
	}
	
	//状态改变
	if(OLC.StatusCO)
	{
		
		OLC.StatusCO = 0;
		
		//更新WIFI图标为连接状态
		GUI_WIFI_LOGO((uint8_t *)WIFI_LOGO_S);
		
	}else if(OLC.StatusCF){
		
		OLC.StatusCF = 0;
		
		//更新WIFI图标为未连接状态
		GUI_WIFI_LOGO((uint8_t *)WIFI_LOGO_D);
		
	}
	
}

/*************************************************
函数名: updateService()
功能: 监控服务器连接状态，如果断连则重连
入口参数: 无
返回值: 无
*************************************************/
void updateService(void)
{
	uint8_t COT = 4;
	
	//判断服务器是否断开连接
	if(Service_OK==0&&Online_OK==1)
	{
		
		ESP8266_Init();
		
		//建立TCP/IP网络连接
		while(ESP8266_SendCmd(Service_INFO, "CONNECT"))
			stdelay_ms(500);
		
		//建立mqtt连接
		while(Service_DevLink()&&--COT)
		{
			
			if(Service_PING()==0)
			{
				break;
			}
			
			stdelay_ms(10);
					
		}
				
		//没有超时
		if(COT!=0)
		{
			//连接成功，把图标更新为连接状态
			ONENET_LOGO("ONENET.bin");
			
			Service_OK=1;//标记为连接状态
		}
		
	}
	
}

/*************************************************
函数名: CK_Internet()
功能: 检查网络连接状态
入口参数: 无
返回值: 无
*************************************************/
void CK_Internet(void)
{
	
	//检查网络连接状态
	if(Service_OK==0&&Online_OK==1)
	{
		//恢复AT指令
		if(ESP8266_SendCmd_N("AT\r\n", "OK")==1)
		{
			while(1)
			{
				if(ESP8266_SendCmd_N("+++","+++")==0)
				{
					while(ESP8266_SendCmd_N("AT+CIPCLOSE","CLOSED"));
					break;
				}
			}
		}
		
		//判断是否有网
		if(ESP8266_SendCmd("AT+CWJAP?\r\n","+CWJAP:")==0)
		{
			
			Online_OK = 1;
			
			
		}else{
			
			Online_OK = 0;
			
			OLC.StatusCF = 1;
			
		}
		
	}
	
	if(Service_OK==0&&Online_OK==0)
	{
		//恢复AT指令
		if(ESP8266_SendCmd_N("AT\r\n", "OK")==1)
		{
			while(1)
			{
				if(ESP8266_SendCmd_N("+++","+++")==0)
				{
					while(ESP8266_SendCmd_N("AT+CIPCLOSE","CLOSED"));
					break;
				}
			}
		}
		
		//判断是否有网
		if(ESP8266_SendCmd("AT+CWJAP?\r\n","+CWJAP:")==0)
		{
			
			OLC.StatusCO = 1;
			
			Online_OK = 1;
			
		}else{
			
			Online_OK = 0;
			
		}
		
	}
	
}

/*************************************************
函数名: updateWebStatus()
功能: 更新网络状态
入口参数: 无
返回值: 无
*************************************************/
void updateStatus_TASK(void)
{
	
	LOGO_Changed();
	
	CK_Internet();
	
	updateService();
	
	ServiceSubscribe_TASK();
	
}

/*************************************************
函数名: ServiceSubscribe_TASK()
功能: 订阅任务
入口参数: 无
返回值: 无
*************************************************/
void ServiceSubscribe_TASK(void)
{
	uint8_t *dataPtr = NULL;
	
	if(Service_OK==1&&SUBSCRIBE_OK==0)
	{
		//订阅一个主题
		Service_Subscribe(topics,1);
		
		dataPtr = ESP8266_GetIPD(100);
		
		if(MQTT_UnPacketRecv(dataPtr)==MQTT_PKT_SUBACK)
		{
			SUBSCRIBE_OK = 1;
//			GUI_INFOPRINT("订阅成功");
//			LED_B_Staus(1);
//			LED_R_Staus(0);
			LedStatus.Subscribe = 1;
			
		}
	}
	
}

/*************************************************
函数名: TIM4_IRQHandler()
功能: 时间计时、语音播报状态定时查询
入口参数: 无
返回值: 无
*************************************************/
void TIM4_IRQHandler()
{
	uint8_t a=0,b=0,c=0;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==1)
	{
		a=(time[0]-0x30)*10+(time[1]-0x30);
		b=(time[3]-0x30)*10+(time[4]-0x30);
		c=(time[6]-0x30)*10+(time[7]-0x30);
		c++;
		if(c>=60&&a==23&&b==59)
		{
			a=0;
			b=0;
			c=0;
		}else if(c>=60&&b==59)
		{
			b=0;
			c=0;
			a++;
		}else if(c>=60)
		{
			b++;
			c=0;
		}
		
		time[0]=a/10+0x30;
		time[1]=a%10+0x30;
		time[3]=b/10+0x30;
		time[4]=b%10+0x30;
		time[6]=c/10+0x30;
		time[7]=c%10+0x30;
		
		GUI_realtime(date,time);//更新时间
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		
	}
}

/*************************************************
函数名: TIM2_IRQHandler()
功能: ping包计时(这个可以把它合到一个定时器)
入口参数: 无
返回值: 无
*************************************************/
void TIM2_IRQHandler()
{
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==1)
	{
		time_cnt++;//语音播报状态定时变量
		ping_cnt++;
		LedWaitCnt++;
		LedC = 1 - LedC;
		QY_cnt++;//转盘回正
		if(QY_cnt>200)
			QY_cnt=0;
		if(ping_cnt>=300)
		{
			ping_cnt = 0;
			LedWaitCnt = 0;
		}
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		
	}
}
