#include "iap.h"

pFunction       Jump_To_Application; /* ����һ��App��ת���� */
uint16_t iapbuf[1024]; /* �������ݻ��� */
/*
 * funtion: ����ջ����ַ
 * addr: ջ����ַ
*/
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

/*
 * funtion: ����������д��App�洢�ռ�
 * appxaddr: Ӧ�ó������ʼ��ַ
 * appbuf: Ӧ�ó���CODE.
 * appsize: Ӧ�ó����С(�ֽ�).
 * return: none
*/
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
	uint16_t t;
	uint16_t i=0;
	uint16_t temp;
	uint32_t fwaddr=appxaddr;//��ǰд��ĵ�ַ
	uint8_t *dfu=appbuf;
	for(t=0;t<appsize;t+=2)
	{						    
		temp=(uint16_t)dfu[1]<<8;
		temp+=(uint16_t)dfu[0];	  
		dfu+=2;//ƫ��2���ֽ�
		iapbuf[i++]=temp;	    
		if(i==1024)
		{
			i=0;
			STMFLASH_Write(fwaddr,iapbuf,1024);	
			fwaddr+=2048;//ƫ��2048  16=2*8.����Ҫ����2.
		}
	}
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
}

/*
 * funtion: ��ת��Ӧ�ó����
 * appxaddr:�û�������ʼ��ַ.
 * return: none
*/
void iap_load_app(uint32_t appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		Jump_To_Application=(pFunction)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		Jump_To_Application();									//��ת��APP.
	}
}	


