#include "voiceBroadcast.h"
#include "stdelay.h"
#include "GUI.h"

//接收指纹返回值
uint8_t ACK_V[6]={0};
//指定返回值的字节数
uint8_t ACKS_V=0;
//接收完成标志
uint8_t ACKSign_V=0;
//中断统计接收次数
uint8_t n_V=0;
//低功耗状态
uint8_t pV=0;

/*************************************************
函数名: voiceBroadcastUasrtINIT
功能: 语音播报串口初始化
入口参数: uint32_t BaudRate 波特率
返回值: 无
*************************************************/
void voiceBroadcastUasrtINIT(uint32_t BaudRate)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_my);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_11;
//	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_my);
	
	USART_InitTypeDef  USART_Init_my;
	USART_Init_my.USART_BaudRate=BaudRate;
	USART_Init_my.USART_WordLength=USART_WordLength_8b;
	USART_Init_my.USART_StopBits=USART_StopBits_1;
	USART_Init_my.USART_Parity=USART_Parity_No;
	USART_Init_my.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_Init_my.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART3,&USART_Init_my);
	
	USART_Cmd(USART3,ENABLE);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
//	USART_ITConfig(USART1,USART_IT_TC,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef  NVIC_Init_my;
	NVIC_Init_my.NVIC_IRQChannel=USART3_IRQn;
	NVIC_Init_my.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_Init_my.NVIC_IRQChannelSubPriority=3;
	NVIC_Init_my.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Init_my);
}

/*************************************************
函数名: equipment
功能: equipment
入口参数: 无
返回值: 无
*************************************************/
void equipment(void)
{
	uint8_t CHK = 0x04^0x35^0x01;
	
	//1bit
	USART_SendData(USART3,0x7E);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//2bit
	USART_SendData(USART3,0x04);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//3bit
	USART_SendData(USART3,0x35);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//4bit
	USART_SendData(USART3,0x01);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//5bit
	USART_SendData(USART3,CHK);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//6bit
	USART_SendData(USART3,0xEF);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
}

/*************************************************
函数名: voiceBroadcastINIT
功能: 语音播报初始化
入口参数: uint32_t BaudRate 波特率
返回值: 无
*************************************************/
void voiceBroadcastINIT(uint32_t BaudRate)
{
	voiceBroadcastUasrtINIT(BaudRate);
	stdelay_ms(100);
	equipment();
	stdelay_ms(30);
	voiceBroadcastSetVolume(0x0f);
//	stdelay_ms(30);
//	voiceBroadcastEnterAndExitLowPowerMode();
//	pV=1;
}

/*************************************************
函数名: voiceBroadcastEnterAndExitLowPowerMode
功能: 进入和退出低功耗模式
入口参数: 无
返回值: 无
*************************************************/
void voiceBroadcastEnterAndExitLowPowerMode(void)
{
	//1bit
	USART_SendData(USART3,0x7E);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//2bit
	USART_SendData(USART3,0x03);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//3bit
	USART_SendData(USART3,0x07);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//4bit
	USART_SendData(USART3,0x04);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//5bit
	USART_SendData(USART3,0xEF);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	
}

/*************************************************
函数名: voiceBroadcastSetVolume
功能: 设置音量大小
入口参数: uint8_t Volume 音量等级(音量为 0-30 级可调，该指令可以实时修改调节音量，音量可以掉电记忆)
返回值: 无
*************************************************/
void voiceBroadcastSetVolume(uint8_t Volume)
{
	uint8_t CHK = 0x04^0x31^Volume;
	
	//1bit
	USART_SendData(USART3,0x7E);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//2bit
	USART_SendData(USART3,0x04);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//3bit
	USART_SendData(USART3,0x31);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//4bit
	USART_SendData(USART3,Volume);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//5bit
	USART_SendData(USART3,CHK);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//6bit
	USART_SendData(USART3,0xEF);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	
}

/*************************************************
函数名: Get_pVStatus()
功能: 得到低功耗状态
入口参数: 无
返回值: 状态 1-低功耗 0-正常
*************************************************/
uint8_t GetpVStatus(void)
{
	return pV;
}

/*************************************************
函数名: SetpVStatus()
功能: 标记低功耗状态
入口参数: 无
返回值: 无
*************************************************/
void SetpVStatus(_Bool status)
{
	pV=status;
}

/*************************************************
函数名: voiceBroadcastStatus
功能: 查询播放状态
入口参数: 无
返回值: uint8_t 1-正在播放 0-空闲
*************************************************/
uint8_t voiceBroadcastStatus(void)
{
	//延时变量
	uint32_t i=0;
	//字节计数变量清零
	n_V=0;
	//指定返回的字节数
	ACKS_V = 6;
	
	//1bit
	USART_SendData(USART3,0x7E);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//2bit
	USART_SendData(USART3,0x03);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//3bit
	USART_SendData(USART3,0x10);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//4bit
	USART_SendData(USART3,0x13);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//5bit
	USART_SendData(USART3,0xEF);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	
	i=0xFFFFFF;
	while(!(ACKSign_V==1)&&--i);
	ACKSign_V=0;
	
	if(i==0)
	{
		#if DEBUG2 == 1
			GUI_INFOPRINT("串口接收失败");
		#endif
		return 2;
	}
	
	if(ACK_V[1]==0x4B&&ACK_V[5]==0x00)
	{
		#if DEBUG2 == 1
			GUI_INFOPRINT("空闲");
		#endif
		return 0;
	}else{
		#if DEBUG2 == 1
			GUI_INFOPRINT("失败");
		#endif
		return 1;
	}
	
}

/*************************************************
函数名: voiceBroadcastSetEQMode
功能: 设置EQ
入口参数: uint8_t EQ 0-5(NO\POP\ROCK\JAZZ\CLASSIC\BASS(掉电记忆)
返回值: 无
*************************************************/
void voiceBroadcastSetEQMode(uint8_t EQ)
{
	uint8_t CHK = 0x04^0x32^EQ;
	
	//1bit
	USART_SendData(USART3,0x7E);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//2bit
	USART_SendData(USART3,0x04);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//3bit
	USART_SendData(USART3,0x32);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//4bit
	USART_SendData(USART3,EQ);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//5bit
	USART_SendData(USART3,CHK);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//6bit
	USART_SendData(USART3,0xEF);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	
}

/*************************************************
函数名: voiceBroadcastSetRepetitionMode
功能: 设置RepetitionMode
入口参数: uint8_t Mode 0-4(全盘/文件夹/单曲/随机/无循环)
返回值: 无
*************************************************/
void voiceBroadcastSetRepetitionMode(uint8_t Mode)
{
	uint8_t CHK = 0x04^0x33^Mode;
	
	//1bit
	USART_SendData(USART3,0x7E);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//2bit
	USART_SendData(USART3,0x04);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//3bit
	USART_SendData(USART3,0x33);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//4bit
	USART_SendData(USART3,Mode);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//5bit
	USART_SendData(USART3,CHK);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//6bit
	USART_SendData(USART3,0xEF);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	
}

/*************************************************
函数名: playingMusic
功能: 播放
入口参数: 无
返回值: 无
*************************************************/
void playingMusic(void)
{
	uint8_t CHK = 0x03^0x01;
	
	//1bit
	USART_SendData(USART3,0x7E);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//2bit
	USART_SendData(USART3,0x03);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//3bit
	USART_SendData(USART3,0x01);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//4bit
	USART_SendData(USART3,CHK);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//5bit
	USART_SendData(USART3,0xEF);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
}

/*************************************************
函数名: voiceBroadcastPointStartPosition
功能: 设置StartPosition
入口参数: uint8_t file file为歌曲名字(001-255)
返回值: 无
*************************************************/
void voiceBroadcastPointStartPosition(uint8_t file)
{
	uint8_t CHK = 0x05^0x41^0x00^file;
	
	//1bit
	USART_SendData(USART3,0x7E);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//2bit
	USART_SendData(USART3,0x05);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//3bit
	USART_SendData(USART3,0x41);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//4bit
	USART_SendData(USART3,0x00);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//5bit
	USART_SendData(USART3,file);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//6bit
	USART_SendData(USART3,CHK);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//7bit
	USART_SendData(USART3,0xEF);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	
}

void USART3_IRQHandler()
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==1)
	{
		ACK_V[n_V]=USART_ReceiveData(USART3);
		n_V++;
		if(n_V==ACKS_V)
		{
			ACKSign_V=1;
		}
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
}
