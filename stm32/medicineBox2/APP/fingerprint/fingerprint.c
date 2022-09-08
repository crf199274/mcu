#include "fingerprint.h"
#include "stdelay.h"
#include "GUI.h"

//接收指纹返回值
uint8_t ACK[207]={0};
//指定返回值的字节数
uint8_t ACKS=0;
//接收完成标志
uint8_t ACKSign=0;
//中断统计接收次数
uint8_t n=0;

/*************************************************
函数名: fingerprint_usart_INIT
功能: 指纹串口初始化
入口参数: uint32_t BaudRate 波特率
返回值: 无
*************************************************/
void fingerprint_usart_INIT(uint32_t BaudRate)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_Init_my;
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_my);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init_my.GPIO_Pin=GPIO_Pin_3;
//	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_my);
	
	USART_InitTypeDef  USART_Init_my;
	USART_Init_my.USART_BaudRate=BaudRate;
	USART_Init_my.USART_WordLength=USART_WordLength_8b;
	USART_Init_my.USART_StopBits=USART_StopBits_1;
	USART_Init_my.USART_Parity=USART_Parity_No;
	USART_Init_my.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_Init_my.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_Init_my);
	
	USART_Cmd(USART2,ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//	USART_ITConfig(USART1,USART_IT_TC,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef  NVIC_Init_my;
	NVIC_Init_my.NVIC_IRQChannel=USART2_IRQn;
	NVIC_Init_my.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_Init_my.NVIC_IRQChannelSubPriority=3;
	NVIC_Init_my.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_Init_my);
	
}

/*************************************************
函数名: fingerprintIO_INIT
功能: 休眠控制IO初始化
入口参数: 无
返回值: 无
*************************************************/
void fingerprintIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_Init_my;
	
	RCC_APB2PeriphClockCmd(fingerprintSleepIOClock,ENABLE);
	
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_my.GPIO_Pin=fingerprintSleepIO;
	GPIO_Init_my.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(fingerprintSleepIOPort,&GPIO_Init_my);
	
	GPIO_Init_my.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init_my.GPIO_Pin=fingerprintWAKEIO;
	GPIO_Init(fingerprintWAKEIOPort,&GPIO_Init_my);
	
	GPIO_SetBits(fingerprintSleepIOPort,fingerprintSleepIO);
}

/*************************************************
函数名: fingerprintStaus
功能: 休眠控制
入口参数: staus 1-启动 0-休眠
返回值: 无
*************************************************/
void fingerprintStaus(uint8_t staus)
{
	if(staus)
	{
		GPIO_SetBits(fingerprintSleepIOPort,fingerprintSleepIO);
	}
	else
	{
		GPIO_ResetBits(fingerprintSleepIOPort,fingerprintSleepIO);
	}
}

/*************************************************
函数名: fingerprintINIT
功能: 指纹初始化
入口参数: uint32_t BaudRate 通信波特率
返回值: 无
*************************************************/
void fingerprintINIT(uint32_t BaudRate)
{
	fingerprint_usart_INIT(BaudRate);
	fingerprintIO_INIT();
//	stdelay_ms(100);
//	getfingerprintNotUsedID();
}

/*************************************************
函数名: CleanACK
功能: 清空数组ACK[]
入口参数: 无
返回值: 无
*************************************************/
void CleanACK(void)
{
	uint8_t i=0;
	for(i=0;i<193;i++)
	{
		ACK[i]=0;
	}
}

/*************************************************
函数名: fingerprintSendCMD
功能: 发送指纹采集命令
入口参数: uint8_t seq 第几次采集(共三次) uint8_t HID 用户ID高8位 uint8_t LID 用户ID低8位 
					uint8_t UserRank 用户权限等级(1最高，3最低)
返回值: 无
*************************************************/
void fingerprintSendCMD(uint8_t seq,uint8_t HID,uint8_t LID,uint8_t UserRank)
{
	n=0;
	//指定返回的字节数
	ACKS = 8;
	
	uint8_t CHK = seq^HID^LID^UserRank^0x00;
	//1bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//2bit
	USART_SendData(USART2,seq);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//3bit
	USART_SendData(USART2,HID);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//4bit
	USART_SendData(USART2,LID);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//5bit
	USART_SendData(USART2,UserRank);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//6bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//7bit
	USART_SendData(USART2,CHK);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//8bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	
}

/*************************************************
函数名: getfingerprintNotUsedID
功能: 得到一个空ID
入口参数: 无
返回值: 无
*************************************************/
void getfingerprintNotUsedID(void)
{
	n=0;
	//指定返回的字节数
	ACKS = 8;
	
	uint8_t CHK = 0x09^0x00^0x00^0x00^0x00;
	//1bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//2bit
	USART_SendData(USART2,0x09);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//3bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//4bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//5bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//6bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//7bit
	USART_SendData(USART2,CHK);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//8bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
}

/*************************************************
函数名: fingerprint
功能: 指纹注册
入口参数: 无
返回值: ACK_SUCCESS，ACK_FAIL
*************************************************/
uint8_t fingerprint(void)
{
	//延时变量
	uint32_t i=0;
	CleanACK();
	getfingerprintNotUsedID();
	i=0xFFFFFF;
	while(!(ACKSign==1)&&--i);
	ACKSign=0;
	if(i==0)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("串口接收失败1");
		#endif
		return ACK_FAIL;
	}
	if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
	{
		CleanACK();
		fingerprintSendCMD(0x01,ACK[2],ACK[3]+1,0x03);
		i=0xFFFFFF;
		while(!(ACKSign==1)&&--i);
		ACKSign=0;
		if(i==0)
		{
			#if DEBUG == 1
				GUI_INFOPRINT("串口接收失败2");
			#endif
			return ACK_FAIL;
		}
		if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
		{
			#if DEBUG == 1
				GUI_INFOPRINT("第一次录入指纹成功！");
			#endif
			CleanACK();
			fingerprintSendCMD(0x02,ACK[2],ACK[3]+1,0x03);
			i=0xFFFFFF;
			while(!(ACKSign==1)&&--i);
			ACKSign=0;
			if(i==0)
			{
				#if DEBUG == 1
					GUI_INFOPRINT("串口接收失败3");
				#endif
				return ACK_FAIL;
			}
			if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
			{
				#if DEBUG == 1
					GUI_INFOPRINT("第二次录入指纹成功！");
				#endif
				CleanACK();
				fingerprintSendCMD(0x03,ACK[2],ACK[3]+1,0x03);
				i=0xFFFFFF;
				while(!(ACKSign==1)&&--i);
				ACKSign=0;
				if(i==0)
				{
					#if DEBUG == 1
						GUI_INFOPRINT("串口接收失败4");
					#endif
					return ACK_FAIL;
				}
				if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
				{
					#if DEBUG == 1
						GUI_INFOPRINT("录入指纹成功！");
					#endif
					return ACK_SUCCESS;
				}else
				{
					#if DEBUG == 1
						GUI_INFOPRINT("第三次录入指纹失败！");
					#endif
					return ACK_FAIL;
				}
			}else
			{
				#if DEBUG == 1
						GUI_INFOPRINT("第二次录入指纹失败！");
					#endif
				return ACK_FAIL;
			}
		}else
		{
			#if DEBUG == 1
				GUI_INFOPRINT("第一次录入指纹失败！");
			#endif
			return ACK_FAIL;
		}
	}else
	{
		#if DEBUG == 1
			GUI_INFOPRINT("获取ID失败！");
		#endif
		return ACK_FAIL;
	}
}

/*************************************************
函数名: fingerprintDEL
功能: 删除指定用户
入口参数: uint8_t HID 用户ID高8位 uint8_t LID 用户ID低8位
返回值: ACK_SUCCESS，ACK_FAIL
*************************************************/
uint8_t fingerprintDEL(uint8_t HID,uint8_t LID)
{
	//指定返回的字节数
	ACKS = 8;
	//延时变量
	uint32_t i=0;
	//字节计数变量清零
	n=0;
	
	uint8_t CHK = 0x04^HID^LID^0x00^0x00;
	
	CleanACK();
	//1bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//2bit
	USART_SendData(USART2,0x04);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//3bit
	USART_SendData(USART2,HID);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//4bit
	USART_SendData(USART2,LID);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//5bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//6bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//7bit
	USART_SendData(USART2,CHK);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//8bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	
	i=0x1FFFF;
	while(!(ACKSign==1)&&i--);
	ACKSign=0;
	
	if(i==0)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("串口接收失败");
		#endif
		return ACK_FAIL;
	}
	
	if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("删除成功");
		#endif
		return ACK_SUCCESS;
	}else{
		#if DEBUG == 1
			GUI_INFOPRINT("删除失败");
		#endif
		return ACK_FAIL;
	}
}

/*************************************************
函数名: fingerprintDEL
功能: 删除所有用户
入口参数: uint8_t HID 用户ID高8位 uint8_t LID 用户ID低8位
返回值: ACK_SUCCESS，ACK_FAIL
*************************************************/
uint8_t fingerprintDELALL(void)
{
	//指定返回的字节数
	ACKS = 8;
	//延时变量
	uint32_t i=0;
	//字节计数变量清零
	n=0;
	
	uint8_t CHK = 0x05^0x00^0x00^0x00^0x00;
	
	CleanACK();
	//1bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//2bit
	USART_SendData(USART2,0x05);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//3bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//4bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//5bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//6bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//7bit
	USART_SendData(USART2,CHK);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//8bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	
	i=0xFFFFFF;
	while(!(ACKSign==1)&&i--);
	ACKSign=0;
	
	if(i==0)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("串口接收失败");
		#endif
		return ACK_FAIL;
	}
	
	if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("删除成功");
		#endif
		return ACK_SUCCESS;
	}else{
		#if DEBUG == 1
			GUI_INFOPRINT("删除失败");
		#endif
		return ACK_FAIL;
	}
}

/*************************************************
函数名: fingerprintCompare()
功能: 指纹比较
入口参数: 无
返回值: ACK_SUCCESS，ACK_FAIL
*************************************************/
uint8_t fingerprintCompare(void)
{
	//指定返回的字节数
	ACKS = 8;
	//延时变量
	uint32_t i = 0;
	//字节计数变量清零
	n=0;
	
	uint8_t CHK = 0x0C^0x00^0x00^0x00^0x00;
	
	
	CleanACK();
	//1bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//2bit
	USART_SendData(USART2,0x0C);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//3bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//4bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//5bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//6bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//7bit
	USART_SendData(USART2,CHK);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//8bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	
	while(1)
	{
		if(ACKSign==1)
		{
			ACKSign=0;
			break;
		}
		
		if(i>=100000)
		{
			i=0;
			break;
		}
		
		stdelay_us(10);
		i++;
		
	}
	
	if(i==0)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("串口接收失败");
		#endif
		return ACK_FAIL;
	}
	
	if(ACK[0]==0xF5&&ACK[3]!=0x00)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("识别通过");
		#endif
		return ACK_SUCCESS;
	}
	else
	{
		#if DEBUG == 1
			GUI_INFOPRINT("识别失败");
		#endif
		return ACK_FAIL;
	}
}

/*************************************************
函数名: fingerprintDownloadEigenvalue()
功能: 下载特征值
入口参数: uint8_t Eigenvalue[] 接收数据 uint8_t HID 用户ID高8位 uint8_t LID 用户ID低8位
返回值: ACK_SUCCESS，ACK_FAIL
*************************************************/
uint8_t fingerprintDownloadEigenvalue(uint8_t Eigenvalue[],uint8_t HID,uint8_t LID,uint8_t len)
{
	//指定返回的字节数
	ACKS = 207;
	//延时变量
	uint32_t i=0;
	//字节计数变量清零
	n=0;
	
	uint8_t CHK = 0x31^HID^LID^0x00^0x00;
	
	if(len!=207)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("数组长度不足");
		#endif
		return ACK_FAIL;
	}
	
	CleanACK();
	//1bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//2bit
	USART_SendData(USART2,0x31);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//3bit
	USART_SendData(USART2,HID);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//4bit
	USART_SendData(USART2,LID);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//5bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//6bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//7bit
	USART_SendData(USART2,CHK);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//8bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	
	i=0xFFFFFF;
	while(!(ACKSign==1)&&i--);
	ACKSign=0;
	
	if(i==0)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("串口接收失败");
		#endif
		return ACK_FAIL;
	}
	
	if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("特征值下载完毕");
		#endif
		for(i=0;i<207;i++)
		{
			Eigenvalue[i]=ACK[i];
		}
		return ACK_SUCCESS;
	}else{
		#if DEBUG == 1
			GUI_INFOPRINT("特征值下载失败");
		#endif
		return ACK_FAIL;
	}
}

/*************************************************
函数名: fingerprintInstallEigenvalue()
功能: 上传特征值
入口参数: uint8_t Eigenvalue[] 接收数据
返回值: ACK_SUCCESS，ACK_FAIL
*************************************************/
uint8_t fingerprintInstallEigenvalue(uint8_t Eigenvalue[])
{
	//延时变量
	uint32_t i=0;
	//字节计数变量清零
	n=0;
	//异或校验和
	uint8_t CHK1=0;
	uint8_t CHK2=0;
	
	CleanACK();
	
	getfingerprintNotUsedID();
	i=0x1FFFF;
	while(!(ACKSign==1)&&i--);
	ACKSign=0;
	if(i==0)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("串口接收失败");
		#endif
		return ACK_FAIL;
	}
	if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
	{
		CleanACK();
		//指定返回的字节数
		ACKS = 8;
		//字节计数变量清零
		n=0;
		//异或校验和
		CHK1 = 0x41^0x00^0xC4^0x00^0x00;
		CHK2 = ACK[2]^(ACK[3]+1)^0x03;
		for(i=0;i<193;i++)
		{
			CHK2 ^= Eigenvalue[i];
		}
		
		//数据头
		//1bit
		USART_SendData(USART2,0xF5);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		//2bit
		USART_SendData(USART2,0x41);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		//3bit
		USART_SendData(USART2,0x00);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		//4bit
		USART_SendData(USART2,0xC4);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		//5bit
		USART_SendData(USART2,0x00);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		//6bit
		USART_SendData(USART2,0x00);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		//7bit
		USART_SendData(USART2,CHK1);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		//8bit
		USART_SendData(USART2,0xF5);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		
		//数据包
		//1bit
		USART_SendData(USART2,0xF5);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		
		//2bit
		USART_SendData(USART2,ACK[2]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		
		//3bit
		USART_SendData(USART2,ACK[3]+1);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		
		//4bit
		USART_SendData(USART2,0x03);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		
		//5-198bit
		for(i=0;i<193;i++)
		{
			USART_SendData(USART2,Eigenvalue[i]);
			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
			USART_ClearFlag(USART2,USART_FLAG_TC);
		}
		
		//199bit
		USART_SendData(USART2,CHK2);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		
		//200bit
		USART_SendData(USART2,0xF5);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		
		i=0xFFFFFF;
		while(!(ACKSign==1)&&i--);
		ACKSign=0;
		
		if(i==0)
		{
			#if DEBUG == 1
				GUI_INFOPRINT("串口接收失败");
			#endif
			return ACK_FAIL;
		}
		if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
		{
			#if DEBUG == 1
				GUI_INFOPRINT("上传特征值成功");
			#endif
			return ACK_SUCCESS;
		}else{
			#if DEBUG == 1
				GUI_INFOPRINT("上传特征值失败");
			#endif
			for(i=0;i<8;i++)
			{
				USART_SendData(USART1,ACK[i]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=1);
				USART_ClearFlag(USART1,USART_FLAG_TC);
			}
			return ACK_FAIL;
		}
	}else{
		#if DEBUG == 1
			GUI_INFOPRINT("获取ID失败");
		#endif
		return ACK_FAIL;
	}
}

/*************************************************
函数名: getUserCount()
功能: 获取总用户数量
入口参数: 无
返回值: ACK_SUCCESS，ACK_FAIL
*************************************************/
uint8_t getUserCount(void)
{
	//延时变量
	uint32_t i=0;
	//字节计数变量清零
	n=0;
	//指定返回的字节数
	ACKS = 8;
	
	uint8_t CHK = 0x09^0x00^0x00^0x00^0x00;
	
	CleanACK();
	//1bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//2bit
	USART_SendData(USART2,0x09);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//3bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//4bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//5bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//6bit
	USART_SendData(USART2,0x00);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//7bit
	USART_SendData(USART2,CHK);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	//8bit
	USART_SendData(USART2,0xF5);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	
	i=0x1FFFF;
	while(!(ACKSign==1)&&i--);
	ACKSign=0;
	
	if(i==0)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("串口接收失败");
		#endif
		return ACK_FAIL;
	}
	
	if(ACK[0]==0xF5&&ACK[4]==ACK_SUCCESS)
	{
		#if DEBUG == 1
			GUI_INFOPRINT("OK");
		#endif
		return ACK_SUCCESS;
	}else{
		#if DEBUG == 1
			GUI_INFOPRINT("获取总用户数量失败");
		#endif
		return ACK_FAIL;
	}
}

void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==1)
	{
		ACK[n]=USART_ReceiveData(USART2);
		n++;
		if(n==ACKS)
		{
			ACKSign=1;
		}
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
}

