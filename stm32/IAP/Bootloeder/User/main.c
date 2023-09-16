#include "stm32f10x.h"
#include "iap.h"
#include "usart.h"
#include "stdelay.h"
#include "led.h"

int main()
{
	uint16_t t=0;
	uint8_t ledStatus = 0;
	
	stdelay_init(72); /* 初始化延时 */
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 设置NVIC中断分组2:2位抢占优先级，2位响应优先级 */
	
	uart_init(115200);	/* 串口初始化为115200 */
	
	LED_Init();
	
	while(1)
	{
		stdelay_ms(10);
		t++;
		
		if(t%10==0)
		{
			ledStatus = 1 - ledStatus;
			if(ledStatus)
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			else
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
		}
		
		if(t==300)
		{
			t=0;
			if(USART1_RXNE()==NONE)
			{
				printf("更新超时\r\n");
				printf("开始执行FLASH用户代码!!\r\n");
				if(((*(vu32*)(ApplicationAddress+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					USART1_CLOSED();
					iap_load_app(ApplicationAddress);//执行FLASH APP代码
				}else 
				{
					printf("非FLASH应用程序,无法执行!\r\n");
				}	
			}
		}
		
		if(USART1_RXNE()==OK)//新周期内,没有收到任何数据,认为本次数据接收完成.
		{
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",USART_RX_LENTH);
				printf("开始更新固件...\r\n");
				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(ApplicationAddress,USART_RX_BUF,USART_RX_LENTH);//更新FLASH代码   
					printf("固件更新完成!\r\n");	
					printf("开始执行FLASH用户代码!!\r\n");
					iap_load_app(ApplicationAddress);//执行FLASH APP代码
				}else 
				{   
					printf("非FLASH应用程序!\r\n");
				}
		}
		
	}
}
