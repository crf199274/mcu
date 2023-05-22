#ifndef __AllTaskINIT_H
#define __AllTaskINIT_H

#include "stm32f10x.h"

#define ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"

#define Service_INFO	"AT+CIPSTART=\"TCP\",\"101.33.210.17\",1883\r\n"

#define TIME_INFO		"AT+CIPSTART=\"TCP\",\"api.k780.com\",80\r\n"

#define TASK_OK							0
#define TASK_ERROR					1

#define WIFI_LOGO_S 													"WF16x12S.bin"
#define WIFI_LOGO_D 													"WF16x12D.bin"

void peripheralDriverINIT(void);

void GUI_TASK(void);

void WEB_TASK(void);

void ConnectService_TASK(void);

void ServicePing_TASK(char oTime);

void GetTime_TASK(void);

void key_TASK(void);

void updateStatus_TASK(void);

void waitMSG_TASK(void);

uint8_t fingerprint_TASK(void);

void alarmClock_TASK(void);

void _42motor_TASK(_Bool TorF);

void checkVoiceBraodcastStatus_TASK(void);

void ServiceSubscribe_TASK(void);

void Tset_TASK(void);

void checkMoveOrReturn_TASK(void);

void LED_TASK(void);

void TimeUpdate_TASK(void);


//服药人信息(可扩充)
typedef struct {
	
	char NAME[10];//名字
	char EAT_TIME1[6];//服药时间1
	char EAT_TIME2[6];//服药时间2
	char EAT_TIME3[6];//服药时间3
	
	_Bool EAT1;
	_Bool EAT2;
	_Bool EAT3;
	
	_Bool QY1;
	_Bool QY2;
	_Bool QY3;
	
	uint8_t record;
	
	
}PATIENT_INFO;

extern PATIENT_INFO patient_Info;

typedef struct {
	
	uint8_t StatusCO;
	uint8_t StatusCF;
	
}StatusC;

typedef struct {
	
	_Bool motor;
	_Bool fingerprintf;
	_Bool voiceBroadcast;
	_Bool guiwei;
	
}Control;

typedef struct {
	
	_Bool Subscribe;
	_Bool MSG;
	_Bool Back;
	
}LED_Status;

extern LED_Status LedStatus;

#endif
