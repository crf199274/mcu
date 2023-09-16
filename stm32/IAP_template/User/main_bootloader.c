#include "stm32f10x.h"
#include "iap.h"
#include "usart.h"
#include "stdelay.h"

int main()
{
	
	uint16_t t;
	uint16_t oldcount=0;				/* 老的串口接收数据值 */
	uint16_t applenth=0;				/* 接收到的app代码长度 */
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 设置NVIC中断分组2:2位抢占优先级，2位响应优先级 */
	
	stdelay_init(72); /* 初始化延时 */
	
	uart_init(115200);	/* 串口初始化为115200 */
	
//	if(((*(vu32*)(ApplicationAddress+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
//	{	 
//		iap_load_app(ApplicationAddress);//执行FLASH APP代码
//	}else 
//	{
//		printf("非FLASH应用程序,无法执行!\r\n");
//	}
	
	while(1)
	{
		if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
				printf("开始更新固件...\r\n");
				if(applenth)
				{
					if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
					{	 
						iap_write_appbin(ApplicationAddress,USART_RX_BUF,applenth);//更新FLASH代码   
						printf("固件更新完成!\r\n");	
						printf("开始执行FLASH用户代码!!\r\n");
						iap_load_app(ApplicationAddress);//执行FLASH APP代码
					}else 
					{   
						printf("非FLASH应用程序!\r\n");
					}
				}
			}else oldcount=USART_RX_CNT;			
		}
		t++;
		stdelay_ms(10);
		if(t==300)
		{
			if(USART_RX_CNT==0)
			{
				printf("更新超时\r\n");
				printf("开始执行FLASH用户代码!!\r\n");
				if(((*(vu32*)(ApplicationAddress+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
//					SCB->VTOR = FLASH_BASE | 0x1400; /* Vector Table Relocation in Internal FLASH. */
					iap_load_app(ApplicationAddress);//执行FLASH APP代码
				}else 
				{
					printf("非FLASH应用程序,无法执行!\r\n");
				}	
			}
		}
	}
}
