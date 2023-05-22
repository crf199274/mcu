#include "AllTaskInit.h"
#include "GUI.h"
#include "main.h"
#include "service.h"
#include "string.h"
#include "stdio.h"
#include "voiceBroadcast.h"
#include "esp8266.h"
#include "stdelay.h"
#include "medicineBoxPosition.h"
#include "userKey.h"

int main()
{
	
	//初始化
	peripheralDriverINIT();
	
	//开始LCD显示
	GUI_TASK();
	
	//接入网络
	WEB_TASK();
	
	//得到网络时间
	GetTime_TASK();
	
	//接入服务器
	ConnectService_TASK();
	
	//订阅
	ServiceSubscribe_TASK();
	
	while(1)
	{
		
		//等待信息
		waitMSG_TASK();
		
		//测试任务
//		Tset_TASK();
		
		//复位(重启)判断
		key_TASK();
		
		//LED状态指示
		LED_TASK();
		
		//更新网络状态
		updateStatus_TASK();
		
		//药仓归位
		checkMoveOrReturn_TASK();
		
		//闹钟
		alarmClock_TASK();
		
		//检查语音播报是否需要进入待机状态
//		checkVoiceBraodcastStatus_TASK();
		
		//ping包任务
		ServicePing_TASK(3);

		//延时,不用频繁去查询信息
		stdelay_ms(10);
		
	}
}
