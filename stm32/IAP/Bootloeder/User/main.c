#include "stm32f10x.h"
#include "iap.h"
#include "usart.h"
#include "stdelay.h"
#include "led.h"

int main()
{
	uint16_t t=0;
	uint8_t ledStatus = 0;
	
	stdelay_init(72); /* ��ʼ����ʱ */
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� */
	
	uart_init(115200);	/* ���ڳ�ʼ��Ϊ115200 */
	
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
				printf("���³�ʱ\r\n");
				printf("��ʼִ��FLASH�û�����!!\r\n");
				if(((*(vu32*)(ApplicationAddress+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					USART1_CLOSED();
					iap_load_app(ApplicationAddress);//ִ��FLASH APP����
				}else 
				{
					printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
				}	
			}
		}
		
		if(USART1_RXNE()==OK)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
		{
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",USART_RX_LENTH);
				printf("��ʼ���¹̼�...\r\n");
				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(ApplicationAddress,USART_RX_BUF,USART_RX_LENTH);//����FLASH����   
					printf("�̼��������!\r\n");	
					printf("��ʼִ��FLASH�û�����!!\r\n");
					iap_load_app(ApplicationAddress);//ִ��FLASH APP����
				}else 
				{   
					printf("��FLASHӦ�ó���!\r\n");
				}
		}
		
	}
}
