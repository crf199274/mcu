#include "stm32f10x.h"
#include "iap.h"
#include "usart.h"
#include "stdelay.h"

int main()
{
	
	uint16_t t;
	uint16_t oldcount=0;				/* �ϵĴ��ڽ�������ֵ */
	uint16_t applenth=0;				/* ���յ���app���볤�� */
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� */
	
	stdelay_init(72); /* ��ʼ����ʱ */
	
	uart_init(115200);	/* ���ڳ�ʼ��Ϊ115200 */
	
//	if(((*(vu32*)(ApplicationAddress+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
//	{	 
//		iap_load_app(ApplicationAddress);//ִ��FLASH APP����
//	}else 
//	{
//		printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
//	}
	
	while(1)
	{
		if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
				printf("��ʼ���¹̼�...\r\n");
				if(applenth)
				{
					if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
					{	 
						iap_write_appbin(ApplicationAddress,USART_RX_BUF,applenth);//����FLASH����   
						printf("�̼��������!\r\n");	
						printf("��ʼִ��FLASH�û�����!!\r\n");
						iap_load_app(ApplicationAddress);//ִ��FLASH APP����
					}else 
					{   
						printf("��FLASHӦ�ó���!\r\n");
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
				printf("���³�ʱ\r\n");
				printf("��ʼִ��FLASH�û�����!!\r\n");
				if(((*(vu32*)(ApplicationAddress+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
//					SCB->VTOR = FLASH_BASE | 0x1400; /* Vector Table Relocation in Internal FLASH. */
					iap_load_app(ApplicationAddress);//ִ��FLASH APP����
				}else 
				{
					printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
				}	
			}
		}
	}
}
